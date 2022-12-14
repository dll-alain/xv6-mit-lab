//
// Created by Alain Dong on 12/13/22.
//
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

/**
 * @brief 读取左邻居的第一个数据
 * @param lpipe 左邻居的管道符
 * @param first 用于存储第一个数据的地址
 * @return 如果没有数据返回-1,有数据返回0
 */
 int lpip_first_data(int lpipe[2], int *first) {
     if (read(lpipe[RD], first, INT_LEN)) {
         printf("primes %d\n", first);
         return 0;
     }
     return -1;
 }

/**
 * @brief 读取左邻居的数据，将不能被first整除的写入右邻居
 * @param lpipe 左邻居的管道符
 * @param rpipe 右邻居的管道符
 * @param first 左邻居的第一个数据
 */
 void transmit_data(int lpipe[2], int rpipe[2], int *first) {
     //receive data from left pipe
     int data;
     while (read(lpipe[RD], &data, INT_LEN) == INT_LEN) {
         if (data % first != 0) {
             write(rpipe[WR], &data, INT_LEN);
         }
     }
     close(lpipe[RD]);
     close(rpipe[WR]);
 }

/**
* @brief 寻找素数
* @param lpipe 左邻居管道
*/
void primes(int lpipe[2]) {
    close(lpipe[WR]);
    int first;
    if (lpip_first_data(lpipe, first) == 0) {
        int rpipe[2];
        pipe(rpipe);
        transmit_data(lpipe, rpipe, first);

        if (fork() == 0) {
            primes(rpipe);
        }
        else {
            close(rpipe[RD]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char const *argv[]) {
    int p[2];
    pipe(p);
    for (int i = 2; i <= 25; i++) {
        write(p, &i, INT_LEN);
    }
    if (fork() == 0) {
        primes(p);
    }
    else {
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}