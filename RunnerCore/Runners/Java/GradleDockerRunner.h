#ifndef GRADLEDOCKERRUNNER_H
#define GRADLEDOCKERRUNNER_H

#include "../../DockerRunner.h"
#include "../../DockerRunnerRegistry.h"
#include "../../DockerCommandBuilder.h"

class GradleDockerRunner : public DockerRunner {
public:
    GradleDockerRunner(const std::string& image = "") : DockerRunner() {
        if (!image.empty()) {
            setOption("image", image);
        }
    }

    // Return Gradle's default image
    virtual std::string getDefaultImage() const override {
        return "gradle:alpine";
    }

    // Automatically register the runner for the "gradle" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        registry.registerRunner("gradle", [](const std::string& image) -> std::unique_ptr<DockerRunner> {
            return std::make_unique<GradleDockerRunner>(image);
        });
    }

    // Override the buildCommand method
    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        DockerCommandBuilder builder;

        // Set the basic parameters of the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .setPort(getOption("port"));

        // Build and return the full Docker command
        return builder.build();
    }
};

// Automatically register the GradleDockerRunner when this file is included
static bool gradleRunnerRegistered = (GradleDockerRunner::registerRunner(), true);

#endif // GRADLEDOCKERRUNNER_H
