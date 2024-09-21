#ifndef PYTHONDOCKERRUNNER_H
#define PYTHONDOCKERRUNNER_H

#include "../DockerRunner.h"
#include "../DockerRunnerRegistry.h"

class PythonDockerRunner : public DockerRunner {
public:
    PythonDockerRunner(const std::string& image = "") : DockerRunner(image) {}

    std::string getDefaultImage() const override {
        return "python:alpine";
    }

    // Automatically register the runner for the "python" environment
    static void registerRunner() {
        DockerRunnerRegistry::getInstance().registerRunner("python", [](const std::string& image) {
            return std::make_unique<PythonDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("pip", [](const std::string& image) {
            return std::make_unique<PythonDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("python3", [](const std::string& image) {
            return std::make_unique<PythonDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("pip3", [](const std::string& image) {
            return std::make_unique<PythonDockerRunner>(image);
        });
        DockerRunnerRegistry::getInstance().registerRunner("django-admin", [](const std::string& image) {
            return std::make_unique<PythonDockerRunner>(image);
        });
    }

    // Override the preRunHook to update the .env file before running the command
    void preRunHook(std::string& dockerCommand) const override {
        // Execute the Python command to get site-packages path
        std::string getSitePackagesCommand = "docker run --rm " + getDefaultImage() + " python -c \"import site; print(site.getsitepackages()[0])\"";

        // Buffer to store the command output
        char buffer[128];
        std::string sitePackagesPath;

        // Execute the command using popen
        FILE* pipe = popen(getSitePackagesCommand.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run command." << std::endl;
            return;
        }

        // Read the output from the command
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            sitePackagesPath += buffer;
        }

        // Remove trailing newline from sitePackagesPath if present
        if (!sitePackagesPath.empty() && sitePackagesPath.back() == '\n') {
            sitePackagesPath.pop_back();
        }

        // Close the pipe
        pclose(pipe);

        // Append the dynamic site-packages path to the dockerCommand
        dockerCommand += " -v python_shared_vol:" + sitePackagesPath + " ";

        std::cout << "Mounting Python's shared volume..." << std::endl;
    }
};

// Register the PythonyDockerRunner when this file is included
static bool pythonRunnerRegistered = (PythonDockerRunner::registerRunner(), true);

#endif // PYTHONDOCKERRUNNER_H
