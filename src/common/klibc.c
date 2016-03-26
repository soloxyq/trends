typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned long      UINT32;
typedef char               INT8;
typedef short              INT16;
typedef long               INT32;

char* kstrcpy(char *dest, const char *src)
{
	unsigned int i;
	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return dest;
}

int kstrcmp(const char *s1, const char *s2)
{
	while (*s1 != '\0' && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

unsigned long kstrlen(const char *src)
{
	unsigned long i;
	for (i = 0; src[i] != '\0'; i++);
	return i;
}

char * kstrrev(char *s)
{
    int j,k,c;
    for(k=0;s[k] != 0;k++);
    for(j=0,k--;j<k;j++,k--) {
	c = s[j];
	s[j] = s[k];
	s[k] = c;
    }
    return s;
}

void* kmemset(void *dest, int c, unsigned int  len)
{
	UINT32 dwCount;
	UINT8 *DestBuf;

	DestBuf = (UINT8 *)dest;

	for (dwCount = 0;dwCount < len;dwCount++)
		*(DestBuf + dwCount) = c;

	return dest;
}

int kmemcpy(const void *buf1, const void *buf2, unsigned int  len)
{
	UINT32 dwCount;
	UINT8* cmp1, *cmp2;

	cmp1 = (UINT8*)buf1;
	cmp2 = (UINT8*)buf2;

	for (dwCount = 0;dwCount < len;dwCount++)
	{
		if (*(cmp1 + dwCount) < *(cmp2 + dwCount))
			return -1;	//buf1 less than buf2
		else if (*(cmp1 + dwCount) > *(cmp2 + dwCount))
			return 1;	//buf1 greater than buf2
	}

	return 0;	//buf1 identical to buf2
}

void * kmemmove(void * dest, const void * src, unsigned int n)
{
	unsigned long dstp = (unsigned long) dest;
	unsigned long srcp = (unsigned long) src;

	UINT32 temp;

	if (!n)
		return dest;

	//if no overlap, just call memcpy
	if((dstp + n < srcp) || (srcp + n < dstp))
	{
		return  memcpy(dest, src, n);
	}
	else
	{
		char *a = dest;
		const char *b = src;
		if (src != dest)
		{
			if (src > dest)
			{
				while (n--) *a++ = *b++;
			}
			else
			{
				a += n - 1;
				b += n - 1;
				while (n--) *a-- = *b--;
			}
		}
		return dest;
	}
	
}