#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

long file_length(struct dirent *dir) {
    FILE *file = fopen(dir->d_name, "r");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    fclose(file);
    return size;
}

void erase_format(char *name, char *format) {
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
    for (int i = 0; i < strlen(name) / 2; i++) {
        char k = name[i];
        name[i] = name[strlen(name) - 1 - i];
        name[strlen(name) - 1 - i] = k;
    }
}

void read_file(struct dirent *dir, char *file_data) {
    FILE *f_in = fopen(dir->d_name, "r");
    fseek(f_in, 0, SEEK_END);
    long size = ftell(f_in);
    fseek(f_in, 0, SEEK_SET);
    char *tmp = malloc(size * sizeof(char));
    do {
        fgets(tmp, size, f_in);
        strcat(file_data, tmp);
    } while (!feof(f_in));
    free(tmp);
}


void create_reverse_file_name(struct dirent *dir, char *file_name, char *file_data) {
    FILE *file;

    char *format = calloc(FILENAME_MAX, sizeof(char));
    strcpy(file_name, dir->d_name);
    erase_format(file_name, format);
    reverse(file_name);
    strcat(file_name, format);
    free(format);
}

void write_file(char *file_name, char *file_data) {
    FILE *f_out;
    f_out = fopen(file_name, "w");
    reverse(file_data);
    fputs(file_data, f_out);
    fclose(f_out);
}

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    char *dirname = argv[1];
    char *file_data = malloc(sizeof(char));
    char *new_dirname = malloc(PATH_MAX * sizeof(char));
    char *file_name = malloc(NAME_MAX * sizeof(char));
    unsigned int i = 0;
    long size;
    if (!realpath(dirname, dirname)) {
        perror("Can't find directory.");
        return 1;
    }
    if (!(d = opendir(dirname))) {
        perror("Can't open directory.");
        return 1;
    }
    while (i <= strlen(dirname) && dirname[strlen(dirname) - i - 1] != '/') {
        new_dirname[i] = dirname[strlen(dirname) - i - 1];
        i++;
    }
    new_dirname[i] = '\0';
    chdir("..");
    if (!(mkdir(new_dirname, 0700))) {
        perror("Can't create directory.");
        return 1;
    }
    realpath(new_dirname, new_dirname);

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
            chdir(dirname);
            create_reverse_file_name(dir, file_name, file_data);
            size = file_length(dir);
            if (strlen(file_data) < size) {
                file_data = realloc(file_data, size * sizeof(char));
            }
            file_data[0] = '\0';
            read_file(dir, file_data);
            chdir(new_dirname);
            write_file(file_name, file_data);
        }
    }
    closedir(d);
    free(file_data);
    free(new_dirname);
    free(file_name);
    return 0;
}
