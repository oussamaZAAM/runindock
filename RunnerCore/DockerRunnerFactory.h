#ifndef DOCKERRUNNERFACTORY_H
#define DOCKERRUNNERFACTORY_H

#include "DockerRunnerRegistry.h"
#include <memory>

// Factory function to choose the appropriate DockerRunner with optional custom image
std::unique_ptr<DockerRunner> getDockerRunner(const std::string& environment, const std::string& customImage = "", const std::string& port = "") {
    auto runner = DockerRunnerRegistry::getInstance().createRunner(environment, customImage);
    if (!port.empty()) {
        runner->setOption("port", port);  // Set the port if provided
    }
    return runner;
}

#endif // DOCKERRUNNERFACTORY_H
