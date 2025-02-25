#include <iostream>
#include <sys/shm.h>
#include <cstring>

int main() {
    //1 获取一个共享内存
    int shmid = shmget(100, 0, IPC_CREAT);
    std::cout << "shmid: " << shmid << std::endl;

    //2 和当前进程进行关联
    void *ptr = shmat(shmid, NULL, 0);

    //3 读数据
    std::cout << static_cast<char *>(ptr) << std::endl; 

    std::cout << "按任意键继续\n";
    getchar();

    //4 解除关联
    shmdt(ptr);

    //是否可以重新关联 (可以)
    shmid = shmget(100, 0, IPC_CREAT);
    ptr = shmat(shmid, NULL, 0);
    std::cout << static_cast<char *>(ptr) << std::endl; 

    //5 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}