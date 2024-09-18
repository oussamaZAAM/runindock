#ifndef PHPDOCKERRUNNER_H
#define PHPDOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class PhpDockerRunner : public DockerRunner {
public:
    PhpDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "php:alpine";
    }

    // Automatically register the runner for the "php" environment
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("php", [](const std::string& image) {
            return std::make_unique<PhpDockerRunner>(image);
        });
    }
};

// Register the PhpyDockerRunner when this file is included
static bool phpRunnerRegistered = (PhpDockerRunner::registerRunner(), true);

#endif // PHPDOCKERRUNNER_H
