#ifndef RUSTDOCKERRUNNER_H
#define RUSTDOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class RustDockerRunner : public DockerRunner {
public:
    RustDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "rust:alpine";
    }

    // Automatically register the runner for the "rust" environment
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("cargo", [](const std::string& image) {
            return std::make_unique<RustDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("rustc", [](const std::string& image) {
            return std::make_unique<RustDockerRunner>(image);
        });
    }

    // Overriding the run function to handle Rust commands
    void run(const std::string& command) const override {
        // Get the current working directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            exit(1);
        }

        std::string dockerCommand = "docker run --rm -v \"" + std::string(cwd) + "\":/usr/src/myapp -w /usr/src/myapp ";
        dockerCommand += getDefaultImage();  // Use the provided or default Rust image

        // Check if the command is for compiling or running a Rust project
        if (command.find("rustc") == 0) {
            // If the command starts with rustc, compile the specific file
            std::string file = command.substr(6);  // Extract file name after "rustc "
            dockerCommand += " rustc " + file;
        } else if (command.find("cargo") == 0) {
            // If the command is for running a Rust project, use cargo run
            dockerCommand += " " + command;
        } else {
            std::cerr << "Unsupported command for RustDockerRunner: " << command << std::endl;
            return;
        }

        // Execute the Docker command
        int result = system(dockerCommand.c_str());
        if (result != 0) {
            std::cerr << "Error executing Docker command for Rust." << std::endl;
        }
    }
};

// Register the RustDockerRunner when this file is included
static bool rustRunnerRegistered = (RustDockerRunner::registerRunner(), true);

#endif // RUSTDOCKERRUNNER_H
