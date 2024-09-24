#ifndef LARAVELDOCKERRUNNER_H
#define LARAVELDOCKERRUNNER_H

#include "../../DockerRunner.h"
#include "../../DockerRunnerRegistry.h"

class LaravelDockerRunner : public DockerRunner {
public:
    LaravelDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "php:alpine";
    }

    // Automatically register the runner for the "php" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Python environments
        for (const auto& env : {"php", "composer"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<LaravelDockerRunner>(image);
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
               .setPort(getOption("port"))
               .addPreRunCommand("apk add curl unzip")
               .addPreRunCommand("curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer")
               .addVolume("php_shared_vol:/var/www");

        // Build and return the full Docker command
        return builder.build();
    }
};

// Register the PhpyDockerRunner when this file is included
static bool laravelRunnerRegistered = (LaravelDockerRunner::registerRunner(), true);

#endif // LARAVELDOCKERRUNNER_H
