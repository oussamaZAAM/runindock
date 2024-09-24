# runindock

**runindock** is a lightweight CLI tool that runs commands inside Docker containers, providing an easy way to execute simple language-specific tasks and web apps in isolated environments. It abstracts Docker commands, allowing you to run tools like Java, Go, Node.js, and more without directly using Docker commands.

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

#### Example:
```
runindock go run main.go --image="golang:latest"
```

### Exposing a Port:
If your application needs to expose a specific port (e.g., when running a web server), you can use the `--port` option to bind a port from the Docker container to your local machine.

#### Example:
```
runindock node app.js --port=8080
```

In this example, the `--port=8080` option will map the container's port 8080 to your local machine, allowing you to access the application at `localhost:8080`.

### Specifying a Build Tool:
For environments like Java, which use build tools like Maven or Gradle, you can specify the build tool with the `--buildTool` option to ensure the correct environment is used inside the Docker container.

#### Example:
```
runindock java -jar app.jar --buildTool=maven
```

In this example, the `--buildTool=maven` option ensures that the application will be built and run using Maven inside the Docker container.

### General Syntax:
```
runindock <environment> <command> [--image=<docker_image>] [--port=<container_port>] [--buildTool=<build_tool>]
```

- `<environment>`: The environment or programming language (e.g., `go`, `node`).
- `<command>`: The command you want to run inside Docker (e.g., `run main.go`).
- `--image`: Optional. Specify a custom Docker image.
- `--port`: Optional. Expose a port from the Docker container.
- `--buildTool`: Optional. Specify a build tool (e.g., `maven`, `gradle`) for environments that support it.

## Supported Languages and Environments

- **Go**: Run Go applications using the `go` environment.
- **Node.js**: Run Node.js applications or servers (e.g., Express) using the `node` environment.
- **Java**: Run Java applications or web apps/servers (e.g., Spring Boot) using the `java` environment.
- **Python**: Run Python scripts or web applications (e.g., Django) using the `python` environment.
- **Rust**: Run Rust applications using the `rust` environment.
- **PHP**: Run PHP scripts and applications using the `php` environment.

More languages can be added by extending the tool.

## Example Usage

### 1. Python
#### a. Running a Python Script:
```
runindock python script.py
```

#### b. Running a web application (Django):
Start by installing Django:
```
runindock pip install django
```
Then create a django project using:
```
runindock python -m django startproject myproject
```
Finally, run the application:
```
runindock python manage.py runserver
```

### 2. Java

#### a. Compile a Java Class
Start by compiling the java code using `javac`:
```
javac HelloWorld.java
```
Then you can now, run the compiled byte-code:
```
java HelloWorld.class
```

#### b. Running a Java Application:
You can run a Spring application directly using `mvn`:
```
runindock mvn spring-boot:run --port=8080
```
Or you can package it using Maven then run it with `java`
```
runindock mvn clean package
runindock java -jar spring-app-1.0-SNAPSHOT.jar --port=8080 --buildTool=maven
```

### 3. Node.js:
#### a. You can set up your project environment using `npm`:
```
runindock npm init
```
#### b. You can also install packages for your project:
```
runindock npm install express
```
#### c. Finally you can launch your server (`index.js`) in a chosen port:
```
runindock node index.js --port=8080
```

### 4. Rust:


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

