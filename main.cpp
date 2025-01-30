#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>

using namespace std;

int main(int argc, char *argv[]) {
    int option = 0;  // default option: execute the command ls -l and terminate normally

    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt == 'n') {
            option = atoi(optarg);
        }
    }

    pid_t pid = fork(); // Fork a new process

    if (pid < 0) {
        cout << "Fork failed" << endl;
        return 1;
    } else if (pid == 0) { // Child process
        cout << "Hello from the child process!" << endl;
        cout << "The parent process ID is " << getppid() << endl;

        if (option % 2 == 0) {
            cout << "The child process will execute the command: ls -l after 6 seconds" << endl;
            sleep(6); // Sleep for 6 seconds
            char *args[] = {(char *)"ls", (char *)"-l", NULL};
            execvp("ls", args); // Execute ls -l
            _exit(EXIT_SUCCESS); // Exit after execution to avoid further execution of code
        } else {
            cout << "The child process is exiting" << endl;
            kill(getpid(), SIGINT); // Send SIGINT to itself
        }
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child process to finish

        cout << "\nHello from the parent process!" << endl;
        cout << "The child process ID is " << pid << endl;

        if (WIFEXITED(status)) {
            cout << "The child process exited normally" << endl;
        } else if (WIFSIGNALED(status)) {
            cout << "The child process exited due to the kill signal" << endl;
        }
    }

    return 0;
}
