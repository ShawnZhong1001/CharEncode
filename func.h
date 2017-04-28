#ifndef FUNC_H
#define FUNC_H

/********************接口函数声明********************/
void AddRule( const char *src_substring, const char *dst_substring );
int GetDstStringIndex( const char *src_string, const char *inspected_string );
void Clear();

/********************结构体定义********************/
/********************字符串对结构体********************/
// 结构体：字符串对——源字符串&目标字符串
typedef struct _string_pair
{
    char *src_substring;
    char *dst_substring;
} string_pair;

// 结构体：字符串对结点
typedef struct _pair_node
{
    string_pair *pair;
    struct _pair_node *next;
} pair_node;

// 结构体：字符串对列表
typedef struct _pair_list
{
    pair_node *first;
    pair_node *last;
} pair_list;

/********************栈结构体********************/
// 实现一个栈数据结构
typedef struct stact_t
{
    string_pair **pair; // pair为一个数组，其元素类型为string_pair*指针（用于栈空间,便于动态申请空间）
    int index; // 指示栈顶元素
    int size; // 栈大小
} STACK;

/********************目标字符串结构体********************/
// 目标字符串结点
typedef struct _dst_string_node
{
    char *dst_string;
    struct _dst_string_node *next;
} dst_string_node;

// 目标字符串列表
typedef struct _dst_string_list
{
    dst_string_node *first;
    dst_string_node *last;
} dst_string_list;

/********************字符串对子函数声明********************/
// 通过返回值创建和初始化两个结构体变量
pair_node *CreatePairNode();
string_pair *CreateStringPair( const char *src_substring, const char *dst_substring );

// 初始化列表
void InitList();

// 释放对结点
void FreePairNode( pair_node *pair_node );

// 释放字符串对
void FreeStringPair( string_pair *string_pair );

// 判断是否
int IsHeaderSubStr( const char *sub_str, const char *string );

// 创建组
void CreateGroup( STACK *stack, pair_list *list, const char *src_string );

/********************栈子函数声明********************/
// 栈的初始化
STACK *StackInit(int size);

// 压栈
int Push( STACK *sp, string_pair *data );

// 弹栈
int Pop( STACK *sp );

// 检查栈是否为空：空返回1，非空返回0
int StackEmpty( STACK *sp );

// 栈销毁
void StackDestroy( STACK *sp );

// 将栈转化成字符串
char *ConvertToStrFromStack( STACK *stack );

/********************目标字符串子函数声明********************/
// 增加目标字符串
void AddDstString( dst_string_list *dst_list, char *str );

// 初始化目标列表
dst_string_list *InitDstList();

// 释放目标列表
void FreeDstList( dst_string_list *dst_list );

#endif
