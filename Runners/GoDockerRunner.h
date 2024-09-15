// GoDockerRunner.h
#ifndef GODOCKERRUNNER_H
#define GODOCKERRUNNER_H

#include "DockerRunner.h"

class GoDockerRunner : public DockerRunner {
public:
    GoDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "golang:alpine";  // Default to Alpine
    }
};

#endif // GODOCKERRUNNER_H
