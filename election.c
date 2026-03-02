#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulate_elecction(const char* council_name, int num_menbers) {
    srand(time(NULL)); // 设置随机种子
    printf("%s 选举开始，选出 %d 人\n", council_name, num_menbers);
    int* member = (int*) malloc(num_menbers * sizeof(int)); // C模式申请内存
    if (!member) {
        fprintf(stderr, "内存申请失败！\n");
        return;
    }
    for (int i = 0; i < num_menbers; i++) {
        member[i] = rand() % 100; // 模拟得票
    }

    // C模式清理内存
    free(member);
    printf("内存清理完毕！\n");
}
