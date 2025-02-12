1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is ideal here for the shell because it reads the input line by line and stops either at a newline or when the specified buffer size is reached. This makes it well suited for command line a pplications where commands are entered one line at a time. fgets() also null-terminates the string, reducing the risk of buffer overflow issues. It's just reliable for interactive use and automated testing in general.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Using malloc to allocate memmory for cmd_buff provides flexibility by allowing the buffer size to be determined dynamically at runtime. This is helpful for supporting variable length input or simply adjusting the buffer size without recompiling the code.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming spaces is necessary to ensure that the commands that are processed and their arguments are clean and standardized. If these spaces weren't trimmed, the shell might misinterpret the command.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  One redirection example is < which is input redirection. Example: (sort < input.txt). One challenge is that the shell must open the file snd replace the standard input of the sort command. Another redirection example is > which is output redirection. Example: (ls > output.txt). One challenge is that the shell needs to create the file and then ensure that standard out for ls writes to that file. One last redirection example is the append redirection which is >>. Example: (echo "add new line" >> append.txt). This is harder than the others because the line needs to be appended rather than overwriting existing content.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection involves directing the standard streams, such as STDIN STDOUT STDERR, of a command to or from a file. On the other hand, piping connects the STDOUT of one command directly to the STDIN of another command.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping STDOUT and STDERR separate is important because it allows users and programs to handle normal output and error messages differently. For example, redirecting and logging all errors to a separate file which helps with debugging.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  When a command failes, we probably want to capture the exit code and display any error messages sent to STDERR in the shell. If a command produces both STDOUT and STDERR we can provide a way to merge them by using a redirection operator from the previous questions.
