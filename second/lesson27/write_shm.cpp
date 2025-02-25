#include <iostream>
#include <sys/shm.h>
#include <cstring>

int main() {
    //1 创建一个共享内存
    int shmid = shmget(100, 4096, IPC_CREAT | 0664);
    std::cout << "shmid: " << shmid << std::endl;

    //2 和当前进程进行关联
    void *ptr = shmat(shmid, NULL, 0);

    //3 写数据
    char *str = "helloworld";
    memcpy(ptr, str, strlen(str) + 1);
    // std::cout << sizeof(str) << std::endl;
    // std::cout << static_cast<char *>(ptr) << std::endl;

    std::cout << "按任意键继续\n";
    getchar();

    //4 解除关联
    shmdt(ptr);

    //5 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}