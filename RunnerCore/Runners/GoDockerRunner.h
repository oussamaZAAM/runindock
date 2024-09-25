#ifndef GODOCKERRUNNER_H
#define GODOCKERRUNNER_H

#include <memory>
#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class GoDockerRunner : public DockerRunner {
public:
    GoDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "golang:alpine";
    }

    // Automatically register the runner for the "go" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Go environments
        for (const auto& env : {"go"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<GoDockerRunner>(image);
            });
        }
    }
};

// Register the GoDockerRunner when this file is included
static bool goRunnerRegistered = (GoDockerRunner::registerRunner(), true);

#endif // GODOCKERRUNNER_H
