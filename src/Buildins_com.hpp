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

// build in declarasions of build in commands
int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);

const char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

int (*builtin_func[])(char **) = {
    &sh_cd,
    &sh_help,
    &sh_exit};

int sh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int sh_cd(char **args)
{
    if (args[1] == NULL)
    {
        std::cerr << "sh: expected argument to \"cd\"\n";
    }
    else
    {
        // is the core of the cd command. It changes the current working directory
        //  If chdir is successful, it returns 0
        if (chdir(args[1]) != 0)
        {
            perror("sh"); // in caas of any errors from the chdir this will print out the error
            // for example if there is no directory present it will print no such directory
        }
    }
    return 1;
}

// help command
int sh_help(char **args)
{
    int i;
    std::cout << "Noob's shell\n";
    std::cout << "Type program names and arguments, and hit enter.\n";
    std::cout << "The following are built in:\n";

    for (i = 0; i < sh_num_builtins(); i++)
    {
        std::cout << builtin_str[i] << std::endl;
    }
    std::cout << "Use the man command for information on other programs.\n";

    return 1;
}

// ecit command
int sh_exit(char **args)
{
    return 0;
}
