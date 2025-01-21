## 关于
`Computer-Exploerer/cpp_explorer/key_words`通过一些具体的toy example代码, 探究了C/C++中的一些关键字的作用.
| 关键字 | 描述 |
| ---- | ---- |
|volatile | 告诉编译器, 被`volatile`修饰的变量可能会在程序的控制之外被改变, 因此编译器不应该对其进行优化。`volatile`的主要作用是确保每次访问该变量时都直接从内存中读取, 而不是使用寄存器中的缓存值。|

## 具体解读
### volatile
声明为`volatile`的对象会被编译器排除在优化之外，因为它们的值可以随时被当前代码范围之外的代码更改。比如一下几个具体的场景:
1. 全局变量被当前scope外的中断服务程序修改了：全局变量可以表示一个数据端口, 这个数据端口会被动态更新。如果没有将这个全局变量声明为`volatile`, 那么编译器会对其进行优化, 从而只读取一次，然后不断使用寄存器中的这个值, 从而无法捕捉到它的变化.
2. 多线程应用中的全局变量也需要声明为`volatile`: 道理是类似的。

在[Understanding “volatile” qualifier in C | Set 2 (Examples)
](https://www.geeksforgeeks.org/understanding-volatile-qualifier-in-c/)这篇文章里, 有如下例子:
```C
#include <stdio.h>
int main(void)
{
    const int local = 10;
    int *ptr = (int*) &local;

    printf("Initial value of local : %d \n", local);

    *ptr = 100;

    printf("Modified value of local: %d \n", local);

    return 0;
}
```
按文章描述:
1. 如果没有开编译器优化, 会输出10和100.
2. 如果开了编译器优化, 值不会被改变.

但是个人觉得这个例子并不是很好, 原因如下:
1. `local`已经被声明为`const int`了, 后续又通过`int*`类型的指针对其进行修改, 这不是一个好的编程习惯
2. 根据[CompilerExplorer](https://gcc.godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:11,endLineNumber:5,positionColumn:11,positionLineNumber:5,selectionStartColumn:11,selectionStartLineNumber:5,startColumn:11,startLineNumber:5),source:'//+Type+your+code+here,+or+load+an+example.%0A%23include+%3Cstdio.h%3E%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++++const+int+local+%3D+10%3B%0A++++int*+ptr+%3D+(int*)%26local%3B%0A%0A++++printf(%22Initialvalue+of+local:+%25d%5Cn%22,+local)%3B%0A++++*ptr+%3D+100%3B%0A++++printf(%22Modified+value+of+local:+%25d%5Cn%22,+local)%3B%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:g142,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+gcc+14.2+(Editor+%231)',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:executor,i:(argsPanelShown:'1',compilationPanelShown:'0',compiler:g142,compilerName:'',compilerOutShown:'0',execArgs:'',execStdin:'',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'',source:1,stdinPanelShown:'1',wrap:'1'),l:'5',n:'0',o:'Executor+x86-64+gcc+14.2+(C%2B%2B,+Editor+%231)',t:'0')),k:33.33333333333333,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)以及我使用gcc13.3的实验, 即使不开启编译优化, 输出的结果也是10和10.

在`Computer-Explorer/cpp_explorer/key_words/test_volatile_ex2.cpp`里, 如果`flag`不使用`volatile`修饰, 在开启编译优化后可以看到非预期行为.但是`Computer-Explorer/cpp_explorer/key_words/test_volatile_ex1.cpp`里通过使用条件变量和互斥锁来避免忙等待(busy waiting)后, `test_volatile_ex2.cpp`里的现象并未出现。这是因为锁本身就可以确保变量的可见性: 当一个线程释放锁时，它会将所有修改的变量刷新到内存中;当另外一个线程获得锁时, 它会从主内存中重新加载变量的值。因此: 如果已经使用锁来保护某个变量的访问, 那么该变量的可见性已经由锁保证，此时无需再使用`volatile`。

关于编译器优化, 可以参考[Options That Control Optimization](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)。这里只提一个点: -O3是一种比较激进的优化，它不保证在任何情况都等小，可能会出现改变程序行为的情况。在正规商业软件开发中一般不会使用-O3,而是使用-O2加上一些-f选项。