#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#define N 50

void erase_format(char *name, char* format) {
    int j = 0, is_format = 0, format_point = strlen(name);
    for (int i = 0; i < strlen(name); i++) {
        if (name[i] == '.') {
            format_point = i;
            format[j] = name[i];
            is_format = 1;
        }
        if (is_format == 1) {
            format[j] = name[i];
            j++;
        }
    }
    name[format_point] = '\0';
}

void reverse(char *name) {
    for (int i = 0; i < strlen(name)/2; i++) {
        char k = name[i];
        name[i] = name[strlen(name) - 1 - i];
        name[strlen(name) - 1 - i] = k;
    }
}

void read_file(FILE *f_in, char *file_data) {
    char *tmp = malloc(N * sizeof(char));
    do {
        fgets(tmp, N, f_in);
        strcat(file_data, tmp);
    } while (!feof(f_in));
    free(tmp);
}

int main(int argc, char *argv[]) {
    DIR *d;
    FILE *f_in, *f_out;
    struct dirent *dir;
    char *dirname = argv[1];
    char *file_data = malloc(N * N * sizeof(char));
    char *new_dirname = malloc(N * sizeof(char));
    char *file_name = malloc(N * sizeof(char));
    char *format = malloc(N * sizeof(char));
    unsigned int i = 0;
    i = 0;
    realpath(dirname, dirname);
    d = opendir(dirname);
    while (i <= strlen(dirname) && dirname[strlen(dirname) - i - 1] != '/') {
        new_dirname[i] = dirname[strlen(dirname) - i - 1];
        i++;
    }
    new_dirname[i] = '\0';
    chdir("..");
    mkdir(new_dirname, 0700);
    realpath(new_dirname, new_dirname);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                strcpy(file_name, dir->d_name);
                erase_format(file_name, format);
                reverse(file_name);
                strcat(file_name, format);
                format[0] = '\0';
                chdir("..");
                chdir(dirname);
                f_in = fopen(dir->d_name, "r");
                if (f_in) {
                    read_file(f_in, file_data);
                    fclose(f_in);
                }
                chdir("..");
                chdir(new_dirname);
                f_in = fopen(file_name, "w");
                reverse(file_data);
                if (f_in) {
                    fputs(file_data, f_in);
                    fclose(f_in);
                }
                file_data[0] = '\0';
            }
        }
        closedir(d);
    }
    free(file_data);
    free(new_dirname);
    free(file_name);
    return 0;
}
