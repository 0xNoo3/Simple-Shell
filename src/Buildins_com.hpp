
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
    std::cout << "Muhammad Bilal shell\n";
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