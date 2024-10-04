#ifndef PERLDOCKERRUNNER_H
#define PERLDOCKERRUNNER_H

#include <memory>
#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class PerlDockerRunner : public DockerRunner {
public:
    PerlDockerRunner(const std::string& image = "") : DockerRunner() {}

    std::string getDefaultImage() const override {
        return "perl:slim";
    }

    // Automatically register the runner for the "perl" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Perl environments
        for (const auto& env : {"perl"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<PerlDockerRunner>(image);
            });
        }
    }
};

// Register the GoDockerRunner when this file is included
static bool perlRunnerRegistered = (PerlDockerRunner::registerRunner(), true);

#endif // PERLDOCKERRUNNER_H
