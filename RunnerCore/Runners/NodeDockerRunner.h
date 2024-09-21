#ifndef NODEDOCKERRUNNER_H
#define NODEDOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class NodeDockerRunner : public DockerRunner {
public:
    NodeDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "node:alpine";
    }

    // Automatically register the runner for the "node" environment
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("node", [](const std::string& image) {
            return std::make_unique<NodeDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("npm", [](const std::string& image) {
            return std::make_unique<NodeDockerRunner>(image);
        });
    }

    // Override the preRunHook to update the .env file before running the command
    void preRunHook(std::string& dockerCommand) const override {
        // If a port is specified, update the .env file inside the Docker container
        if (!port.empty()) {
            // Command to update the PORT value in .env (or create it if not existing)
            std::string updateEnvCommand = dockerCommand + getDockerImage() + " /bin/sh -c \""
                                           "if [ -f .env ]; then "
                                           "sed -i 's/^PORT=.*/PORT=" + port + "/' .env; "
                                           "else "
                                           "echo 'PORT=" + port + "' > .env; "
                                           "fi\"";

            std::cout << "Updating .env with PORT=" << port << std::endl;
            int updateEnvResult = system(updateEnvCommand.c_str());
            if (updateEnvResult != 0) {
                std::cerr << "Error updating .env file." << std::endl;
            }
        }
    }
};

// Register the NodeyDockerRunner when this file is included
static bool nodeRunnerRegistered = (NodeDockerRunner::registerRunner(), true);

#endif // NODEDOCKERRUNNER_H
