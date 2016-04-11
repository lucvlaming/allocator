#include <iostream>
#include <vector>

#include "Parent.h"
#include "Field.h"

struct InliningTest: Parent<InliningTest> {
    Field(1, int64_t) first;  //offset = 16
    Field(2, int32_t) second; //offset = 1935760
    Field(3, int8_t ) third;  //offset = 2903632
    
    //elementcount = 241968

    InliningTest() {
        //note: do not set values for the fields to keep assembly clean
        //somehow the constructor adds a test that cannot be optimized away
    }
};

struct VectorizingTest: Parent<VectorizingTest> {
    Field(1, int32_t) first;
    Field(2, int64_t) second;
    Field(3, int8_t) third;
};

struct Test: Parent<Test> {
    Test(): first(-1), second(-2), third(-3) {}

    Field(1, int64_t) first;
    Field(2, int32_t) second;
    Field(3, int8_t) third;
};

static void escape(const void* p) {
    asm volatile ( "" : : "g" (const_cast<void*>(p)) : "memory" );
}

int main(int, char*[]) {
//    static constexpr size_t N = 1024*1024*4-100;
//    std::vector<Test*> array;
//    array.reserve(N);
//    for (size_t i=0; i<N; ++i) {
//        array.emplace_back(new Test());
//    }
//    for (size_t i=0; i<N; ++i) {
//        array[i]->first = 5;
//    }
//    for (size_t i=0; i<N; ++i) {
//        delete array[i];
//    }

//    auto test = new InliningTest();
//    std::cout << "first-field  " << reinterpret_cast<void*>(&test->first) << std::endl;
//    std::cout << "second-field " << reinterpret_cast<void*>(&test->second) << std::endl;
//    test->second = -1;
//    std::cout << "third-field  " << reinterpret_cast<void*>(&test->third) << std::endl;

//    int32_t storage[1024];

//    for (int i=0; i<1024; ++i) {
//        storage[i] = 5;
//    }
    
//    std::cout << "BLA" << std::endl;
//    escape(storage);
    
    VectorizingTest* bla = reinterpret_cast<VectorizingTest*>(aligned_alloc(1<<25, 2048<<5));
    for (int i=0; i<2048; ++i) {
        (bla + i)->first = 7;
    }
    
    std::cout << "BLA2" << std::endl;
    escape(bla);
    
    return 0;
}
//movq    %rax, %rdx
//movq    %rax, %rcx
//addq    $1, %rax
//andq    $-3145728, %rdx
//andl    $3145727, %ecx
//movq    $7, 16(%rdx,%rcx,8)
//cmpq    %rsi, %rax
//jne     .L8


//movq    %rax, %rdx
//movq    %rax, %rsi
//movq    %rax, %rcx
//andl    $2621439, %edx
//andl    $524288, %esi
//andq    $-3145728, %rcx
//leaq    (%rdx,%rdx,4), %rdx
//addq    $1, %rax
//leaq    (%rdx,%rsi,8), %rdx
//movq    $7, 16(%rcx,%rdx)
//cmpq    %rdi, %rax
//jne     .L8

//movq    %rax, %rdx
//addq    $1, %rax
//andq    $-3145728, %rdx
//movq    $7, 16(%rdx)
//cmpq    %rax, %rcx
//jne     .L8
