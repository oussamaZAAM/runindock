// DockerRunnerRegistry.h
#ifndef DOCKERRUNNERREGISTRY_H
#define DOCKERRUNNERREGISTRY_H

#include "DockerRunner.h"
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

class DockerRunnerRegistry {
public:
    using DockerRunnerCreator = std::function<std::unique_ptr<DockerRunner>(const std::string&)>;

    // Singleton instance
    static DockerRunnerRegistry& getInstance() {
        static DockerRunnerRegistry instance;
        return instance;
    }

    // Register a new DockerRunner type
    void registerRunner(const std::string& environment, DockerRunnerCreator creator) {
        registry[environment] = std::move(creator);
    }

    // Create a DockerRunner for the given environment
    std::unique_ptr<DockerRunner> createRunner(const std::string& environment, const std::string& customImage = "") const {
        auto it = registry.find(environment);
        if (it != registry.end()) {
            return it->second(customImage);
        } else {
            std::cerr << "Unsupported environment: " << environment << std::endl;
            return nullptr;
        }
    }

private:
    DockerRunnerRegistry() = default;

    // Disable copy and assignment
    DockerRunnerRegistry(const DockerRunnerRegistry&) = delete;
    DockerRunnerRegistry& operator=(const DockerRunnerRegistry&) = delete;

    // Map of environment names to their DockerRunner creators
    std::map<std::string, DockerRunnerCreator> registry;
};

#endif // DOCKERRUNNERREGISTRY_H
