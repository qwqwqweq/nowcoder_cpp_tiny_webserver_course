 #include <unistd.h>
 #include <stdio.h>

 int main() {
    int ret = truncate("b.txt", 20);

    if (ret == -1) {
        perror("truncate");
        return -1;
    }


    return 0;
 }