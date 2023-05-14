#include <stdio.h>#include <stdlib.h>#include <string.h>#include <unistd.h>#define N 100// Global variablesint global_initialized = 42;int global_uninitialized;const int global_const = 123;void var() {    // Local variables    int local;    static int static_local;    const int const_local = 456;    printf("Address of global initialized variable: %p\n", &global_initialized);    printf("Address of global uninitialized variable: %p\n", &global_uninitialized);    printf("Address of global const variable: %p\n", &global_const);    printf("Address of local variable: %p\n", &local);    printf("Address of static local variable: %p\n", &static_local);    printf("Address of const local variable: %p\n", &const_local);    sleep(15);}int *create_local_variable() {    int num = 42;    int *address = &num;    return address;}void array() {    char *buf = malloc(N * sizeof(char));    strcpy(buf, "array");    printf("Buf info: %s\n", buf);    free(buf);    //printf(buf);    char *buf2 = malloc(N * sizeof(char));    strcpy("Array", buf2);    printf("Buf2 info: %s\n", buf2);    //free(buf2 + N/2);    printf("Buf2 info: %s\n", buf2);    //free(buf2);}void get_env_var() {    char *my_var = getenv("VAR");    if (my_var == NULL) {        perror("variable not found.");        return;    }    printf("VAR = %s\n", my_var);    if (setenv("VAR", "125", 1) != 0) {        perror("setenv error.");        return;    }    my_var = getenv("VAR");    printf("VAR = %s\n", my_var);}int main() {    printf("PID: %d\n", getpid());    var();//    array();//get_env_var();    int *address = create_local_variable();    if (address == NULL) {        return 1;    }    printf("%d\n", *address);    return 0;}