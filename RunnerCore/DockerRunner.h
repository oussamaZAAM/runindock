#ifndef DOCKERRUNNER_H
#define DOCKERRUNNER_H

#include "DockerCommandBuilder.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#ifdef _WIN32
    // Windows-specific includes
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
    #include <process.h>
    #include <conio.h>     // For _kbhit and _getch
    #include <direct.h>    // For _getcwd
#else
    // POSIX-specific includes
    #include <unistd.h>
    #include <pty.h>
    #include <sys/wait.h>
    #include <fcntl.h>
    #include <sys/select.h>
    #include <errno.h>
#endif

class DockerRunner {
protected:
    std::string customImage;
    std::string port;

public:
    DockerRunner(const std::string& image = "") : customImage(image), port("") {}

    virtual ~DockerRunner() = default;

    // Set the port for the Docker container
    void setPort(const std::string& portValue) {
        port = portValue;
    }

    // The function that child classes will implement
    virtual std::string getDefaultImage() const = 0;

    // Function to return the image to be used (default or overridden)
    std::string getDockerImage() const {
        return customImage.empty() ? getDefaultImage() : customImage;
    }

    // Method to allow derived classes to customize pre-run steps
    virtual void preRunHook(std::string& dockerCommand) const {
        // By default, do nothing; subclasses can override this to add behavior
    }

    // Method to retrieve the current working directory (cwd)
    std::string getCwd() const {
        char cwdBuffer[1024];
        std::string cwd;

#ifdef _WIN32
        if (_getcwd(cwdBuffer, sizeof(cwdBuffer)) == NULL) {
            perror("_getcwd() error");
            exit(1);
        }
#else
        if (getcwd(cwdBuffer, sizeof(cwdBuffer)) == NULL) {
            perror("getcwd() error");
            exit(1);
        }
#endif
        return std::string(cwdBuffer);
    }

    // Method to write default command and allow derived classes to override the command
    virtual std::string buildCommand(const std::string& cwd, const std::string &command) const {
        DockerCommandBuilder builder;

        // Set the basic parameters of the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .setPort(port);

        // Build and return the full Docker command
        return builder.build();
    }

    // Function to run the command in Docker using the appropriate image
    void run(std::string& command) const {
        // Get current working directory
        std::string cwd = getCwd();
        std::string fullCommand = buildCommand(cwd, command);

#ifdef _WIN32
        // Windows-specific implementation
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        SECURITY_ATTRIBUTES sa;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags |= STARTF_USESTDHANDLES;

        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;

        HANDLE hStdInRead = NULL;
        HANDLE hStdInWrite = NULL;
        HANDLE hStdOutRead = NULL;
        HANDLE hStdOutWrite = NULL;

        // Create pipes for STDIN and STDOUT
        if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
            std::cerr << "Error creating STDIN pipe." << std::endl;
            return;
        }
        if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << "Error setting handle information for STDIN write pipe." << std::endl;
            return;
        }

        if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) {
            std::cerr << "Error creating STDOUT pipe." << std::endl;
            return;
        }
        if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
            std::cerr << "Error setting handle information for STDOUT read pipe." << std::endl;
            return;
        }

        si.hStdInput = hStdInRead;
        si.hStdOutput = hStdOutWrite;
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE); // You can also create a pipe for STDERR if needed

        // Create the child process
        BOOL success = CreateProcessA(
            NULL,
            const_cast<char*>(fullCommand.c_str()),
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &si,
            &pi
        );

        if (!success) {
            std::cerr << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
            return;
        }

        // Close handles to the child's STDIN pipe that the parent doesn't need
        CloseHandle(hStdInRead);

        // Close handles to the child's STDOUT pipe that the parent doesn't need
        CloseHandle(hStdOutWrite);

        // Main loop to handle input/output
        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

        DWORD dwRead, dwWritten;
        CHAR buffer[4096];
        BOOL bSuccess;

        // Set console mode to enable virtual terminal processing
        DWORD consoleMode = 0;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleMode(hConsole, &consoleMode)) {
            consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hConsole, consoleMode);
        }

        while (true) {
            // Check if child process has terminated
            DWORD exitCode;
            if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
                std::cerr << "Failed to get child process exit code." << std::endl;
                break;
            }
            if (exitCode != STILL_ACTIVE) {
                break;
            }

            // Read from child process's STDOUT and write to parent's STDOUT
            bSuccess = PeekNamedPipe(hStdOutRead, NULL, 0, NULL, &dwRead, NULL);
            if (bSuccess && dwRead > 0) {
                bSuccess = ReadFile(hStdOutRead, buffer, sizeof(buffer), &dwRead, NULL);
                
                // Insert a newline to prevent prompt overlap
                std::cout << "" << std::endl;
                
                if (bSuccess && dwRead > 0) {
                    for (DWORD i = 0; i < dwRead; ++i) {
                        char ch = buffer[i];

                        if (ch == '\n') {
                            // Check if the previous character is not '\r'
                            if (i == 0 || buffer[i - 1] != '\r') {
                                char cr = '\r';
                                WriteFile(hStdOut, &cr, 1, &dwWritten, NULL);
                            }
                        }

                        WriteFile(hStdOut, &ch, 1, &dwWritten, NULL);
                    }
                }
            }

            // Inside the Windows-specific loop
            if (_kbhit()) {
                int ch = _getch();
                buffer[0] = static_cast<char>(ch);
                
                // Write to child STDIN
                bSuccess = WriteFile(hStdInWrite, buffer, 1, &dwWritten, NULL);
                if (!bSuccess) {
                    std::cerr << "Failed to write to child STDIN." << std::endl;
                    break;
                }

                // Echo back to parent's STDOUT
                bSuccess = WriteFile(hStdOut, buffer, 1, &dwWritten, NULL);
                if (!bSuccess) {
                    std::cerr << "Failed to echo to parent STDOUT." << std::endl;
                    break;
                }

                // Handle special keys (e.g., arrow keys, function keys)
                if (ch == 0 || ch == 224) {
                    ch = _getch(); // Get the second byte of special key
                    buffer[0] = static_cast<char>(ch);
                    
                    // Write to child STDIN
                    bSuccess = WriteFile(hStdInWrite, buffer, 1, &dwWritten, NULL);
                    if (!bSuccess) {
                        std::cerr << "Failed to write special key to child STDIN." << std::endl;
                        break;
                    }

                    // Echo back special key
                    bSuccess = WriteFile(hStdOut, buffer, 1, &dwWritten, NULL);
                    if (!bSuccess) {
                        std::cerr << "Failed to echo special key to parent STDOUT." << std::endl;
                        break;
                    }
                }
            }

            Sleep(10); // Prevent high CPU usage
        }

        // Wait until child process exits
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close handles
        CloseHandle(hStdInWrite);
        CloseHandle(hStdOutRead);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

#else
        // POSIX-specific implementation
        int master_fd;
        pid_t pid = forkpty(&master_fd, nullptr, nullptr, nullptr);

        if (pid == -1) {
            perror("forkpty failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            // Execute the Docker command
            execlp("/bin/sh", "sh", "-c", dockerCommand.c_str(), nullptr);
            perror("execlp failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            // Set stdin to non-blocking mode
            int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

            // Set master_fd to non-blocking mode
            flags = fcntl(master_fd, F_GETFL, 0);
            fcntl(master_fd, F_SETFL, flags | O_NONBLOCK);

            while (true) {
                fd_set readfds;
                FD_ZERO(&readfds);
                FD_SET(master_fd, &readfds);
                FD_SET(STDIN_FILENO, &readfds);

                int maxfd = (master_fd > STDIN_FILENO) ? master_fd : STDIN_FILENO;

                int ret = select(maxfd + 1, &readfds, NULL, NULL, NULL);
                if (ret == -1) {
                    perror("select failed");
                    break;
                }

                if (FD_ISSET(master_fd, &readfds)) {
                    char buffer[256];
                    ssize_t n = read(master_fd, buffer, sizeof(buffer));
                    if (n > 0) {
                        write(STDOUT_FILENO, buffer, n);
                    } else if (n == 0) {
                        break; // EOF
                    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("read from master_fd failed");
                        break;
                    }
                }

                if (FD_ISSET(STDIN_FILENO, &readfds)) {
                    char buffer[256];
                    ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer));
                    if (n > 0) {
                        write(master_fd, buffer, n);
                        // Echo the input back to stdout
                        write(STDOUT_FILENO, buffer, n);
                    } else if (n == 0) {
                        // EOF on stdin
                        break;
                    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("read from stdin failed");
                        break;
                    }
                }
            }

            int status;
            waitpid(pid, &status, 0);

            if (status != 0) {
                std::cerr << "Error executing Docker command." << std::endl;
            }
        }
#endif
    }
};

#endif // DOCKERRUNNER_H