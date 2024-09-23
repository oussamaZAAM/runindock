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

    virtual std::string getDefaultImage() const override {
        return "openjdk:alpine";
    }

    // Automatically register the runner for both "java" and "javac" environments
    static void registerRunner() {
        auto& registry = DockerRunnerRegistry::getInstance();
        registerForEnvironments(registry, {"java", "javac"});
    }

    // Register multiple environments for a runner
    static void registerForEnvironments(DockerRunnerRegistry& registry, const std::initializer_list<std::string>& environments) {
        for (const auto& env : environments) {
            registry.registerRunner(env, [](const std::string& image) -> std::unique_ptr<DockerRunner> {
                return std::make_unique<JavaDockerRunner>(image);
            });
        }
    }

    virtual std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        std::string tool = getOption("buildTool");
        if (tool == "maven"){
            MavenDockerRunner mavenRunner;
            return mavenRunner.buildCommand(cwd, command);
        } else if (tool == "gradle") {
            GradleDockerRunner gradleRunner;
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