// NodeDockerRunner.h
#ifndef NODEDOCKERRUNNER_H
#define NODEDOCKERRUNNER_H

#include "DockerRunner.h"

class NodeDockerRunner : public DockerRunner {
public:
    NodeDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "node:slim";  // Default to Alpine
    }
};

#endif // NODEDOCKERRUNNER_H
