//
// Created by Alain Dong on 12/12/22.
//

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(2, "usage sleep <time>\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}