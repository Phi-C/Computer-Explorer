// Some experiments about struct alignment
// 规则1:
// 每个成员都要对齐到对齐数的整数倍地址处(第一个成员的在结构体的offset为0)
// 规则2:
// 结构体总的字节数要是最大对齐数的整数倍
// 不用考虑) micro #pragma pack (n) can specify the alignment bytes g++ -o
// struct_alignment struct_alignment.cpp

#include <stdio.h>
// #pragma pack (n)

struct Example1 {
    char a;
    int b;
    double c;
}; // 1 + (3) + 4 + 8 = 16

struct Example2 {
    char a;
    int b;
    char d;
    double c;
}; // 1 + (3) + 4 + 1 + (7) + 8 = 24

struct Example3 {
    double c;
    int b;
    char a;
}; // 8 + 4 + 1 + (3) = 16

struct BadExample {
    char a;
    // pad 3 bytes to make b satisfy alignment
    int b;
    char c;
    // pad 3 bytes to make sizeof(BadExample) % 4 == 0
};

struct GoodExample {
    int b;
    char a;
    char c;
    // pad 2 bytes to make sizeof(GoodExample) % 4 == 0
};

int main(int argc, char* argv[]) {
    printf("Size of int: %zu bytes\n", sizeof(int));    // 4 bytes
    printf("Size of int: %zu bytes\n", sizeof(char));   // 1 bytes
    printf("Size of int: %zu bytes\n", sizeof(double)); // 8 bytes
    printf("Size of struct Example1: %zu bytes\n", sizeof(struct Example1));
    printf("Size of struct Example2: %zu bytes\n", sizeof(struct Example2));
    printf("Size of struct Example3: %zu bytes\n", sizeof(struct Example3));
    printf("Size of struct BadExample: %zu bytes\n", sizeof(struct BadExample));
    printf("Size of struct GoodExample: %zu bytes\n",
           sizeof(struct GoodExample));
    return 0;
}