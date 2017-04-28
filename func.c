// Spit func.c into several functions
// StringPair.c
// DstString.c
// StackOpera.c
// __main.c
#include <stdio.h>
#include <stdlib.h>

#include "func.h"

// 全局变量
// 转换规则列表
pair_list pairlist;
dst_string_list *dst_list;
int list_init = 0; // 列表是否被初始化的标志

// 接口函数一：添加规则表项
void AddRule( const char *src_substring, const char *dst_substring )
{
    /********************如果未初始化********************/
    if ( list_init == 0 )
	{
		InitList(); 
	}

    /********************创建字符串对规则********************/
	// pair_node *pNode = CreatePairNode();
    // pNode->pair = CreateStringPair( src_substring, dst_substring );
    pair_node *pNode = (pair_node *) malloc(sizeof(pair_node));
    pNode->next = NULL;
    
    pNode->pair = (string_pair *) malloc(sizeof(string_pair));
    pNode->pair->src_substring = (char *) malloc((strlen(src_substring) + 1) * sizeof(char));
    memcpy(pNode->pair->src_substring, src_substring, strlen(src_substring) + 1);
    pNode->pair->dst_substring = (char *) malloc((strlen(dst_substring) + 1) * sizeof(char));
    memcpy(pNode->pair->dst_substring, src_substring, strlen(dst_substring) + 1);
	
    /********************更新字符串对列表********************/
	if ( pairlist.first == NULL )
	{
        pairlist.first = pNode; 
        pairlist.last = pNode;
	}
    else
    {
	    pairlist.last->next = pNode;
	    pairlist.last = pNode;     
    }	
}

// 接口函数二：输出待查目标字符串的索引
int GetDstStringIndex( const char *src_string, const char *inspected_string )
{
    /********************对源字符串进行编码，并按字典顺序存放********************/
    // 栈的初始化操作，栈用于存放已匹配编码规则的字符串对（源字符串，目标字符串）
    STACK *stack = StackInit( strlen( src_string ) );
    // 目标字符串列表初始化
    dst_list = InitDstList();
    // 为源字符串编码，将编码后的目标字符串按字典顺序存放在目标字符串列表中
	CreateGroup( stack, &pairlist, src_string );
	
    /********************对目标字符串进行匹配，返回索引值********************/
	dst_string_node *pNode = dst_list->first;
	int ret = -1;
    
    while ( pNode != NULL )
	{
        ++ret;

        if ( strcmp( pNode->dst_string, inspected_string ) == 0 )
        {
            break;
        }
        
        pNode = pNode->next;
    }

    if ( ret > -1 )
    {
        ++ret;
    }
	
    if ( pNode == NULL )
    {
        ret = -1;
    }

    // 初始化之后必须释放和摧毁
	FreeDstList( dst_list );
	StackDestroy( stack );

    return ret;    
}

// 接口函数三：清空所有数据
void Clear()
{
    pair_node *pNode = pairlist.first;
    pair_node *temp;

    while ( pNode != NULL )
    {
        temp = pNode;
        pNode = pNode->next;

        // FreePairNode( temp );
        free(temp->pair->src_substring);
        free(temp->pair->dst_substring);
        free(temp->pair);
        free(temp);
    }   
}

/********************************************************************************/
/********************字符串对子函数定义********************/
// 初始化列表
void InitList()
{
    pairlist.first = NULL; 
    pairlist.last = NULL;
	list_init = 1;	
}

//pair_node *CreatePairNode()
//{
//    pair_node *pNode = (pair_node*)malloc( sizeof( pair_node ) );
//    pNode->pair = NULL;
//    pNode->next = NULL;
//    return pNode;
//}
//
//string_pair *CreateStringPair( const char *src_substring, const char *dst_substring )
//{
//    string_pair *pPair = (string_pair*)malloc( sizeof( string_pair ) );
//
//    // 初始化字符数组的时，注意长度，注意结尾需要加上null
//    pPair->src_substring = (char*)malloc((strlen(src_substring) + 1) * sizeof(char));
//    memcpy( pPair->src_substring, src_substring, strlen(src_substring) + 1 );
//
//    pPair->dst_substring = (char*)malloc((strlen(dst_substring) + 1) * sizeof(char));
//    memcpy( pPair->dst_substring, dst_substring, strlen(dst_substring) + 1 );
//    
//    return pPair;       
//}

//void FreePairNode( pair_node *pair_node )
//{
//    FreeStringPair( pair_node->pair ); 
//    // free( pair_node );
//}
//
//void FreeStringPair( string_pair *string_pair )
//{
//    free( string_pair->src_substring );
//    free( string_pair->dst_substring );
//    free( string_pair );
//}

// 递归函数
// 参数pair_list* pairlist为规则列表，遍历规则列表中所有源子串，如果源子串
// 和参数src_string的前面部分子串相同，则将对应规则入栈，继续匹配src_string余下的字符串
void CreateGroup( STACK* stack, pair_list* pairlist, const char* src_string )
{
    pair_node* pNode = pairlist->first;
    
    /********************按规则列表逐个比较********************/
    while ( pNode != NULL )
    {
        // 判断规则中的项是否属于src_string前面部分的子串
        if ( IsHeaderSubStr( pNode->pair->src_substring, src_string ) )
        {
            Push( stack, pNode->pair );
            printf( "push %s-%s\r\n", pNode->pair->dst_substring, pNode->pair->src_substring );
            
            /********************核心算法：递归匹配********************/
            // !!如果完全相同，说明src_string匹配到了结尾，整个栈的规则是有效可用的。!!
            if ( strcmp( pNode->pair->src_substring, src_string ) == 0 )  
            {
                char *tmp = ConvertToStrFromStack( stack );
                AddDstString( dst_list, tmp );
            }
            else
            {
                // !!递归匹配src_string剩下的部分字符串!!
                CreateGroup( stack, pairlist, src_string + strlen(pNode->pair->src_substring) );
            }
            
            // 出栈，匹配链表中下一个规则
            // 此处的出栈定义与一般意义不同，不删除结点，只移动栈顶索引
            Pop( stack );
            printf( "pop %s-%s\r\n", pNode->pair->dst_substring, pNode->pair->src_substring );
        }
        
        pNode = pNode->next;
    }
}

int IsHeaderSubStr( const char *sub_str, const char *string )
{
    if ( strlen(sub_str) > strlen(string) )
    {
        return 0;
    }
    
    int i;
    
    // 逐个比较字符串中的字符
    for ( i = 0; i < strlen(sub_str); ++i )
    {
        if ( sub_str[i] != string[i] )
        {
            break;
        }
    }
    
    if ( i == strlen(sub_str) )
    {
        return 1;
    }
    
    return 0;
}

char *ConvertToStrFromStack( STACK *stack )
{
    int i = 0;
    int len = 0;
    
    /********************计算长度，分配内存********************/
    for ( i = 0; i < stack->index; ++i )
    {
        len += strlen( stack->pair[i]->dst_substring );
    }
    
    char *tmp = (char*)malloc( (len+1) * sizeof( char ) );
    tmp[len] = '\0'; // 注意字符串数组的最后一位
    
    len = 0;
    
    /********************进行复制********************/
    for ( i = 0; i < stack->index; ++i )
    {
        memcpy( tmp+len, stack->pair[i]->dst_substring, strlen( stack->pair[i]->dst_substring ) );
        len += strlen( stack->pair[i]->dst_substring );
    }
    
    return tmp;
}

/********************栈子函数定义********************/
// 实现栈的操作
// 栈的初始化
STACK *StackInit( int size )
{
    STACK *tmp;
    // 初始化指向栈结构的指针（任何指针都要初始化，因为指针编译后只占4字节（32位机），
    // 用于存放地址，故申请一个内存空间，存放结构体的对象，然后这个内存空间的地址给到指针）
    tmp = (STACK*)malloc(sizeof(STACK));
    tmp->size = size;
    tmp->index = 0;
    // 将void * 类型的指针强制转化为指向NODE*的指针的指针类型NODE**
    // (即开始的指针是指向void的指针，强制转化为指向NODE*的指针)
    tmp->pair = (string_pair**)malloc(size * sizeof(string_pair*));
    
    return tmp; //将指向初始化的栈的地址返回
}

// 检查栈是否为空：空返回1，非空返回0
int StackEmpty( STACK *sp )
{
    if (sp->index <= 0 || sp == NULL )
        return 1;
    else return 0;
}

// 压栈
int Push( STACK *sp, string_pair *data )
{
    if ( sp->index >= sp->size || sp == NULL )
    {
        return 1; // 压栈失败
    }
    else
    {
        sp->pair[sp->index++] = data;
        return 0;
    }
}

// 弹栈
int Pop( STACK *sp )
{
    if ( sp->index <= 0 || sp == NULL )
    {
        return 1;
    }
    else
    {
        --sp->index; // 只修改了栈指针
        return 0;
    }
}

// 销毁栈
void StackDestroy( STACK *sp )
{
    free( sp->pair );
    free( sp );
}

/********************目标字符串子函数定义********************/
// 将编码后的字符串加入目标字符串列表中，在加入的时候即进行字典排序存放
void AddDstString( dst_string_list *dst_list, char *str )
{
	dst_string_node *pNode;

    /********************目标字符串列表为空********************/
    if ( dst_list->first == NULL )
	{
        pNode = (dst_string_node*)malloc( sizeof( dst_string_node ) );
        pNode->dst_string = str;
        pNode->next = NULL;

        dst_list->first = pNode;
        dst_list->last = pNode;
        
        // printf( "push:%s\r\n", str );
        return;         
    }
    
    /********************与第一个字符串比较********************/
    // strcmp实现的就是字典排序
    // 字符串比目标字符串列表第一个字符串小，则插在列表首部
    if ( strcmp( str, dst_list->first->dst_string ) < 0 )
    {
        pNode = (dst_string_node*)malloc( sizeof( dst_string_node ) );
        pNode->dst_string = str;
        pNode->next = dst_list->first;

        dst_list->first = pNode; 
        
        //printf( "push:%s\r\n", str );
        return;            
    }

    // 字符串比目标字符串列表第一个字符串小，则不改变列表，直接对字符串进行释放
    if ( strcmp( str, dst_list->first->dst_string ) == 0 )
    {
        free( str );

		return;            
    }
    
    /********************字符串插入在字符串列表中间********************/
    dst_string_node *p1, *p2;
    p1 = dst_list->first;
    p2 = dst_list->first->next;

	while ( p2 != NULL )
	{
		if ( strcmp( str, p2->dst_string ) < 0 )
        {
            pNode = (dst_string_node*)malloc( sizeof( dst_string_node ) );
            pNode->dst_string = str;
            pNode->next = p2;
            p1->next = pNode;

            //printf( "push:%s\r\n", str );
            return;            
        }
    
        if ( strcmp( str, p2->dst_string ) == 0 )
        {
            free( str );

			return;            
        }
        
        p1 = p2;
		p2 = p2->next;
	}

    /********************字符串插在字符串列表尾部********************/
	pNode = (dst_string_node *)malloc( sizeof( dst_string_node ) );
    pNode->dst_string = str;
    pNode->next = NULL;

    p1->next = pNode; 
    dst_list->last = pNode;

    // printf( "push:%s\r\n", str );
    return;
}

dst_string_list *InitDstList()
{
	dst_string_list *dst_list = (dst_string_list*)malloc( sizeof( dst_string_list ) );
	dst_list->first = NULL;
	dst_list->last = NULL;
	return dst_list;
}

void FreeDstList( dst_string_list *dst_list )
{
	dst_string_node *pNode = dst_list->first;
	dst_string_node *pTmp;

	while ( pNode != NULL )
	{
		// 必须先把原结点复制出来，这样释放之后才能继续找到下一个结点
        pTmp = pNode;
		pNode = pNode->next;
        
		// printf( "free %s\r\n", pTmp->dst_string );
        // 先释放结构体内指针，再释放整体
		free( pTmp->dst_string );
		free( pTmp );
	}
}
