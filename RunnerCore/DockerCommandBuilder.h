#ifndef DOCKERCOMMANDBUILDER_H
#define DOCKERCOMMANDBUILDER_H

#include <string>
#include <vector>

class DockerCommandBuilder {
private:
    std::string cwd;
    std::string image;
    std::string userCommand;
    std::vector<std::string> volumes;
    std::vector<std::string> envFiles;
    std::vector<std::string> preRunCommands;
    std::string port;

public:
    DockerCommandBuilder& setWorkingDirectory(const std::string& workingDirectory) {
        this->cwd = workingDirectory;
        return *this;
    }

    DockerCommandBuilder& setDockerImage(const std::string& dockerImage) {
        this->image = dockerImage;
        return *this;
    }

    DockerCommandBuilder& setUserCommand(const std::string& command) {
        this->userCommand = command;
        return *this;
    }

    DockerCommandBuilder& addVolume(const std::string& volume) {
        this->volumes.push_back(volume);
        return *this;
    }

    DockerCommandBuilder& addEnvFile(const std::string& envFile) {
        this->envFiles.push_back(envFile);
        return *this;
    }

    DockerCommandBuilder& addPreRunCommand(const std::string& preRunCommand) {
        this->preRunCommands.push_back(preRunCommand);
        return *this;
    }

    DockerCommandBuilder& setPort(const std::string& portMapping) {
        this->port = portMapping;
        return *this;
    }

    std::string build() const {
        // Construct the base Docker command
        std::string dockerCommand = "docker run --rm -i -v \"" + cwd + "\":/app -w /app ";

        // Add ports if any
        if (!port.empty()) {
            dockerCommand += "-p " + port + ":" + port + " ";
        }

        // Add volumes
        for (const auto& volume : volumes) {
            dockerCommand += "-v " + volume + " ";
        }

        // Add environment files
        for (const auto& envFile : envFiles) {
            dockerCommand += "--env-file " + envFile + " ";
        }

        // Add pre-run commands
        std::string preRunSection;
        for (const auto& preRunCommand : preRunCommands) {
            preRunSection += preRunCommand + " && ";
        }

        // Final command to run inside the container
        dockerCommand += image + " /bin/sh -c \"" + preRunSection + userCommand + "\"";

        return dockerCommand;
    }
};

#endif // DOCKERCOMMANDBUILDER_H
