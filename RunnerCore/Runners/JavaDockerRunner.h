#ifndef JAVADOCKERRUNNER_H
#define JAVADOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class JavaDockerRunner : public DockerRunner {
public:
    JavaDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "openjdk:alpine";
    }

    // Automatically register the runner for both "java" and "javac" environments
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("java", [](const std::string& image) {
            return std::make_unique<JavaDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("javac", [](const std::string& image) {
            return std::make_unique<JavaDockerRunner>(image);
        });
    }
};

// Register the JavaDockerRunner for both "java" and "javac" environments when this file is included
static bool javaRunnerRegistered = (JavaDockerRunner::registerRunner(), true);

#endif // JAVADOCKERRUNNER_H
