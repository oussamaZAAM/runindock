#ifndef MAVENDOCKERRUNNER_H
#define MAVENDOCKERRUNNER_H

#include "../../DockerRunner.h"
#include "../../DockerRunnerRegistry.h"
#include "../../DockerCommandBuilder.h"

class MavenDockerRunner : public DockerRunner {
public:
    MavenDockerRunner(const std::string& image = "") : DockerRunner() {
        if (!image.empty()) {
            setOption("image", image);
        }
    }

    // Return Maven's default image
    virtual std::string getDefaultImage() const override {
        return "maven:latest";  // Specific Maven image with JDK 11
    }

    // Automatically register the runner for "maven" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        registry.registerRunner("mvn", [](const std::string& image) -> std::unique_ptr<DockerRunner> {
            return std::make_unique<MavenDockerRunner>(image);
        });
    }

    // Override the buildCommand method
    virtual std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        DockerCommandBuilder builder;

        // Set the basic parameters of the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .addVolume("maven_shared_vol:/root/.m2")
               .setPort(getOption("port"));

        // Build and return the full Docker command
        return builder.build();
    }
};

// Automatically register the MavenDockerRunner when this file is included
static bool mavenRunnerRegistered = (MavenDockerRunner::registerRunner(), true);

#endif // MAVENDOCKERRUNNER_H
