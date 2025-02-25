#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

//int kill(pid_t pid, int sig);
//int raise(int sig);
//void abort(void); 

int main() {

    pid_t pid = fork();

    if (!pid) {
        for (int i = 0; i < 5; i++) {
            std::cout << "child process\n";
            sleep(1);
        }
    } else {
        std::cout << "parent process\n";
        sleep(2);
        std::cout << "kill child process\n";
        kill(pid, SIGINT);
    }

    return 0;
}