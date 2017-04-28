#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(int argc, char *argv[]) 
{
    AddRule( "12", "a" );
    AddRule( "1", "b" );
    AddRule( "1", "ab" );
    AddRule( "2", "c" );
    AddRule( "1", "bc" );
    AddRule( "3", "d" );
    AddRule( "23", "e" );
    AddRule( "4", "a" );

    int index = GetDstStringIndex( "123", "bccd" );
    
    printf( "the index of bccd with src_string 123 is:%d\r\n", index );
    
    Clear();
	system( "pause" );
	return 0;
}
