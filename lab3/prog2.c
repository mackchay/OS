#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#define BUFFER_SIZE 4096

void create_directory(char *path, mode_t mode) {
    if (mkdir(path, mode) == -1) {
        perror("Can't create directory.");
    }
}

void display_directory_contents(char *path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    else {
        perror("Directory does not exist.");
    }
}

int remove_directory(char *path) {
    DIR *dir = opendir(path);
    int r1 = -1;
    if (dir) {
        struct dirent *entity;

        size_t len, path_len = strlen(path);
        r1 = 0;
        while ((entity = readdir(dir)) && !r1) {
            int r2 = -1;
            if (!strcmp(entity->d_name, ".") || !strcmp(entity->d_name, "..")) {
                continue;
            }

            char *entity_path;
            len = path_len + strlen(entity->d_name) + 2;
            entity_path = malloc(len);

            if (entity_path) {
                struct stat st;

                snprintf(entity_path, len, "%s/%s", path, entity->d_name);
                if (!stat(entity_path, &st)) {
                    if (S_ISDIR(st.st_mode))
                        r2 = remove_directory(entity_path);
                    else
                        r2 = unlink(entity_path);
                }
                free(entity_path);
            }
            r1 = r2;
        }
        closedir(dir);
    }
    else {
        perror("Directory does not exist.");
    }

    if (!r1)
        r1 = rmdir(path);

    return r1;
}

void create_file(char *path) {
    FILE *file = fopen(path, "w");
    fclose(file);
    printf("%s", path);
}

void display_file_contents(char *path) {
    FILE *file = fopen(path, "r");
    int c;
    if (file) {
        while ((c = fgetc(file)) != EOF) {
            putchar(c);
        }
    }
    else {
        perror("File doesn't exist.");
    }
    fclose(file);
}

void remove_file(char *path) {
    if (remove(path) == -1) {
        perror("Can't delete this file.");
    }
}

void create_symbolic_link(char *target_path, char *link_path) {
    symlink(target_path, link_path);
}

void display_symbolic_link_contents(char *path) {
    char buf[BUFFER_SIZE];
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        printf("%s\n", buf);
    }
    else {
        perror("Symbolic link does not exist.");
    }
}

void display_symbolic_link_target_contents(char *path) {
    char buf[BUFFER_SIZE];
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        display_file_contents(buf);
    }
    else {
        perror("Symbolic link does not exist.");
    }
}

void remove_symbolic_link(char *path) {
    if (remove(path) == -1) {
        perror("Can't remove symbolic link.");
    }
}

void create_hard_link(char *target_path, char *link_path) {
    link(target_path, link_path);
}

void remove_hard_link(char *path) {
    if (remove(path) == -1) {
        perror("Can't remove hard link.");
    }
}

void display_permissions_and_hard_links(char *path) {

    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("Entity does not exist.");
    }
    else {
        printf("Permissions: %o\n", sb.st_mode & 0777);
        printf("File Permissions: \t");
        printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
        printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
        printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
        printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
        printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
        printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
        printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
        printf( (sb.st_mode & S_IROTH) ? "r" : "-");
        printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
        printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n\n");

        printf("Number of hard links: %ld\n", sb.st_nlink);
    }
}

void change_permissions(char *path, mode_t mode) {
    if (chmod(path, mode) == -1) {
        perror("Can't change permissions on file.");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <action> <path>\n", argv[0]);
        return 1;
    }
    char *action = argv[0];
    char *path = argv[1];

    if (strcmp(action, "./create_directory") == 0) {
        if (argc != 3) {
            printf("Usage: %s create_directory <path> <mode>\n", argv[0]);
            return 1;
        }
        mode_t mode = strtol(argv[2], NULL, 8);
        create_directory(path, mode);
    } else if (strcmp(action, "./display_directory_contents") == 0) {
        display_directory_contents(path);
    } else if (strcmp(action, "./remove_directory") == 0) {
        remove_directory(path);
    } else if (strcmp(action, "./create_file") == 0) {
        create_file(path);
    } else if (strcmp(action, "./display_file_contents") == 0) {
        display_file_contents(path);
    } else if (strcmp(action, "./remove_file") == 0) {
        remove_file(path);
    } else if (strcmp(action, "./create_symbolic_link") == 0) {
        if (argc != 3) {
            printf("Usage: %s create_symbolic_link <target> <link>\n", argv[0]);
            return 1;
        }
        char *target_path = argv[1];
        char *link_path = argv[2];
        create_symbolic_link(target_path, link_path);
    } else if (strcmp(action, "./display_symbolic_link_contents") == 0) {
        display_symbolic_link_contents(path);
    } else if (strcmp(action, "./display_symbolic_link_target_contents") == 0) {
        display_symbolic_link_target_contents(path);
    } else if (strcmp(action, "./remove_symbolic_link") == 0) {
        remove_symbolic_link(path);
    } else if (strcmp(action, "./create_hard_link") == 0) {
        if (argc < 4) {
            printf("Usage: %s create_symbolic_link <path> <mode>\n", argv[0]);
            return 1;
        }
        char *target_path = argv[1];
        char *link_path = argv[2];
        create_hard_link(target_path, link_path);
    } else if (strcmp(action, "./remove_hard_link") == 0) {
        remove_hard_link(path);
    } else if (strcmp(action, "./display_permissions_and_hard_links") == 0) {
        display_permissions_and_hard_links(path);
    } else if (strcmp(action, "./change_permissions") == 0) {
        if (argc != 2) {
            printf("Usage: %s create_symbolic_link <target> <link>\n", argv[0]);
            return 1;
        }
        mode_t mode = strtol(argv[2], NULL, 8);
        change_permissions(path, mode);
    } else {
        printf("Invalid format.\nUsage: %s <command> <args...>\n", argv[0]);
    }
    return 0;
}