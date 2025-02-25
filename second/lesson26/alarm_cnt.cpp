//1s电脑能数多少个数
#include <unistd.h>
#include <cstdio>

// 实际的时间 = 内核时间 + 用户时间 + 消耗的时间 

int main() {
    alarm(1);

    int i = 0;
    while(true) {
        printf("%d\n", i++);
    }

    return 0;
}