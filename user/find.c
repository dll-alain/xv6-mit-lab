//
// Created by Alain Dong on 12/13/22.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char const *filename) {
    char buf[512], *p;
    int fid, fid1;
    struct stat st, st1;
    struct dirent de;

    if ((fid = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fid, &st) < 0) {
        fprintf(2, "find: cannot fstat %s\n", path);
        close(fid);
        return;
    }

//    if (st.type != T_DIR) {
//        fprintf(2, "usage: find <DIRECTORY> <filename>\n");
//        return;
//    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        fprintf(2, "find: path too long\n");
        return;
    }

    switch (st.type) {
        case T_FILE:
            fprintf(2, "usage: find <DIRECTORY> <filename>\n");
            return;
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/'; //p指向最后一个'/'之后
            while ((read(fid, &de, sizeof de)) == sizeof de) {
                if (de.inum == 0) {
                    continue;
                }
                if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
                    // 若是'.'或'..'目录，则跳过
                    continue;
                }
                // 在模板后添加属于自己的内容：自己的文件名
                memmove(p, de.name, DIRSIZE);
                if ((fid1 = open(buf, 0)) >= 0) {
                    if ((fstat(fid1, &) >= 0) {
                        switch (st1.type) {
                            case T_FILE:
                                if (!strcmp(de.name, filename)) {
                                    printf("%s\n", buf);
                                }
                                close(fid1);
                                break;
                            case T_DIR:
                                close(fid1);
                                find(buf, filename);
                                break;
                            case T_DEVICE:
                                close(fid1);
                                break;
                        }
                    }
                }
            }
            break;
    }
    close(fid);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find path filename\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}