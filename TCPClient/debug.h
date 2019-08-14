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
TYPE1(int, c); ת��Ϊ��int ��name_int_type ; (��Ϊ##�Ž������Ϊ name_ ��type �� _type���飬�滻��ǿ������)
TYPE2(int, d);ת��Ϊ�� int ��d_int_type ; (��Ϊ##�Ž������Ϊ name��_��type ��_type���飬�滻��ǿ������)
3���궨����do{ }while(0)

��������һ�ۿ��������ĺ�ʱ�����÷ǳ���֣�ΪʲôҪ��do����while(0)�Ѻ궨��Ķ���������������ǳ���֪�����������ĺô���ʲô������google���ٶ�һ���ˡ�

����  �������ַ�ʽ��Ϊ�˷�����ʹ�ú�����г��ִ�����Ҫ�����¼��㣺

������1���յĺ궨�����warning:
����#define foo() do{}while(0)
������2������һ��������block�������������б������壬���бȽϸ��ӵ�ʵ�֡�
������3������������ж�������ĺ꣬�������Ա�֤��Ϊһ����������ʵ�֣�
��������  #define foo(x) \
�������������� action1(); \
������������ ��action2();
������������������£�
��������if(NULL == pPointer)
 ������������  foo();
���������ͻ����action1��action2����ͬʱ��ִ�е������������Ȼ���ǳ�����Ƶ�Ŀ�ġ�
������4�����ϵĵ�3������õ�����{}Ҳ����ʵ�֣�����Ϊʲôһ��Ҫһ��do{}while(0)�أ������´��룺
������������#define switch(x,y) {int tmp; tmp="x";x=y;y=tmp;}
������������if(x>y)
����������������switch(x,y);
������������else       //error, parse error before else
������������otheraction();
���������ڰѺ���������У�����һ���ֺţ��Ӷ��ᱨ�������һ�㣬���Խ�if��else�����{}�����������Ա���ֺŴ���
����ʹ��do{��.}while(0) ����������������Ϊһ���������﷨��Ԫ���Ӷ������������ķ���������ͬʱ��Ϊ��������ı��������ܹ�ʶ��do{��}while(0)�������õ�ѭ���������Ż�������ʹ�����ַ���Ҳ���ᵼ�³�������ܽ���
*/