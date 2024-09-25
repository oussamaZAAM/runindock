#ifndef PHPDOCKERRUNNER_H
#define PHPDOCKERRUNNER_H

#include "../../DockerRunner.h"
#include "../../DockerRunnerRegistry.h"
#include "./LaravelDockerRunner.h"
#include "./SymfonyDockerRunner.h"

class PhpDockerRunner : public DockerRunner {
public:
    PhpDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "php:alpine";
    }

    // Automatically register the runner for the "php" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Python environments
        for (const auto& env : {"php", "composer"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<PhpDockerRunner>(image);
            });
        }
    }

    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        std::string framework = getOption("framework");
        std::map<std::string, std::string> options = getOptions();
        if (framework == "laravel"){
            LaravelDockerRunner laravelRunner;
            laravelRunner.setOptions(options);
            return laravelRunner.buildCommand(cwd, command);
        } else if (framework == "symfony") {
            SymfonyDockerRunner symfonyRunner;
            symfonyRunner.setOptions(options);
            return symfonyRunner.buildCommand(cwd, command);
        } else {
            DockerCommandBuilder builder;
            
            std::string port = getOption("port");

            // Set the basic parameters of the Docker command
            builder.setWorkingDirectory(cwd)
                .setDockerImage(getDockerImage())
                .setUserCommand(command)
                .setPort(port);

            // Build and return the full Docker command
            return builder.build();
        }
    }
};

// Register the PhpyDockerRunner when this file is included
static bool phpRunnerRegistered = (PhpDockerRunner::registerRunner(), true);

#endif // PHPDOCKERRUNNER_H
