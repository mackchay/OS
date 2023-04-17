#include <stdio.h>

// Глобальные переменные
int global_initialized = 42;
int global_uninitialized;

const int global_const = 123;

void foo() {
    // Локальные переменные
    int local;
    static int static_local;
    const int const_local = 456;

    printf("Address of local variable: %p\n", &local);
    printf("Address of static local variable: %p\n", &static_local);
    printf("Address of const local variable: %p\n", &const_local);
}

int main() {
    // Локальные переменные
    int local;
    static int static_local;
    const int const_local = 789;

    printf("Address of local variable: %p\n", &local);
    printf("Address of static local variable: %p\n", &static_local);
    printf("Address of const local variable: %p\n", &const_local);

    printf("Address of global initialized variable: %p\n", &global_initialized);
    printf("Address of global uninitialized variable: %p\n", &global_uninitialized);
    printf("Address of global const variable: %p\n", &global_const);

    foo();

    return 0;
}

