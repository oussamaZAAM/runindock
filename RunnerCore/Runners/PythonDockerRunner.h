#ifndef PYTHONDOCKERRUNNER_H
#define PYTHONDOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class PythonDockerRunner : public DockerRunner {
public:
    PythonDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "python:alpine";
    }

    // Automatically register the runner for the "python" environment
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("python", [](const std::string& image) {
            return std::make_unique<PythonDockerRunner>(image);
        });
    }
};

// Register the PythonyDockerRunner when this file is included
static bool pythonRunnerRegistered = (PythonDockerRunner::registerRunner(), true);

#endif // PYTHONDOCKERRUNNER_H
