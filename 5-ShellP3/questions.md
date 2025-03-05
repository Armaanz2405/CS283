1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation ensures that all child processes complete before the shell continues accepting user input by calling waitpid on each forked process. This basically ensures that the shell waits for all child processes in a pipeline to finish executing before returning control to the user. If waitpid was not called on child processes, then zombie processes would accumulate because the shell wouldn't get their exit statuses leading to potential extra resource usage.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

The dup2 function is used to redirect input and output file descriptors so that processes in a pipeline can communicate correctly. It is necessary to close the unused pipe ends because leaving them open can cause issues such as processes waiting indefinitely for input that never arrives. If a  process keeps a write end of a pipe open while another process reads from it, the reader may never receive an EOF signal.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is implemented as a built in command because it modiifes the shells working directory, which is a property of the shell itself. If cd were implemented as an external process, it would only change the working directory of the child process executing execvp. Once the process terminates, the parent shell would remain in its original directory.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To support an arbitrary number of pipred commands, you can replace the fixed-size array used for storing pipeline commands with a dynamically allocated list such as a linked list or a resizable array. This would allow the shell to allocate memory as needed rather than being constrained by CMD_MAX. Some trade offs include increased memory management and potential performance issues.
