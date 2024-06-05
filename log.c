#include <stdio.h>
#include <string.h>
#include "filesys.h"


int login(unsigned short uid, char* passwd)//µÇÂ¼
{
	int i, j;
	//struct pwd tmp;
	char name[PWDSIZ];
	//fseek(fd, DATASTART + 2 * BLOCKSIZ, SEEK_SET);
	//fread(&pwd[0], 1, BLOCKSIZ, fd);
	for (i = 0; i < PWDNUM; i++)
	{
		//tmp = pwd[i];
		strcpy(name, passwd);
		if ((uid == pwd[i].p_uid) && ~(strcmp(passwd, name)))
		{
			for (j = 0; j < USERNUM; j++)
				if (user[j].u_uid == 0) break;
			if (j == USERNUM)
			{

				printf("\nToo much user in the System, waited to login\n");
				return 0;
			}
			else
			{
				user[j].u_uid = uid;
				user[i].u_gid = pwd[i].p_gid;
				user[j].u_default_mode = DEFAULTMODE;
			}
			break;
		}
	}

	if (i == PWDNUM)
	{
		printf("\n incorrect password\n");
		return 0;
	}
	else
		return 1;
}

int logout(unsigned short uid)//µÇ³ö
{
	int i, j, sys_no;
	struct inode* inode;
	for (i = 0; i < USERNUM; i++)
		if (uid == user[i].u_uid) break;
	if (i == USERNUM)
	{
		printf("\nno such a file\n");
		return NULL;
	}
	for (j = 0; j < NOFILE; j++)
	{
		if (user[i].u_ofile[j] != SYSOPENFILE + 1)
		{
			/* iput the mode free the sys_ofile and clear the user-ofile */
			sys_no = user[i].u_ofile[j];
			inode = sys_ofile[sys_no].f_inode;
			iput(inode);
			sys_ofile[sys_no].f_count--;
			user[i].u_ofile[j] = SYSOPENFILE + 1;
		}
	}
	return 1;
}

int regist()
{
	int i; //empty_pwd
	struct pwd passwd[PWDNUM];
	unsigned short passwd_uid;
	unsigned short passwd_gid;

	for (i = 0; i < PWDNUM; i++) {
		if (pwd[i].p_uid == 0 && pwd[i].p_gid == 0) 
		{
			break;
		}
	}
	if (i == PWDNUM) {
		//printf(); full
		return 0;
	}
	//printf(); ÊäÈë
	scanf("%d", &passwd_uid);
	scanf("%d", &passwd_gid);
	scanf("%s", passwd);

	passwd[i].p_uid = passwd_uid;
	passwd[i].p_gid = passwd_gid;
	strcpy(passwd[i].password, passwd);
	//passwd[0].p_uid = 2116; passwd[0].p_gid = 03;
	//strcpy(passwd[0].password, "dddd");
}