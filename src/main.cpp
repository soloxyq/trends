#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "klib/cCurl.h"
#include "klib/cDebug.h"
#include "klib/cList.h"
#include "klib/cJSON.h"
#include "klib/cQueue.h"
#include <gtk/gtk.h>

char M_UrlStr[] = "http://api.yi18.net/top/list?id=7&page=1&limit=1000&type=id";
QueueList *TrendMessageList;

typedef struct _YiSubItem
{
	char *title;
	char *imgurl;
	char *keryword;
	int count;
	int fcount;
	int rcount;
	char *from;
	char *time;
	int id;
}YiSubItem;

typedef enum
{
    TRENDS_KEY_UNDEFINED       = 255,
	TRENDS_KEY_0=0,
	TRENDS_KEY_1,
	TRENDS_KEY_2,
	TRENDS_KEY_3,
	TRENDS_KEY_4,
	TRENDS_KEY_5,
	TRENDS_KEY_6,
	TRENDS_KEY_7,
	TRENDS_KEY_8,
	TRENDS_KEY_9,
    TRENDS_KEY_UP,//10
    TRENDS_KEY_DOWN,
    TRENDS_KEY_LEFT,
}TRENDS_Key_t;   

typedef enum
{
	TRENDS_STATE_NOCHANGE	= 0,
	TRENDS_STATE_NUM
}TRENDS_State_t;

typedef enum{
	TRENDS_INPUT_MSG,
	TRENDS_MODULE_MSG,
	TRENDS_STATE_CHANGE
}MsgType;

typedef struct{
	MsgType	m_msgtype;
	union{
		TRENDS_State_t m_nextstate;
		TRENDS_Key_t m_keyvalue;
		};
	unsigned int m_params;
}TRENDS_Message_t;


void *Trends_GuiEngine(void *dummy)
{
    GtkBuilder      *builder;
    GtkWidget       *window;
	
	int argc;
	char **argv;
    gtk_init (&argc, &argv);
 
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "test1.glade", NULL);

    window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    gtk_builder_connect_signals (builder, NULL);
 
    g_object_unref (G_OBJECT (builder));
        
    gtk_widget_show_all(window);                
    gtk_main ();
	
	return (void *)0;
}

void *Trends_StateEngine(void* dummy)
{
	TRENDS_Message_t msg;
	TRENDS_Message_t *ptempmsg;
	
	CCurl m_Curl;//CCurl = new CCurl();
	
	while(1)
	{
		Sync_Sem_Wait(TrendMessageList->SyncMessageSem);
		Sync_Mutex_Lock(TrendMessageList->SyncMessageMux);
		ptempmsg = (TRENDS_Message_t *)qlist_get(TrendMessageList);
		memcpy(&msg,ptempmsg,sizeof(TRENDS_Message_t));
		qlist_out(TrendMessageList);	
		Sync_Mutex_UnLock(TrendMessageList->SyncMessageMux);
		
		switch(msg.m_msgtype)
		{
		case TRENDS_INPUT_MSG:
			if(msg.m_keyvalue == TRENDS_KEY_UP)
			{
				char * m_buf = NULL;
				int m_len = 0;
				int ret;
				if(!m_Curl.LoadUrl(M_UrlStr,M_UrlStr))
				{
					FILE *fp = fopen("./kaka","rb");
					if(!fp)
					{
						fprintf(stdout,"fopen read fail \n");
					}
					fseek (fp , 0 , SEEK_END); 
					long  lSize = ftell (fp); 
					rewind (fp);  
					m_buf = (char*) malloc (sizeof(char)*lSize); 
					if(m_buf == NULL) fprintf(stdout,"malloc fail \n");
					ret = fread (m_buf,1,lSize,fp); 
					if(ret != lSize) fprintf(stdout,"fread may ErrorHappened \n");
					fclose(fp);	
					printf("I READ IT FROM KAKA\n");
				}
				else
					m_Curl.GetBuffer(m_buf,m_len);
				cJSON * m_jroot = cJSON_Parse(m_buf);
				if(!m_jroot) printf("cJSON_Parse Error\n");

				cJSON * m_yiSub = cJSON_GetObjectItem(m_jroot, "yi18");
				int yisize = cJSON_GetArraySize(m_yiSub);
				
				printf("%d\n", yisize);					
				for(int icnt = 0; icnt < yisize; icnt++)
				{
					cJSON * pyiSubSub = cJSON_GetArrayItem(m_yiSub, icnt);
					if(pyiSubSub == NULL) continue;
					YiSubItem newysItem;
					cJSON * pyiSubSubItem = cJSON_GetObjectItem(pyiSubSub, "title");
					newysItem.title = pyiSubSubItem->valuestring;
					printf("%03d.%s\n",icnt,newysItem.title);
				}
				//cJSON * cJSON_GetArrayItem(cJSON * array,int item);
				//printf("%s\n", cJSON_Print(m_jroot))
				
				cJSON_Delete(m_jroot);
				if(m_buf)	free(m_buf);		
				fflush(stdout);	
			}
			break;
		case TRENDS_MODULE_MSG:
		case TRENDS_STATE_CHANGE:
			break;
		default:
			break;
		}
		memset(&msg,0,sizeof(TRENDS_Message_t));
	}
}


int main (int argc, char *argv[])
{
	//curl_global_init(CURL_GLOBAL_ALL);
	//"http://www.winrar.com.cn/download/winrar-x64-521sc.exe"
	static unsigned int params_initvalue = 0;
	TrendMessageList = (QueueList *)malloc(sizeof(QueueList));
	qlist_init(TrendMessageList);
	
	pthread_t pthid_TS,pthid_TG;
	int ret = pthread_create(&pthid_TS, NULL, Trends_StateEngine, NULL);
	ret = pthread_create(&pthid_TG, NULL, Trends_GuiEngine, NULL);
	char cmd[80];
	
	while(1)
	{
		TRENDS_Message_t *pmsg = (TRENDS_Message_t *)malloc(sizeof(TRENDS_Message_t));
		pmsg->m_msgtype = TRENDS_INPUT_MSG;
		pmsg->m_keyvalue = TRENDS_KEY_UNDEFINED;	
		
		scanf("%s",cmd);
		if(strcmp(cmd,"up")==0)
		{
			pmsg->m_keyvalue = TRENDS_KEY_UP;
			pmsg->m_params	= ++params_initvalue;
			qlist_in(TrendMessageList,pmsg);
		}
		else if(strcmp(cmd,"down")==0)
		{
			pmsg->m_keyvalue = TRENDS_KEY_DOWN;
			pmsg->m_params	= ++params_initvalue;
			qlist_in(TrendMessageList,pmsg);
		}
		else
		{
			break;
		}
	}
    return 0;
}

