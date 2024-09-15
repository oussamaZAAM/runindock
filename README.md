# runindock

**runindock** is a lightweight CLI tool that runs commands inside Docker containers, providing an easy way to execute simple language-specific tasks in isolated environments. It abstracts Docker commands, allowing you to run tools like Go, Node.js, and more without directly using Docker commands.

## Features
- **Docker-based environments**: Execute commands inside isolated Docker containers without manually writing Docker commands.
- **Language support**: Easily run Go, Node.js, and more.
- **Custom Docker images**: Override default images with the `--image` option.
- **Cross-platform support**: Works on Linux, macOS, and WSL.

## Installation

### Linux / macOS / WSL
1. Clone the repository:
   ```
   git clone https://github.com/oussamaZAAM/runindock.git
   cd runindock
   ```
2. Run the installation script to make `runindock` globally available:
   ```
   ./install.sh
   ```
3. Now you can use the `runindock` command globally:
   ```
   runindock go run main.go
   ```

### Windows
For native Windows users, you can manually compile the project or run it inside **WSL**.

## Usage

Once installed, you can use `runindock` to execute commands inside Docker containers. Here are a few examples:

### Running Go Code:
```
runindock go run main.go
```

### Running Node.js Code:
```
runindock node app.js
```

### Overriding the Docker Image:
By default, `runindock` uses lightweight Docker images like `golang:alpine` and `node:alpine`. You can override this by specifying a custom image with the `--image` option.

Example:
```
runindock go run main.go --image="golang:latest"
```

### General Syntax:
```
runindock <environment> <command> [--image=<docker_image>]
```

- `<environment>`: The environment or programming language (e.g., `go`, `node`).
- `<command>`: The command you want to run inside Docker (e.g., `run main.go`).
- `--image`: Optional. Specify a custom Docker image.

## Supported Languages
- Go
- Node.js
- More languages can be added easily by extending the tool.

## Contributing

Contributions are welcome! If you'd like to add support for more languages or features, follow these steps:

1. Fork the project.
2. Create a new branch for your feature.
3. Make your changes and ensure all tests pass.
4. Submit a pull request, explaining your changes.

### Running Tests
Make sure to test your changes before submitting:
```
# Compile and test the project locally
g++ main.cpp -o runindock
./runindock go run main.go
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

Special thanks to the open-source community for the inspiration and support!
