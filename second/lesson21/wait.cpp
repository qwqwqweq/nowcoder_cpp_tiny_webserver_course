//  #include <sys/wait.h>

//        pid_t wait(int *_Nullable wstatus);

#include <sys/wait.h>
#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid;

    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (!pid)
            break;
        //产生5个子进程（兄弟关系），若pid == 0则跳出循环，防止后续的子进程循环创建自己的子进程
    }

    if (pid > 0) {
        while (true) {
            std::cout << "parent, pid = " << getpid() << '\n';

            // int ret = wait(NULL);
            int st;
            int ret = wait(&st);
            if (ret == -1)
                break;
            if (WIFEXITED(st)) {
                //是不是正常退出
                std::cout << "退出的状态码: " << WEXITSTATUS(st) << '\n';
            }
            if (WIFSIGNALED(st)) {
                //是不是异常终止
                std::cout << "被哪个信号干掉了: " << WTERMSIG(st) << '\n';
            }

            std::cout << "child die, pid = " << ret << '\n';

            sleep(1);
        }
    } else {
        while (true) {
            std::cout << "child, pid = " << getpid() << '\n';
            sleep(1);
            
        }
        exit(0);
    }

    return 0;
}