#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << argv[0] << " path\n";
        return -1;
    }

    //获取个数
    auto getFileNum = [&](auto &&self, const char *pth) -> int{
        //打开目录
        DIR *dir = opendir(pth);

        if (dir == NULL) {
            perror("opendir");
            exit(0);
        }

        //记录普通文件的个数
        int total = 0;

        struct dirent *ptr;
        while ((ptr = readdir(dir)) != NULL) {
            //获取名称
            char *dname = ptr->d_name;

            //忽略掉.和..
            if (strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0)
                continue;
            
            //判断文件是普通文件还是目录
            if (ptr->d_type == DT_DIR) {
                //目录，需要继续读取这个目录
                char newpath[256];
                sprintf(newpath, "%s/%s", pth, dname);
                total += self(self, newpath);
            }

            if (ptr->d_type == DT_REG)
                total++;
        }
        closedir(dir);
        return total;
    };

    int num = getFileNum(getFileNum, argv[1]);
    std::cout << "普通文件的个数: " << num << std::endl;

    return 0;
}