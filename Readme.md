# Simple Shell (C++)

Welcome to **Simple Shell**, a basic Unix-like shell built from scratch using **C++**. This shell mimics some common shell behaviors like executing commands, handling built-in commands (`cd`, `help`, `exit`), and more!

### 🚀 Features:

- **Basic Command Execution**: Execute system commands like `ls`, `pwd`, `echo`, etc.
- **🧑‍💻Built-in Commands**:
  - `cd`: Change the current directory.
  - `help`: Display helpful information about the shell.
  - `exit`: Exit the shell session.
- **Process Management**: The shell uses `fork()` to create child processes and `execvp()` to replace the child process with the given command.
- **Command Parsing**: Command input is parsed into arguments for execution.

### 📂 Files:

- `main.cpp`: The main program that runs the shell loop.
- `Buildins_com.hpp`: Contains implementations of built-in commands like `cd`, `help`, and `exit`.

### 🛠️ Compilation and Usage:

1. **Clone the repository**:

   ```bash
   git clone https://github.com/bilalaniq/Simple-Shell.git
   cd simple-shell
   ```

2. **Compile the shell** using a C++ compiler (e.g., `g++`):

   ```bash
   g++ main.cpp -o simple_shell
   ```

3. **Run the shell**:

   ```bash
   ./simple_shell
   ```

4. **Available Commands**:
   - `cd <directory>`: Change the current working directory.
   - `help`: Display help about available commands.
   - `exit`: Exit the shell.

### ⚙️ Code Explanation:

#### 1. **Main Shell Loop**:

The `shell_loop()` function repeatedly prompts the user for input, processes the input into commands and arguments, and executes them using `fork()` and `execvp()`.

```cpp
void shell_loop() {
    char *line;
    char **args;
    int status;
    do {
        std::cout << "> ";  // Prompt
        line = read_line();  // Read user input
        args = get_args(line);  // Parse the input into arguments
        status = sh_execute(args);  // Execute the command
        free(line);  // Free memory
        free(args);
    } while (status);  // Continue looping
}
```

#### 2. **Process Creation**:

The `sh_launch()` function creates a child process using `fork()`. The child process runs the command via `execvp()`, and the parent process waits for the child to finish using `waitpid()`.

```cpp
int sh_launch(char **args) {
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("sh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("sh");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
```

#### 3. **Built-in Commands**:

Built-in commands like `cd`, `exit`, and `help` are implemented directly in the shell because they need to affect the shell process itself. For example, `cd` needs to change the current directory of the shell process, not a child process.

```cpp
int sh_cd(char **args) {
    if (args[1] == NULL) {
        std::cerr << "sh: expected argument to \"cd\"\n";
    } else {
        if (chdir(args[1]) != 0) {
            perror("sh");
        }
    }
    return 1;
}
```

### 🛑 Known Limitations:

- Currently, only basic functionality for handling external commands is implemented.
- No support for background tasks or piping between commands yet.

### 📄 License:

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### 📝 Contributions:

Feel free to fork this project, create issues, or send pull requests with new features or bug fixes! 😊

---

🚀 **Enjoy using Simple Shell**! Happy coding! 😎