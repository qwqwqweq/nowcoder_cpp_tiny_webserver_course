//-rw-r--r-- 1 zhangzs zhangzs 13 Nov 30 17:34 a.txt
#include <sys/stat.h>
#include <cstdio>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <cstring>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << argv[0] << " filename\n";
        return -1;
    }

    struct stat st;
    int ret = stat(argv[1], &st);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    //获取文件类型和权限
    char perms[11] = {0};

    switch(st.st_mode & S_IFMT) {
        case S_IFLNK: 
            perms[0] = 'l';
            break;
        case S_IFDIR: 
            perms[0] = 'd';
            break;
        case S_IFREG: 
            perms[0] = '-';
            break;
        case S_IFBLK: 
            perms[0] = 'b';
            break;
        case S_IFCHR: 
            perms[0] = 'c';
            break;
        case S_IFSOCK: 
            perms[0] = 's';
            break;
        case S_IFIFO: 
            perms[0] = 'p';
            break;
        default:
            perms[0] = '?';
            break;
    }

    //判断文件的访问权限
    //文件所有者
    perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-1';
    perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-1';
    perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-1';

    //文件所在组
    perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-1';
    perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-1';
    perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-1';

    //其他人
    perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-1';
    perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-1';
    perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-1';

    //硬链接数
    int linkNum = st.st_nlink;

    //文件所有者
    char *fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组
    char *fileGrp = getgrgid(st.st_gid)->gr_name;

    //文件大小
    long int fileSize = st.st_size;

    //获取修改的时间
    char *time = ctime(&st.st_mtime);

    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1);

    char buf[1024];
    sprintf(buf, "%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);

    std::cout << buf << '\n';

    return 0;
}