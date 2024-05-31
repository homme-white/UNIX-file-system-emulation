/* name.c*/
#include <string.h>
#include <stdio.h>
#include "filesys.h"
unsigned int namei(name) /* namei */
char* name;
{
	int i, notfound = 1;
	for (i = 0; ((i < dir.size) && (notfound)); i++)
	{
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[1].d_ino != 0))
			return i;   /* find */
	}
	/* notfind */
	return NULL;
};

unsigned short iname(name)	/* iname */
char* name;
{
	int i, notfound = 1;
	for (i = 0; ((i < DIRNUM) && (notfound)); i++)
	{
		if (dir.direct[i].d_ino == 0)
		{
			notfound = 0;
			break;
		}
	}
	if (notfound)
	{
		printf("\nThe current directory is full! !\n");
		return 0;
	}
	else
	{
		strcpy(name, dir.direct[i].d_name);
		return i;
	}
}