// main.cpp
#include "Runners/DockerRunnerFactory.h"
#include <string>
#include <unistd.h>
#include <memory>
#include <vector>

std::string parseImageOption(int& argc, char* argv[]) {
    std::string image = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--image=") == 0) {
            image = arg.substr(8);  // Extract the image after "--image="
            // Remove the --image option from the argument list
            for (int j = i; j < argc - 1; ++j) {
                argv[j] = argv[j + 1];
            }
            --argc;
            break;
        }
    }
    return image;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: devenv <environment> <command...> [--image=<docker_image>]" << std::endl;
        return 1;
    }

    // Parse the custom --image option
    std::string customImage = parseImageOption(argc, argv);

    // First argument is the environment (e.g., "go", "node")
    std::string environment = argv[1];

    // Collect all command-line arguments starting from the second one into a single command string
    std::string command;
    for (int i = 1; i < argc; ++i) {
        command += std::string(argv[i]) + " ";
    }

    // Get the appropriate DockerRunner based on the environment and custom image
    auto dockerRunner = getDockerRunner(environment, customImage);

    // Run the provided command inside Docker
    dockerRunner->run(command);

    return 0;
}
