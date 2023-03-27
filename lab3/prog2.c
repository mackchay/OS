#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <dirent.h>

void create_directory(char *path, mode_t mode) {
    mkdir(path, mode);
}

void display_directory_contents(char *path) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
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
    if (!r1)
        r1 = rmdir(path);

    return r1;
}

void create_file(char *path) {
    FILE *file = fopen(path, "w");
    fclose(file);
}

void display_file_contents(char *path) {
    FILE *file = fopen(path, "r");
    int c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }
    fclose(file);
}

void remove_file(char *path) {
    remove(path);
}

void create_symbolic_link(char *target_path, char *link_path) {
    symlink(target_path, link_path);
}

void display_symbolic_link_contents(char *path) {
    char buf[1024];
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        printf("%s\n", buf);
    }
}

void display_symbolic_link_target_contents(char *path) {
    char buf[1024];
    ssize_t len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        display_file_contents(buf);
    }
}

void remove_symbolic_link(char *path) {
    remove(path);
}

void create_hard_link(char *target_path, char *link_path) {
    link(target_path, link_path);
}

void remove_hard_link(char *path) {
    remove(path);
}

void display_permissions_and_hard_links(char *path) {
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    printf("Permissions: %o\n", sb.st_mode & 0777);
    printf("Number of hard links: %ld\n", sb.st_nlink);
}

void change_permissions(char *path, mode_t mode) {
    chmod(path, mode);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <action> <path>\n", argv[0]);
        return 1;
    }
    char *action = argv[1];
    char *path = argv[2];

    if (strcmp(action, "create_directory") == 0) {
        if (argc < 3) {
            printf("Usage: %s create_directory <target> <link>\n", argv[0]);
            return 1;
        }
        mode_t mode = strtol(argv[3], NULL, 8);
        create_directory(path, mode);
    } else if (strcmp(action, "display_directory_contents") == 0) {
        display_directory_contents(path);
    } else if (strcmp(action, "remove_directory") == 0) {
        remove_directory(path);
    } else if (strcmp(action, "create_file") == 0) {
        create_file(path);
    } else if (strcmp(action, "display_file_contents") == 0) {
        display_file_contents(path);
    } else if (strcmp(action, "remove_file") == 0) {
        remove_file(path);
    } else if (strcmp(action, "create_symbolic_link") == 0) {
        if (argc < 4) {
            printf("Usage: %s create_symbolic_link <target> <link>\n", argv[0]);
            return 1;
        }
        char *target_path = argv[2];
        char *link_path = argv[3];
        create_symbolic_link(target_path, link_path);
    } else if (strcmp(action, "display_symbolic_link_contents") == 0) {
        display_symbolic_link_contents(path);
    } else if (strcmp(action, "display_symbolic_link_target_contents") == 0) {
        display_symbolic_link_target_contents(path);
    } else if (strcmp(action, "remove_symbolic_link") == 0) {
        remove_symbolic_link(path);
    } else if (strcmp(action, "create_hard_link")==0) {
        if (argc < 4) {
            printf("Usage: %s create_symbolic_link <target> <link>\n", argv[0]);
            return 1;
        }
        char *target_path = argv[2];
        char *link_path = argv[3];
        create_hard_link(target_path, link_path);
    } else if (strcmp(action, "remove_hard_link")==0) {
        remove_hard_link(path);
    } else if (strcmp(action, "display_permissions_and_hard_links")==0) {
        display_permissions_and_hard_links(path);
    } else if (strcmp(action, "change_permissions")==0) {
        if (argc < 3) {
            printf("Usage: %s create_symbolic_link <target> <link>\n", argv[0]);
            return 1;
        }
        mode_t mode = strtol(argv[3], NULL, 8);
        change_permissions(path, mode);
    }
    return 0;
}