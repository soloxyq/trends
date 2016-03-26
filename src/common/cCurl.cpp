#include "klib/CCurl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//CURLOPT_WRITEFUNCTION,handle the data that have been downloaded by the libcurl
//ptr: pointer to the data that have been downloaded by the libcurl
//size: number of data element
//nmemb: size of data element
//param: parameter passed to the function,specified by CURLOPT_WRITEDATA
//return: length of the data that have been handled
static size_t SaveDataToBuf(void *ptr, size_t size, size_t nmemb, void *param)
{
	void * buf;
	CCurl *pCurl = (CCurl *)param;

	//stop the download
	if(pCurl->m_IsDLStopNow == true)
	{
		printf("Stop the SaveDateToBuf!\n");
		return 0;
	}

	size_t res_size = size * nmemb;

	buf = realloc(pCurl->m_res_buf, pCurl->m_shift+res_size + 1);

	if(buf != NULL)
	{
		pCurl->m_res_buf = buf;
		memcpy(pCurl->m_res_buf + pCurl->m_shift, ptr, res_size);
		pCurl->m_shift += res_size;
		return res_size;
	}
	else
	{
		printf("remalloc failed!\n");
		return 0;
	}

}



//construct function
CCurl::CCurl()
{
	m_buffer = NULL;
	m_buflen = 0;
	m_IsDLStopNow = false;
	m_NeedBackFlag = true;
}

//destruct function
CCurl::~CCurl()
{

}

bool CCurl::LoadUrl_redirect(const char *url, char *new_url)
{
    bool ret = true;
    CURL *curl; // Our curl objects 
    CURLcode result; 

    curl = curl_easy_init(); // Create our curl handle 

    if (curl) 
    {
            // Now set up all of the curl options 
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0); 
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SaveDataToBuf);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA,this);
        curl_easy_setopt(curl, CURLOPT_URL, url); 
        result = curl_easy_perform(curl); // Attempt to retrieve the remote page
        char* redirect_url = NULL;
         curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirect_url); 

        if (result == CURLE_OK) // Did we succeed? 
        {
		printf("url_redirect  = %s\n", redirect_url);
		if(redirect_url != NULL)
			strcpy(new_url, redirect_url);
		else
			ret = false;
           } 
           else 
           { 
		printf("redirect error\n");
		ret = false;
           } 
    } 
    curl_easy_cleanup(curl); 

    return ret; 
}



//url: specify the url
//return: true for success and false for fail
bool CCurl::LoadUrl(const char *url, char* redirect_url)
{
	CURL *curl_handle;
	int ret=0;
	int flag=0;
	
	if((url == NULL)||(strlen(url)==0))
	{
		printf("url = NULL or strlen(url)=0\n");
		return false;
	}

	m_IsDLStopNow = false;
	//save the url
	strncpy(m_url, url, MAX_URL_LEN);
	curl_handle = curl_easy_init();
	if(!curl_handle)
  	{
  		printf("create CURL error!");
  		return false;
 	 }
	
	m_shift = 0;
	m_res_buf = NULL;
	//char *redirect_url = NULL; 

	
	ret|=curl_easy_setopt(curl_handle, CURLOPT_URL, m_url);
      // ret|= curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0); 
	ret|=curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, SaveDataToBuf);
	ret|=curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,this);
	
	ret|=curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	ret|=curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10);
	ret|=curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 60);
	ret|=curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT,10000);
	ret|=curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME,1);
	ret|=curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION,1);
	ret|=curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1);

	ret|=curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);


	            // Now set up all of the curl options 


	if(ret!=CURLE_OK)
	{
		printf("set curl setopt error");
		m_buffer = NULL;
		m_buflen = 0;
		return false;
	}
	
	ret=curl_easy_perform(curl_handle);			//program poll here
    //   curl_easy_getinfo(curl_handle, CURLINFO_REDIRECT_URL, redirect_url); 
	   
	switch(ret)
	{
		case CURLE_OK:
			flag=1;
			break;
		case CURLE_COULDNT_CONNECT:
			printf("connect to server error\n");
			flag=2;
			break;
		case CURLE_REMOTE_ACCESS_DENIED:
			printf("ask for http connect erro\nr");
			flag=2;
			break;
		case CURLE_HTTP_RETURNED_ERROR:
			printf("http return error\n");
			flag=2;
			break;
		case CURLE_WRITE_ERROR:
			printf("write data error\n");
			flag=3;
			break;
		default:
			printf("default error\n");
			flag=4;
			break;
	}

	if(curl_handle!=NULL)
	{
		curl_easy_cleanup(curl_handle);
	}

	//if download failed,free the buffer
	if(flag!=1)
	{
		if(m_res_buf != NULL)
		{
			free(m_res_buf);
			m_res_buf = NULL;
		}
		
		m_buffer = NULL;
		m_buflen = 0;
		return false;
	}
	else
	{
		m_buffer = m_res_buf;
		m_buflen = m_shift;

		if(m_NeedBackFlag == true)
		{
			FILE *fp = fopen("./kaka","wb");
			if(!fp)
			{
				fprintf(stdout,"fopen fail \n");
			}
			int written = fwrite(m_buffer, m_buflen, 1 , fp);   
			fclose(fp);
		}		
		return true;
	}
}

//return: true for success and false for fail
bool CCurl::StopLoad()
{
	m_IsDLStopNow = true;
	return true;
}

//buf: return the pointer to the buffer
//buflen: length of the data
//return: true for success and false for fail 
bool CCurl:: GetBuffer(char *&buf, int &buflen)
{
	if((m_buffer == NULL)||(m_buflen == 0))
	{
		printf("GetBuffer NULL!\n");
		buf=NULL;
		buflen = 0;
		return false;
	}
	else
	{
		//printf("GetBuffer Ok!\n");
		buf=(char *)m_buffer;
		buflen= m_buflen;
		return true;
	}
	
}

/*
size_t callback_write_file(void *ptr, size_t size, size_t nmemb, void *userp)
 {
    //fprintf(stdout,"size=%d\n",size);
    fwrite(ptr,size,nmemb,(FILE *)userp);
    return size * nmemb;     //必须返回这个大小, 否则只回调一次, 不清楚为何.
 }
 void *func_download_file(FILE *fp)
 {

    curl_easy_setopt(curl, CURLOPT_URL, "http://10.255.64.30:8082/20140225/vt/01B5CE3D7366577E5A85179BCAC742C9.8BB4DB74.8d8c8a829de211e3aae200266cf463ac"); //设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //设置超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
    //curl_easy_setopt(curl, CURLOPT_HEADERDATA, 1);      //下载数据包
    //curl_easy_setopt(curl, CURLOPT_RANGE, "0-500");     //用于断点续传, 设置下载的分片

    //char buffer[MAXHEADLEN] = {0x0};
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_write_file); //设置下载数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stdout,"curl execute fail\n");
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
 }
int main()
{

    CURL *curl = NULL;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    printf("%p\n", curl);
    fclose(fp);
    return 0;
}

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t SaveDataToFile(void *ptr, size_t size, size_t nmemb, void *param)
{
	int written = fwrite(ptr, size, nmemb , (FILE *)param);   
	return written;	 
}
*/

