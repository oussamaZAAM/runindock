// main.cpp
#include "RunnerCore/DockerRunnerFactory.h"
#include "RunnerCore/Runners/GoDockerRunner.h"
#include "RunnerCore/Runners/NodeDockerRunner.h"
#include "RunnerCore/Runners/Java/JavaDockerRunner.h"
#include "RunnerCore/Runners/PythonDockerRunner.h"
#include "RunnerCore/Runners/PHP/PhpDockerRunner.h"
#include "RunnerCore/Runners/RustDockerRunner.h"
#include <string>
#include <unistd.h>
#include <memory>
#include <vector>

#define DOCKER_RUNNERS \
    X(GoDockerRunner) \
    X(NodeDockerRunner) \
    X(JavaDockerRunner) \
    X(PythonDockerRunner) \
    X(PhpDockerRunner) \
    X(RustDockerRunner)

namespace {
    #define X(runner) bool runner##Registered = (runner::registerRunner(), true);
    DOCKER_RUNNERS
    #undef X
}

// Function to parse all command-line options into a map, now using --rid:key=value format
std::map<std::string, std::string> parseOptions(int& argc, char* argv[]) {
    std::map<std::string, std::string> options;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Look for options in the form --rid:key=value
        if (arg.substr(0, 6) == "--rid:") {
            size_t pos = arg.find("=");
            if (pos != std::string::npos) {
                std::string key = arg.substr(6, pos - 6);   // Extract the key (without "--rid:")
                std::string value = arg.substr(pos + 1);    // Extract the value
                options[key] = value;

                // Remove this option from argv
                for (int j = i; j < argc - 1; ++j) {
                    argv[j] = argv[j + 1];
                }
                --argc;
                --i;
            }
        }
    }

    return options;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: runindock <environment> <command...> [--image=<docker_image>] [--port=<container_port>]" << std::endl;
        return 1;
    }

    // Parse all command-line options into a map
    std::map<std::string, std::string> options = parseOptions(argc, argv);

    // First argument is the environment (e.g., "go", "node", "java", "javac", ...)
    std::string environment = argv[1];

    // Collect all command-line arguments starting from the second one (i.e., argv[2]) into a single command string
    std::string command;
    for (int i = 1; i < argc; ++i) {  // Start from argv[2] to skip the environment
        command += std::string(argv[i]) + " ";
    }

    // Trim the trailing space from the command string
    if (!command.empty()) {
        command.pop_back();
    }

    // Get the appropriate DockerRunner based on the environment and custom image
    auto dockerRunner = getDockerRunner(environment);
    dockerRunner->setOptions(options);

    // Run the provided command inside Docker
    dockerRunner->run(command);

    return 0;
}
