#include <stdio.h>
#include <malloc.h>
#include "klib/cQueue.h"

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
void qlist_init(QueueList *qlist)
{
	qlist->size = 0;
	qlist->head = NULL;
	qlist->tail = NULL;
	qlist->SyncMessageMux = Sync_Mutex_Init();
	qlist->SyncMessageSem = Sync_Sem_Init(0);		
	return;
}

void *qlist_get(QueueList *qlist)
{
	return qlist->head->data;
}

int qlist_out(QueueList *qlist)
{
	if(qlist_size(qlist) == 0)
		return -1;
	if(qlist_size(qlist) == 1)
	{
		free(qlist->head->data);
		free(qlist->head);		
		qlist->head = NULL;
		qlist->tail = NULL;
	}
	else// > 1
	{
		QueueElemt *ptempelemt = qlist->head->next;
		free(qlist->head->data);
		free(qlist->head);
		qlist->head = ptempelemt;
	}

	qlist->size--;
}

int qlist_in(QueueList *qlist, void *data)
{
	Sync_Mutex_Lock(qlist->SyncMessageMux);		
	QueueElemt *newElemt;
	if((newElemt = (QueueElemt *)malloc(sizeof(QueueElemt))) == NULL)
		return -1;
	newElemt->data = data;
	newElemt->next = NULL;

	if(qlist_size(qlist) == 0)
	{
		qlist->head = newElemt;
		qlist->tail = newElemt;
	}
	else
	{
		qlist->tail->next = newElemt;
		qlist->tail = newElemt;
	}
	qlist->size++;
	Sync_Mutex_UnLock(qlist->SyncMessageMux);
	Sync_Sem_Signal(qlist->SyncMessageSem);	
	return 0;
}

void qlist_destroy(QueueList *qlist)
{
	while(qlist_size(qlist) > 0)
	{
		qlist_out(qlist);
	}
	memset(qlist, 0, sizeof(QueueList));
	return;
}