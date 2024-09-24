#ifndef JAVADOCKERRUNNER_H
#define JAVADOCKERRUNNER_H

#include <memory> // Correctly include the standard memory header
#include "../../DockerRunner.h"
#include "../../DockerRunnerRegistry.h"
#include "../../DockerCommandBuilder.h"

#include "./MavenDockerRunner.h"
#include "./GradleDockerRunner.h"

class JavaDockerRunner : public DockerRunner {
public:
    JavaDockerRunner(const std::string& image = "") : DockerRunner() {
        if (!image.empty()) {
            setOption("image", image);
        }
    }

    std::string getDefaultImage() const override {
        return "openjdk:alpine";
    }

    // Automatically register the runner for both "java" and "javac" environments
    // Automatically register the runner for the "python" environment
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Python environments
        for (const auto& env : {"java", "javac"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<JavaDockerRunner>(image);
            });
        }
    }

    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        std::string tool = getOption("tool");
        std::map<std::string, std::string> options = getOptions();
        if (tool == "maven"){
            MavenDockerRunner mavenRunner;
            mavenRunner.setOptions(options);
            return mavenRunner.buildCommand(cwd, command);
        } else if (tool == "gradle") {
            GradleDockerRunner gradleRunner;
            gradleRunner.setOptions(options);
            return gradleRunner.buildCommand(cwd, command);
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

// Register the JavaDockerRunner for both "java" and "javac" environments when this file is included
static bool javaRunnerRegistered = (JavaDockerRunner::registerRunner(), true);

#endif // JAVADOCKERRUNNER_H