#ifndef __DEBUG__
#define __DEBUG__

#include <cstdio>

#define log(stream, msg)(fprintf(stream, "%s@line%d:%s\n", __func__, __LINE__, msg))
#define logi(stream, format, i)(fprintf(stream, "%s@line%d:"#format"\n", __func__, __LINE__, i))
#define logmi(stream, format, msg, i)(fprintf(stream, "%s@line%d:"#format"\n", __func__, __LINE__, msg, i))

#endif // !DEBUG




/*
1 #define TYPE1(type,name)   type name_##type##_type
2 #define TYPE2(type,name)   type name##_##type##_type
TYPE1(int, c); 转换为：int 　name_int_type ; (因为##号将后面分为 name_ 、type 、 _type三组，替换后强制连接)
TYPE2(int, d);转换为： int 　d_int_type ; (因为##号将后面分为 name、_、type 、_type四组，替换后强制连接)
3、宏定义中do{ }while(0)

　　　第一眼看到这样的宏时，觉得非常奇怪，为什么要用do……while(0)把宏定义的多条语句括起来？非常想知道这样定义宏的好处是什么，于是google、百度一下了。

　　  采用这种方式是为了防范在使用宏过程中出现错误，主要有如下几点：

　　（1）空的宏定义避免warning:
　　#define foo() do{}while(0)
　　（2）存在一个独立的block，可以用来进行变量定义，进行比较复杂的实现。
　　（3）如果出现在判断语句过后的宏，这样可以保证作为一个整体来是实现：
　　　　  #define foo(x) \
　　　　　　　 action1(); \
　　　　　　 　action2();
　　　　在以下情况下：
　　　　if(NULL == pPointer)
 　　　　　　  foo();
　　　　就会出现action1和action2不会同时被执行的情况，而这显然不是程序设计的目的。
　　（4）以上的第3种情况用单独的{}也可以实现，但是为什么一定要一个do{}while(0)呢，看以下代码：
　　　　　　#define switch(x,y) {int tmp; tmp="x";x=y;y=tmp;}
　　　　　　if(x>y)
　　　　　　　　switch(x,y);
　　　　　　else       //error, parse error before else
　　　　　　otheraction();
　　　　在把宏引入代码中，会多出一个分号，从而会报错。这对这一点，可以将if和else语句用{}括起来，可以避免分号错误。
　　使用do{….}while(0) 把它包裹起来，成为一个独立的语法单元，从而不会与上下文发生混淆。同时因为绝大多数的编译器都能够识别do{…}while(0)这种无用的循环并进行优化，所以使用这种方法也不会导致程序的性能降低
*/