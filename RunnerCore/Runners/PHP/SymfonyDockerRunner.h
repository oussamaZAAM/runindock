#ifndef SYMFONYDOCKERRUNNER_H
#define SYMFONYDOCKERRUNNER_H

#include "../../DockerRunner.h"
#include "../../DockerRunnerRegistry.h"

class SymfonyDockerRunner : public DockerRunner {
public:
    SymfonyDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "php:alpine";
    }

    // Automatically register the runner for the "php" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Python environments
        for (const auto& env : {"php", "composer"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<SymfonyDockerRunner>(image);
            });
        }
    }

    // Override the buildCommand method
    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        DockerCommandBuilder builder;

        // Set the basic parameters of the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .addPreRunCommand("apk add curl unzip")
               .addPreRunCommand("curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer")
               .setPort(getOption("port"));

        // Build and return the full Docker command
        return builder.build();
    }
};

// Register the PhpyDockerRunner when this file is included
static bool symfonyRunnerRegistered = (SymfonyDockerRunner::registerRunner(), true);

#endif // SYMFONYDOCKERRUNNER_H
