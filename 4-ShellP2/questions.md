1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use it to create a new child process before calling execvp so that the original parent process remains unchanged. The value of fork is that it allows the shell to remain active while the child process runs the external command independently.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork fails it usually returns -1. This is handled by checking the return value of fork and printing an error message if it does fail, and it also doesn't execute the following command.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It finds the command to execute by searching for the command in the directories listed in the PATH environment variable. If the command is found, it executes it. If it isn't found it returns an error and says the command wasn't found.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The call ensures that the parent process waits for the child process to complete before continuing execution. If we don't call it, it leads to the child process causing resource leaks because the exit status isn't collected.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It extracts the exit code of the child process from the status returned by wait.  It's important because it allows the parent process to determine whether the child process executed successfully or encountered an error.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It detects quoted arguments and treats them as a single token even if they contain spaces. This is for commands and stuff like echo "hello world" so they print properly with their spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: I had to improve the parsing logic to manage the quoted strings needed for the assignment. I think it was a bit messy to refactor the last assignment into this one I felt a bit lost at first so I would say that was my challenge.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals provide a way for processes to communicate and notifying them of specific events. They differ from other forms of IPC becvause signals are lightweight and do not require explicit data transfer between processes.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL immediately terminates a process, used typically when a process must be forcefully stopped. SIGTERM requests a process to terminate, but not with brute force like sigkill so it allows it to clean up before exiting. SIGINT interrupts a process and its used by ctrl+c in the terminal.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a processs receives SIGSTOP it is paused until it receives a SIGCONT signal. Unlike SIGINT, SIGSTOP cannot be caught or ignored because it is handled directly by the kernel, ensuring that a process can always be suspended.
