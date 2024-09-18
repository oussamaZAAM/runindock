#ifndef DOCKERRUNNER_H
#define DOCKERRUNNER_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

class DockerRunner {
protected:
    std::string customImage;

public:
    DockerRunner(const std::string& image = "") : customImage(image) {}

    virtual ~DockerRunner() = default;

    // The function that child classes will implement
    virtual std::string getDefaultImage() const = 0;

    // Function to return the image to be used (default or overridden)
    std::string getDockerImage() const {
        return customImage.empty() ? getDefaultImage() : customImage;
    }

    // Function to run the command in Docker using the appropriate image
    virtual void run(const std::string& command) const {
        // Get current working directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            exit(1);
        }

        // Construct the Docker command with the language-specific image
        std::string dockerCommand = "docker run --rm -v \"" + std::string(cwd) + "\":/app -w /app " + getDockerImage() + " /bin/sh -c \"" + command + "\"";

        // Execute the Docker command
        int result = system(dockerCommand.c_str());
        if (result != 0) {
            std::cerr << "Error executing Docker command." << std::endl;
        }
    }
};

#endif // DOCKERRUNNER_H
