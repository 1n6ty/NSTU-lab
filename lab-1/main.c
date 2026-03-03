#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void print_directories() {
    DIR *dir;
    struct dirent *entry;
    struct stat stbuf;

    if ((dir = opendir(".")) == NULL) {
        perror("Ошибка opendir");
        return;
    }

    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) == NULL){
        perror("Ошибка getcwd");
        return;
    }
    printf("каталог %s\n", buffer);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (stat(entry->d_name, &stbuf) != -1) {
            if (S_ISDIR(stbuf.st_mode)) {
                printf("\tкаталог %s\n", entry->d_name);
            }
        }
    }

    closedir(dir);
    printf("\n...............\n\n");
}

int main(int argc, char *argv[]) {
    struct stat st_dot, st_parent;

    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_каталога>\n", argv[0]);
        return 1;
    }

    if (chdir(argv[1]) != 0) {
        perror("Ошибка chdir");
        return 1;
    }

    while (1) {
        print_directories();

        if (stat(".", &st_dot) == -1 || stat("..", &st_parent) == -1) {
            perror("Ошибка stat");
            break;
        }

        if (st_dot.st_ino == st_parent.st_ino && st_dot.st_dev == st_parent.st_dev) {
            break;
        }

        if (chdir("..") != 0) {
            perror("Ошибка chdir");
            break;
        }
    }

    return 0;
}