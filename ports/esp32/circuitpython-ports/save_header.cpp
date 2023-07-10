#include <stdio.h>
#include "sine.h"  // 包含刚刚保存的.h文件

int main() {
    FILE *file;
    file = fopen("output.mp3", "wb");  // 打开一个二进制文件以写入
    if (file == NULL) {
        printf("无法打开文件。");
        return 1;
    }

    // 写入字节数组到文件
    fwrite(sine_mp3, sizeof(sine_mp3[0]), sizeof(sine_mp3) / sizeof(sine_mp3[0]), file);

    fclose(file);  // 关闭文件
    return 0;
}
