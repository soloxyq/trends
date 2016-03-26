#include <stdio.h>

void recursion(int n, int m, int *data, int data_length)
{
	for(; n >= m; n--)
	{
		data[m-1] = n;
		if(m>1)
			recursion(n-1, m-1,data,data_length);
		else
		{
			for(int i=0; i<data_length; i++)
				printf("%d ",data[i]);
			printf("\n");
			recursion(n-1,1,data,data_length);
			return ;
		}
	}
}

void main(int argc, char *argv[])
{
	int data[3];
	recursion(5,3,data,3);
}
