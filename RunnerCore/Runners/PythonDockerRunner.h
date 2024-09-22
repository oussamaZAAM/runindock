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
        auto& registry = DockerRunnerRegistry::getInstance();
        // Register for multiple Python environments
        for (const auto& env : {"python", "pip", "python3", "pip3", "django-admin"}) {
            registry.registerRunner(env, [](const std::string& image) {
                return std::make_unique<PythonDockerRunner>(image);
            });
        }
    }

    std::string buildCommand(const std::string& cwd, const std::string& command) const override {
        DockerCommandBuilder builder;

        // Get the site-packages path dynamically by running a Python command
        std::string sitePackagesPath = getSitePackagesPath();

        // Set the basic Docker command parameters
        builder.setWorkingDirectory(cwd)
               .setDockerImage(getDockerImage())
               .setUserCommand(command)
               .setPort(port);

        // Add the volume mount for site-packages if available
        if (!sitePackagesPath.empty()) {
            builder.addVolume("python_shared_vol:" + sitePackagesPath);
        }

        // Build and return the full Docker command
        return builder.build();
    }


private:
    // Helper function to execute a command to get the site-packages path
    std::string getSitePackagesPath() const {
        std::string getSitePackagesCommand = "docker run --rm " + getDefaultImage() +
                                             " python -c \"import site; print(site.getsitepackages()[0])\"";
        return executeCommand(getSitePackagesCommand);
    }

    // Helper function to execute a shell command and return its output
    std::string executeCommand(const std::string& command) const {
        std::array<char, 128> buffer;
        std::string result;
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run command: " << command << std::endl;
            return result;
        }

        // Read the command's output
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        // Close the pipe
        pclose(pipe);

        // Remove trailing newline if present
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return result;
    }
};

// Register the PythonyDockerRunner when this file is included
static bool pythonRunnerRegistered = (PythonDockerRunner::registerRunner(), true);

#endif // PYTHONDOCKERRUNNER_H
