#ifndef RUSTDOCKERRUNNER_H
#define RUSTDOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class RustDockerRunner : public DockerRunner {
public:
    RustDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "rust:alpine";
    }

    // Automatically register the runner for the "rust" environment
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("cargo", [](const std::string& image) {
            return std::make_unique<RustDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("rustc", [](const std::string& image) {
            return std::make_unique<RustDockerRunner>(image);
        });
    }

     // Overriding the constructCommand function to handle Rust commands
    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        DockerCommandBuilder builder;

        // Set the basic parameters for the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setPort(port)
               .addPreRunCommand("apk add --no-cache musl-dev build-base");

        // Construct command based on whether it uses rustc or cargo
        if (command.find("rustc") == 0) {
            std::string file = command.substr(6);  // Extract file name after "rustc "
            builder.setUserCommand("rustc " + file);
        } else if (command.find("cargo") == 0) {
            builder.setUserCommand(command);       // Pass the cargo command as is
        } else {
            std::cerr << "Unsupported command for RustDockerRunner: " << command << std::endl;
            return "";
        }

        return builder.build();
    }
};

// Register the RustDockerRunner when this file is included
static bool rustRunnerRegistered = (RustDockerRunner::registerRunner(), true);

#endif // RUSTDOCKERRUNNER_H
