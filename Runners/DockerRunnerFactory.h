// DockerRunnerFactory.h
#ifndef DOCKERRUNNERFACTORY_H
#define DOCKERRUNNERFACTORY_H

#include "GoDockerRunner.h"
#include "NodeDockerRunner.h"
#include <memory>
#include <iostream>

// Factory function to choose the appropriate DockerRunner with optional custom image
std::unique_ptr<DockerRunner> getDockerRunner(const std::string& environment, const std::string& customImage = "") {
    if (environment == "go") {
        return std::make_unique<GoDockerRunner>(customImage);
    } else if (environment == "node") {
        return std::make_unique<NodeDockerRunner>(customImage);
    } else {
        std::cerr << "Unsupported environment: " << environment << std::endl;
        exit(1);
    }
}

#endif // DOCKERRUNNERFACTORY_H
