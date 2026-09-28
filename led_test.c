#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define LED_DEV "/dev/led_dev"

static void usage(const char *prog)
{
    printf("Usage:\n");
    printf("  %s on      # 点亮 LED\n", prog);
    printf("  %s off     # 熄灭 LED\n", prog);
    printf("  %s blink N # 闪烁 N 次\n", prog);
    printf("  %s status  # 读取当前状态(需要驱动支持 read)\n", prog);
}

int main(int argc, char *argv[])
{
    int fd;
    char val;

    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }

    fd = open(LED_DEV, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open %s failed: %s\n", LED_DEV, strerror(errno));
        return -1;
    }

    if (strcmp(argv[1], "on") == 0) {
        val = 1;
        if (write(fd, &val, 1) != 1) {
            fprintf(stderr, "write failed: %s\n", strerror(errno));
            close(fd);
            return -1;
        }
        printf("LED ON\n");
    }
    else if (strcmp(argv[1], "off") == 0) {
        val = 0;
        if (write(fd, &val, 1) != 1) {
            fprintf(stderr, "write failed: %s\n", strerror(errno));
            close(fd);
            return -1;
        }
        printf("LED OFF\n");
    }
    else if (strcmp(argv[1], "blink") == 0) {
        int n = (argc >= 3) ? atoi(argv[2]) : 3;
        if (n <= 0) n = 3;
        for (int i = 0; i < n; i++) {
            val = 1;
            write(fd, &val, 1);
            usleep(300 * 1000);   /* 300ms */
            val = 0;
            write(fd, &val, 1);
            usleep(300 * 1000);
        }
        printf("Blink %d times done\n", n);
    }
    else {
        usage(argv[0]);
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}
