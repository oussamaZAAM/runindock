#ifndef RUBYDOCKERRUNNER_H
#define RUBYDOCKERRUNNER_H

#include <memory>
#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class RubyDockerRunner : public DockerRunner {
public:
    RubyDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "ruby";
    }

    // Automatically register the runner for the "ruby" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Ruby environments
        for (const auto& env : {"ruby", "rails", "gem"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<RubyDockerRunner>(image);
            });
        }
    }

    // Overriding the constructCommand function to handle Ruby commands
    std::string buildCommand(const std::string& cwd, const std::string &command) const override {
        DockerCommandBuilder builder;

        // Set the basic parameters of the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .setPort(getOption("port"))
               .addVolume("ruby_shared_vol:/usr/local/bundle");

        // Build and return the full Docker command
        return builder.build();
    }
};

// Register the GoDockerRunner when this file is included
static bool rubyRunnerRegistered = (RubyDockerRunner::registerRunner(), true);

#endif // RUBYDOCKERRUNNER_H
