#ifndef CCPPDOCKERRUNNER_H
#define CCPPDOCKERRUNNER_H

#include <memory>
#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class CCppDockerRunner : public DockerRunner {
public:
    CCppDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "gcc";
    }

    // Automatically register the runner for the "cpp" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple C/C++ environments
        for (const auto& env : {"gpp", "g++"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<CCppDockerRunner>(image);
            });
        }
    }
};

// Register the GoDockerRunner when this file is included
static bool ccppRunnerRegistered = (CCppDockerRunner::registerRunner(), true);

#endif // CCPPDOCKERRUNNER_H
