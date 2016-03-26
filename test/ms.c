#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "ipc_sync.h"

void main(int argc, char *argv[])
{

	printf("exces");
}

int strcmp(const char* const stra, const char* const strb)
{
	if((stra != NULL) && (strb != NULL))
	{
		while(*stra != '\0')
		{
			if(*strb == '\0') return 1;			//strb是子串
			if(*stra++ == *strb++) continue;
		}
		if(*strb == '\0')
			return 0;
		else
			return 1;							////stra是子串
	}
	return -1;
}


/*******************************************************************************
 *线程A: 每1S输出100内随机数
 *线程B: 对随机数进行累加,当和>1000时线程B退出,最后线程A退出
 ******************************************************************************/
int cursor = 0;
int sum = 0;
bool IsTrue = false;
Sync_Mutex_t *Sync_Cursormux;
Sync_Sem_t *Sync_RandSem;

void *ThreadB(void *arg)
{
	while(1)
	{
		Sync_Sem_Wait(Sync_RandSem);
		sum += cursor;
		if(sum > 400)
		{
			IsTrue = true;
			printf("ThreadB EXIT NOW!\n");
			fflush(stdout);
			return (void *)0xabcdef;
		}
		printf("%d\n", sum);
		fflush(stdout);
	}
}

void ThreadA(int argc, char *argv[])
{
	srand(0xaaaaaaaa);
	unsigned char temp = 0;
	Sync_Cursormux = Sync_Mutex_Init();
	Sync_RandSem = Sync_Sem_Init(0);

	pthread_t pthid;
	void *status;
	int ret = pthread_create(&pthid, NULL, ThreadB, NULL);
	while(1)
	{
		if(IsTrue)
			break;
		Sync_Mutex_Lock(Sync_Cursormux);
		cursor = rand()%100;
		Sync_Mutex_UnLock(Sync_Cursormux);
		printf("%d\n", cursor);
		fflush(stdout);
		Sync_Sem_Signal(Sync_RandSem);
		sleep(1);
	}
	pthread_join(pthid,&status);
	printf("0x%x From ThreadB. Now the main thread A will exit()\n", (int)status);
	return ;
}

/*******************************************************************************
 *从长字符串中找出子字符串出现的次数？
 *CountTimes("qwertghyuiopghasdfghhjklgh", "gh")			out:4
 ******************************************************************************/
int CountTimes(char *m_str, char *m_childstr)
{
	int count = 0;
	for(int i = 0; i < strlen(m_str) - strlen(m_childstr) + 1; i++)
	{
		if(strncmp(m_str + i,m_childstr,strlen(m_childstr)) == 0)
			count++;
	}
	return count;
}
/*******************************************************************************
 *单向链表,找出倒数第N个元素?
 *.......
 ******************************************************************************/
typedef struct Node
{
	int data;
	struct Node *next;
}ElemNode;

ElemNode *FindNfromEnd(ElemNode *head, unsigned int N)
{
	ElemNode *cursor = NULL;
	ElemNode *shadow = NULL;
	int i = 0;
	for(cursor = head; cursor->next != NULL; cursor = cursor->next, i++)
	{
		if( i == N - 1)
			shadow = head;
		if(shadow != head)
			shadow = shadow->next;
	}
	return shadow;
}


/*******************************************************************************
 *memcpy原型是void *memcpy(void *dest, const void *src, size_t count)
 *写出my_memcpy要求针对32位系统优化.
 ******************************************************************************/
 void *memcpy(void *dest, const void *src, size_t count)
 {
 	assert((dest!=NULL)&&(src!=NULL));  
 	char *tmp = (char *)dest;
 	char *p = (char *)src;

 	while(count--)
 	{
 		*tmp++ = *p++;
 	}
 	return dest;
 }

 void *my_memcpy(void *dest, const void *src, size_t count)
 {
 	assert((dest!=NULL)&&(src!=NULL));
 	int sizenum = count/4;
 	int weishu = count%4;
 	int *tmp = (int *)dest;
 	int *p = (int *)src;

 	while(sizenum--)
 	{
 		*tmp++ = *p++;
 	}
 	while(weishu--)
 	{
 		*((char *)tmp++) = *((char *)p++);
 	}
 	return dest;
 }