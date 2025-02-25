// #include <fcntl.h>

// int open(const char *pathname, int flags, ...
// /* mode_t mode */ ); 

//  flags must include one of the following access modes: O_RDONLY, O_WRONLY, or O_RDWR.
// #include <stdio.h>
// void perror(const char *s);

// #include <errno.h>
// int errno;       /* Not really declared this way; see errno(3) */

//  #include <unistd.h>

//        int close(int fd);


#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {

    int fd = open("a.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
    }

    close(fd);

    return 0;
}