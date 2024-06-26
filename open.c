/*open.c*/
#include <stdio.h>
#include "filesys.h"
unsigned short aopen(int user_id, char* filename, unsigned short openmode)
{
	unsigned int dinodeid;
	struct inode* inode;
	int i, j;
	dinodeid = namei(filename);
	if (dinodeid = NULL)    /* nosuchfile */
	{
		printf("\nfile does not existed!!\n");
		return NULL;
	}
	inode = iget(dinodeid);
	if (!access(user_id, inode, openmode))    /* access denied */
	{
		printf("\nfile open has not access!!!");
		iput(inode);
		return NULL;
	}
	/* alloc the sys-ofile item */
	for (i = 0; i < SYSOPENFILE; i++)
	{
		if (sys_ofile[i].f_count == 0) break;
	}
	if (i == SYSOPENFILE)
	{
		printf("\nsystem open file too much\n");
		iput(inode);
		return NULL;
	}
	sys_ofile[i].f_inode = inode;
	sys_ofile[i].f_flag = openmode;
	sys_ofile[i].f_count = 1;
	if (openmode & FAPPEND)
		sys_ofile[i].f_off = inode->di_size;
	else
		sys_ofile[i].f_off = 0;
	/* alloc the user open file item */
	for (j = 0; j < NOFILE; j++)
		if (user[user_id].u_ofile[j] == SYSOPENFILE + 1) break;
	if (j == NOFILE)
	{
		printf("\nuser open file too much!!! \n");
		sys_ofile[i].f_count = 0;
		iput(inode);
		return NULL;
	}
	user[user_id].u_ofile[j] = i;//?
	/*if APPEND, free the block of the file before */
	if (!(openmode & FAPPEND))
	{
		for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
			bfree(node_address(inode, i));
		inode->di_size = 0;
	}
	return j;
}