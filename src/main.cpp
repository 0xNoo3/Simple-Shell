#include <iostream>
#include <unistd.h> // For chdir(), fork(), execvp() ,pid_t
#include <sys/wait.h> // For waitpid() and associated macros
#include <cstdio>     // For getchar(), perror()
#include <cstdlib>    //For exit(), malloc(), realloc(), and free(), EXIT_SUCCESS, EXIT_FAILURE
#include <cstring> //strcmp(), strtok()

#include"Buildins_com.hpp"






#define SH_RL_BUFSIZE 1024
char *read_line()
{
    int bufferSize = SH_RL_BUFSIZE;
    size_t position = 0;
    char *buffer = (char *)malloc(sizeof(char) * bufferSize);
    int c;

    if (!buffer)
    {
        std::cerr << "sh: allocation error\n";
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        c = getchar();

        // When the user signals the end of input in a terminal
        // (e.g., pressing Ctrl-D in UNIX-like systems, or Ctrl-Z in Windows).
        if (c == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if (c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;

        if (position >= bufferSize)
        {
            bufferSize += SH_RL_BUFSIZE;
            buffer = (char *)realloc(buffer, sizeof(char) * bufferSize);
            if (!buffer)
            {
                std::cerr << "sh: allocation error\n";
                exit(EXIT_FAILURE);
            }
        }
    }
}

// for parsing the line into arguments

#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a"

// delimiters (often abbreviated as delim) are characters or sequences of characters used to separate or mark the boundaries
// between different parts of a string.

char **get_args(char *line)
{
    int buffersize = SH_TOK_BUFSIZE;
    size_t position = 0;
    char **tokens = (char **)malloc(sizeof(char *) * buffersize);
    char *token, **temp_tokens;

    if (!token)
    {
        std::cerr << "sh: allocation error\n";
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffersize)
        {
            buffersize += SH_TOK_BUFSIZE;
            temp_tokens = tokens;
            tokens = (char **)realloc(tokens, sizeof(char) * buffersize);
            if (!tokens)
            {
                free(line);
                free(temp_tokens);
                std::cerr << "sh: allocation error\n";
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int sh_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    // The return value of fork() is:
    // 0 for the child process.
    // > 0 (the child's PID) for the parent process.
    // < 0 in case of an error (e.g., if the system can't create a new process).
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1) // If the execvp() call is successful, the rest of the code will not run in the child process
        {
            perror("sh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("sh");
    }
    else
    {
        // Parent process
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// is part of a simple shell implementation. It processes the arguments passed to it
// (such as the command and its arguments) and determines whether the command is a built-in command or should be executed as
// an external program

/*
How shells start processes
Now, we’re really at the heart of what a shell does. Starting processes is the main function of shells. So writing a shell means that
you need to know exactly what’s going on with processes and how they start. That’s why I’m going to take us on a short diversion to
discuss processes in Unix-like operating systems.

There are only two ways of starting processes on Unix. The first one (which almost doesn’t count) is by being Init. You see, when a
Unix computer boots, its kernel is loaded. Once it is loaded and initialized, the kernel starts only one process, which is called Init.
This process runs for the entire length of time that the computer is on, and it manages loading up the rest of the processes that you
need for your computer to be useful.

Since most programs aren’t Init, that leaves only one practical way for processes to get started: the fork() system call. When this
function is called, the operating system makes a duplicate of the process and starts them both running. The original process is called
the “parent”, and the new one is called the “child”. fork() returns 0 to the child process, and it returns to the parent the process
ID number (PID) of its child. In essence, this means that the only way for new processes is to start is by an existing one duplicating
itself.

This might sound like a problem. Typically, when you want to run a new process, you don’t just want another copy of the same
program – you want to run a different program. That’s what the exec() system call is all about. It replaces the current running
program with an entirely new one. This means that when you call exec, the operating system stops your process, loads up the new
program, and starts that one in its place. A process never returns from an exec() call (unless there’s an error).

With these two system calls, we have the building blocks for how most programs are run on Unix. First, an existing process forks itself
into two separate ones. Then, the child uses exec() to replace itself with a new program. The parent process can continue doing other
things, and it can even keep tabs on its children, using the system call wait().

Let’s say you run the command ls in your shell:

The shell uses fork() to create a child process.
The child process calls exec() to replace itself with the ls command.
The ls command lists the contents of the directory.
After the ls command finishes, the parent shell can wait for the child to finish using wait(), and then it can return to the prompt.

Phew! That’s a lot of information, but with all that background, the following code for launching a program will actually make sense:
*/

// function is designed to handle the launching of external commands in a shell
int sh_execute(char **args)
{
    int i;

    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < sh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return sh_launch(args);
}

void shell_loop()
{
    char *line;
    char **args;
    int status;

    do
    {
        std::cout << "> ";
        line = read_line();
        args = get_args(line);
        status = sh_execute(args);

        free(line);
        free(args);

    } while (status);
}

int main(int argc, char **args)
{
    shell_loop();
}

// -------------------------------------shell buildins--------------------------------------
/*
You may have noticed that the lsh_loop() function calls lsh_execute(), but above, we titled our function lsh_launch(). This
was intentional! You see, most commands a shell executes are programs, but not all of them. Some of them are built right into the shell.

-------------why cd must be an shell buildins
The reason is actually pretty simple. If you want to change directory, you need to use the function chdir(). The thing is, the
current directory is a property of a process. So, if you wrote a program called cd that changed directory, it would just change
its own current directory, and then terminate. Its parent process’s current directory would be unchanged. Instead, the shell process
itself needs to execute chdir(), so that its own current directory is updated. Then, when it launches child processes, they will
inherit that directory too.


Similarly, if there was a program named exit, it wouldn’t be able to exit the shell that called it. That command also needs to
be built into the shell. Also, most shells are configured by running configuration scripts, like ~/.bashrc. Those scripts use
commands that change the operation of the shell. These commands could only change the shell’s operation if they were implemented
within the shell process itself.

So, it makes sense that we need to add some commands to the shell itself. The ones I added to my shell are cd, exit, and help.


*/