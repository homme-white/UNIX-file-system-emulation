/* name.c*/
#include <string.h>
#include <stdio.h>
#include "filesys.h"
unsigned int namei(char* name) /* namei查找名为name的文件 */
{
	struct dir temp = dir;
	int i, notfound = 1;
	for (i = 0; ((i < dir.size) && (notfound)); i++)
	{
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))
			return dir.direct[i].d_ino;   /* find */
	}
	/* notfind */
	return NULL;
};

unsigned short iname(char* name)	/* iname */
{
	struct dir temp = dir;
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
		strcpy(dir.direct[i].d_name, name);
		dir.direct[i].d_ino = 1;
		return i;
	}
}

char* GetFilename(char* p)
{
	char ch = '\\';
	char* q = strrchr(p, ch);
	int length;
	if (q != NULL) {
		length = strlen(p) - strlen(q);
		*(p + length) = '\0';
		return q + 1;
	}
	return q;
}

int rename(char* name, char* re)
{
	struct dir temp = dir;
	int i, notfound = 1;
	for (i = 0; ((i < dir.size) && (notfound)); i++)
	{
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))
		{
			strcpy(dir.direct[i].d_name, re);
			return 0;
			/* find */
		}
	}
	/* notfind */
	return NULL;

}

//int main() {
//	char name[30] = "baobaobaba\\baoerzi\\abcde";
//	char* a;
//	while ((a = GetFilename(name)) != NULL)
//	{
//		printf("%s\n", a);
//	}
//	printf("%s", name);
//}
