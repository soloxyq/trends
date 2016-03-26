#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char a_str[6]= {'a','b','c','d','e'};	
	char b_str[]= {'a','b','c','d','e'};	
	char c_str[5]= "abcde";
	char d_str[]= "abcde";

	d_str[2] = 'x';
	printf("%s\n",d_str);
	return 0;
}
