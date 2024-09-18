#ifndef DOCKERRUNNERREGISTRY_H
#define DOCKERRUNNERREGISTRY_H

#include "DockerRunner.h"
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

// Registry for DockerRunner types
class DockerRunnerRegistry {
public:
    using DockerRunnerCreator = std::function<std::unique_ptr<DockerRunner>(const std::string&)>;

    // Singleton instance of the registry
    static DockerRunnerRegistry& getInstance() {
        static DockerRunnerRegistry instance;
        return instance;
    }

    // Register a new DockerRunner type
    void registerRunner(const std::string& environment, DockerRunnerCreator creator) {
        registry[environment] = std::move(creator);
    }

    // Create a DockerRunner for the given environment
    std::unique_ptr<DockerRunner> createRunner(const std::string& environment, const std::string& customImage = "") {
        if (registry.find(environment) != registry.end()) {
            return registry[environment](customImage);
        } else {
            std::cerr << "Unsupported environment: " << environment << std::endl;
            exit(1);
        }
    }

private:
    DockerRunnerRegistry() = default;

    // Map of environment names to their DockerRunner creators
    std::map<std::string, DockerRunnerCreator> registry;
};

#endif // DOCKERRUNNERREGISTRY_H
