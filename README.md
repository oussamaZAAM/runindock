# runindock

**runindock** is a lightweight CLI tool that runs commands inside Docker containers, providing an easy way to execute simple language-specific tasks and web apps in isolated environments. It abstracts Docker commands, allowing you to run tools like Java, Go, Node.js, and more without directly using Docker commands.

## Features
- **Docker-based environments**: Execute commands inside isolated Docker containers without manually writing Docker commands.
- **Language support**: Easily run Go, Node.js, and more.
- **Custom Docker images**: Override default images with the `--rid:image` option.
- **Cross-platform support**: Works on Linux, macOS, and WSL.

## Installation

### Windows
1. Download the `runindock.exe` from the [Releases](https://github.com/runindock/releases) page.
2. Move the `.exe` to `C:\Windows\System32` or any folder in your `PATH`.  
   Alternatively, create a folder like `C:\runindock` and add it to your `PATH` by:
   - Right-click on **This PC** or **My Computer**, select **Properties**.
   - Click on **Advanced system settings** > **Environment Variables**.
   - Find the **Path** variable, select it, and click **Edit**.
   - Add `C:\runindock` and move your `.exe` there.
3. Now, you can run the executable globally by typing `runindock` in any command prompt or terminal.

### macOS / Linux
1. Download the `runindock` binary from the [Releases](https://github.com/runindock/releases) page.
2. Move it to `/usr/local/bin/`:
   - `sudo mv runindock /usr/local/bin/`
3. Make the binary executable:
   - `sudo chmod +x /usr/local/bin/runindock`
4. Run the program from any terminal by typing `runindock`.


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
By default, `runindock` uses lightweight Docker images like `golang:alpine` and `node:alpine`. You can override this by specifying a custom image with the `--rid:image` option.

#### Example:
```
runindock go run main.go --rid:image="golang:latest"
```

### Exposing a Port:
If your application needs to expose a specific port (e.g., when running a web server), you can use the `--rid:port` option to bind a port from the Docker container to your local machine.

#### Example:
```
runindock node app.js --rid:port=8080
```

In this example, the `--rid:port=8080` option will map the container's port 8080 to your local machine, allowing you to access the application at `localhost:8080`.

### Specifying a Build Tool:
For environments like Java, which use build tools like Maven or Gradle, you can specify the build tool with the `--rid:tool` option to ensure the correct environment is used inside the Docker container.

#### Example:
```
runindock java -jar app.jar --rid:tool=maven
```

In this example, the `--rid:tool=maven` option ensures that the application will be built and run using Maven inside the Docker container.

### General Syntax:
```
runindock <environment> <command> [--rid:image=<docker_image>] [--rid:port=<container_port>] [--rid:tool=<build_tool>] [--rid:framework=<framework>]
```

- `<environment>`: The environment or programming language (e.g., `go`, `node`).
- `<command>`: The command you want to run inside Docker (e.g., `run main.go`).
- `--rid:image`: Optional. Specify a custom Docker image.
- `--rid:port`: Optional. Expose a port from the Docker container.
- `--rid:tool`: Optional. Specify a build tool (e.g., `maven`, `gradle`) for environments that support it.
- `--rid:framework`: Optional. Specify a framework (e.g., `laravel`, `symfony`) for languages that support it.
- `rid`: Stands for (`runindock`), is used to separate between potential conflicts (like `--port` in case of **Laravel**).

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
You can run a Spring Boot application directly using Maven:
```
runindock mvn spring-boot:run --rid:port=8080
```
Or package the application with Maven and run the generated JAR:
```
runindock mvn clean package
runindock java -jar target/spring-app-1.0-SNAPSHOT.jar --rid:port=8080 --rid:tool=maven
```

### 3. Node.js:
#### a. Set up your project environment using `npm`:
```
runindock npm init
```
#### b. Install packages for your project:
```
runindock npm install express
```
#### c. Launch your server (`index.js`) on a specific port:
```
runindock node index.js --rid:port=8080
```

### 4. Rust:
#### a. Compiling and Running a Rust Program:
First, compile your Rust program:
```
runindock cargo build
```
Then, run the compiled binary:
```
runindock cargo run
```
#### b. Running Rust Tests:
To run tests in your Rust project:
```
runindock cargo test
```

### 5. PHP:
#### a. Running a PHP Script:
To execute a standalone PHP script:
```
runindock php script.php
```
#### b. Running a Symfony application:
After generating a project using:
```
runindock composer create-project symfony/skeleton my_project --rid:framework=symfony
```
You can run a simple PHP development server:
```
runindock php -S 0.0.0.0:8080 -t public/ --rid:port=8080
```
This serves files from the public/ directory.

#### c. Running a Laravel Application:
If you're working with a Laravel application, you can serve it using the built-in Laravel development server:

##### i. Generate a project using your preferred framework (e.g. Laravel):
```
runindock composer create-project laravel/laravel my-laravel-app --rid:framework=laravel
```
##### ii. Run the Laravel development server:
```
runindock php artisan serve --host=0.0.0.0 --port=8080 --rid:port=8080
```

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

