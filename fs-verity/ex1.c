#include "fsverity.h"
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

void enable_fs_verity(int fd)
{
    int r;
    struct fsverity_enable_arg arg;

    memset(&arg, 0, sizeof(arg));

    arg.version = 1;
    arg.hash_algorithm = FS_VERITY_HASH_ALG_SHA256;
    arg.block_size = 4096;
    arg.salt_size = 0;
    arg.salt_ptr = NULL;
    arg.sig_size = 0;
    arg.sig_ptr = NULL;

    r = ioctl(fd, FS_IOC_ENABLE_VERITY, &arg);
    if (r) {
        printf("ioctl error: %d, %s\n", errno, strerror(errno));
    }
}

int main(int argc, char **argv)
{
    char buf[64] = {0,};
    char new_data[16] = "yyyyy";
    int r;

    // 0. intial write
    system("echo aaaaa > hello.txt");

    int fd = open("hello.txt", O_RDONLY);
    if (fd < 0) {
        printf("open error: %d, %s\n", errno, strerror(errno));
        return 0;
    }

    // 1. enable fsverity
    enable_fs_verity(fd);

    // 2. read file
    r = read(fd, buf, 2);
    if (r < 0) {
        printf("1st read fail: %d, %s\n", r, strerror(errno));
        goto out;
    }
    printf("read: %s\n", buf);
    memset(buf, 0, sizeof(buf));
    close(fd);

    // 3. corrupt file
    system("echo yyyyy > hello.txt");

    // 4. open again
    fd = open("hello.txt", O_RDONLY);
    if (fd < 0) {
        printf("open error: %d, %s\n", errno, strerror(errno));
        return 0;
    }

    // 5. read file again, check it will fail as supposed
    r = read(fd, buf, 2);
    if (r < 0) {
        printf("2nd read fail: %d, %s\n", r, strerror(errno));
        goto out;
    }
    printf("read: %s\n", buf);

out:
    close(fd);
    return 0;
}
