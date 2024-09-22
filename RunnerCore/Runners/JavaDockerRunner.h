#ifndef JAVADOCKERRUNNER_H
#define JAVADOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"
#include "../DockerCommandBuilder.h"

class JavaDockerRunner : public DockerRunner {
public:
    JavaDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "openjdk:alpine";
    }

    // Automatically register the runner for both "java" and "javac" environments
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for both "java" and "javac"
        for (const auto& env : {"java", "javac"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<JavaDockerRunner>(image);
            });
        }
    }

    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        std::cout << cwd << std::endl;
        DockerCommandBuilder builder;
        // Set the basic parameters of the Docker command
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .setPort(port);

        // Build and return the full Docker command
        return builder.build();
    }
};

// Register the JavaDockerRunner for both "java" and "javac" environments when this file is included
static bool javaRunnerRegistered = (JavaDockerRunner::registerRunner(), true);

#endif // JAVADOCKERRUNNER_H
