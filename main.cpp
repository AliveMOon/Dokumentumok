#include <stdio.h>
#include <stdlib.h>

int main( int nARG, char** ppARG ) {
    for( int i = nARG; i; i-- ) 
	    printf( "%d. \"%s\r\n", nARG-i,*(ppARG+nARG-i));
    return 0;
}
