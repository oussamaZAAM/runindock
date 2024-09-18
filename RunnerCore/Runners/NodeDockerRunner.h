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
    }
};

// Register the NodeyDockerRunner when this file is included
static bool nodeRunnerRegistered = (NodeDockerRunner::registerRunner(), true);

#endif // NODEDOCKERRUNNER_H
