// Copyright Colby Hall. All Rights Reserved.

#include <iostream>
#include <vector>

#define SHORT 1
#define SUPER_LONG_NAME 12

auto main(int argc, char** argv) -> int {
    (void)argc;
    (void)argv;

    std::cout << "Hello World" << 4 << std::endl;

    void* foo = nullptr;

    return 0;
}

struct foo {};

struct Foo_Bar {
    int x;
};
