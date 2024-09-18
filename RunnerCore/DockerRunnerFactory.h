#ifndef DOCKERRUNNERFACTORY_H
#define DOCKERRUNNERFACTORY_H

#include "DockerRunnerRegistry.h"
#include <memory>

// Factory function to choose the appropriate DockerRunner with optional custom image
std::unique_ptr<DockerRunner> getDockerRunner(const std::string& environment, const std::string& customImage = "") {
    return DockerRunnerRegistry::getInstance().createRunner(environment, customImage);
}

#endif // DOCKERRUNNERFACTORY_H
