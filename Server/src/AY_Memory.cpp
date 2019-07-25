//#include "pch.h"
#undef UNICODE

#include <stdio.h>
#include <time.h>
#include <AY_Memory.h>

unsigned char	*_AY_MallocMemory(Ui32 Cnt) {
	return((unsigned char	*)malloc(Cnt));
}

void _AY_FreeMemory(unsigned char	*pBuff) {
	free(pBuff);
	return;
}

AY_DeviceRead	*_AY_MallocClientsMemory(Ui32 ClntCnt) {
	AY_DeviceRead	*ptr;
	// Allocate space for a path name
	ptr = (AY_DeviceRead	*)_AY_MallocMemory((ClntCnt * sizeof(AY_DeviceRead)));
	return (ptr);
}

void _AY_FreeClientsMemory(AY_DeviceRead	*ptr) {
	_AY_FreeMemory((unsigned char	*)ptr);
	return;
}

/*
 * This will convert www.google.com to 3www6google3com
 * got it :)
 * */
void ChangetoDnsNameFormat(unsigned char* dns, unsigned char* host)
{
	unsigned int lock = 0, i;
	strcat((char*)host, ".");

	for (i = 0; i < strlen((char*)host); i++)
	{
		if (host[i] == '.')
		{
			*dns++ = i - lock;
			for (; lock < i; lock++)
			{
				*dns++ = host[lock];
			}
			lock++; //or lock=i+1;
		}
	}
	*dns++ = '\0';
}

/*
 *
 * */
unsigned char* ReadName(unsigned char* reader, unsigned char* buffer, int* count)
{
	unsigned char *name;
	unsigned int p = 0, jumped = 0, offset;
	int i, j;

	*count = 1;
	name = (unsigned char*)_AY_MallocMemory(256);

	name[0] = '\0';

	//read the names in 3www6google3com format
	while (*reader != 0)
	{
		if (*reader >= 192)
		{
			offset = (*reader) * 256 + *(reader + 1) - 49152; //49152 = 11000000 00000000 ;)
			reader = buffer + offset - 1;
			jumped = 1; //we have jumped to another location so counting wont go up!
		}
		else
		{
			name[p++] = *reader;
		}

		reader = reader + 1;

		if (jumped == 0)
		{
			*count = *count + 1; //if we havent jumped to another location then we can count up
		}
	}

	name[p] = '\0'; //string complete
	if (jumped == 1)
	{
		*count = *count + 1; //number of steps we actually moved forward in the packet
	}

	//now convert 3www6google3com0 to www.google.com
	for (i = 0; i < (int)strlen((const char*)name); i++)
	{
		p = name[i];
		for (j = 0; j < (int)p; j++)
		{
			name[i] = name[i + 1];
			i = i + 1;
		}
		name[i] = '.';
	}
	name[i - 1] = '\0'; //remove the last dot
	return name;
}

int AY_Delay(Ui64 mSec) {
	time_t	Time = time(0);
	Ui32 j = 0;

	while ((time(0) - Time) < (Si64)(mSec / 1000)) {
		//while (++j < 100000000) { j = 0; }
	}
	return 1;
}


