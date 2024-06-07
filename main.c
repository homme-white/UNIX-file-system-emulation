
#include <stdio.h>
#include "filesys.h"
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

//struct hinode hinode[NHINO];
//struct dir dir;
//struct file sys_ofile[SYSOPENFILE];
//struct filsys filsys;
//struct pwd pwd[PWDNUM];
//struct user user[USERNUM];
//FILE* fd;
//struct inode* cur_path_inode;
//int user_id;
//int file_block;

void main()
{
	SetConsoleOutputCP(CP_UTF8);
	unsigned short ab_fd1, ab_fd2, ab_fd3, ab_fd4;
	unsigned short bhy_fd1;
	char* buf;
	char tprecord[1000] = "";
	int cmd = 0;
	char cmd_buff[1000] = "";
	int size = 0;
	int n = 0;
	int* open = (int*)malloc(n * sizeof(int));
	char list[10][20] = { "" };
	int online = 0;

	struct hinode* temp1 = hinode;
	struct dir* tmp2 = &dir;
	struct file* tmp3 = &sys_ofile[0];
	struct filsys* tmp4 = &filsys;
	struct pwd* tmp5 = pwd;
	struct user* tmp6 = user;
	struct inode* tmp7 = &cur_path_inode;

	/*printf("\nDo you want to format the disk \n");
	if (getch() == 'y')
	{
		printf("\nFormat Will erase all context on the disk \nAre You Sure! (y(es)/n(o)! \n");
	}
	if (getch() == 'y')*/
	format();

	install();
	strcpy(tprecord, "欢迎使用Utopian操作系统\n请选择您需要的操作:1.mkdir 2.chdir 3.creat&write&close 4.delete 5.ls 0.logout and halt 6.rename\nroot@UtopianUnix:~/root");

	printf("\nCommand : dir  \n");
	_dir();

	login(2118, "abcd");
	online = 1;
	user_id = 0;
	do {
		printf("%s& ", tprecord);
		//fgets(cmd, sizeof(cmd), stdin); // 获取输入的命令 
		scanf("%d", &cmd);//获取输入的命令 
		switch (cmd)
		{
		case 1:printf("\n请输入文件夹名\n");
			scanf("%s", cmd_buff);
			mkdir(cmd_buff);
			break;
		case 2:printf("\n请输入文件夹名\n");
			scanf("%s", cmd_buff);
			chdir(cmd_buff);
			if (strcmp(cmd_buff, "..") == 0)
			{
				char temp[1000] = "";
				char* q = strrchr(tprecord, 47);
				strcpy(temp, tprecord);
				int length = strlen(temp) - strlen(q);
				*(temp + length) = '\0';
				memset(tprecord, 0, 1000);
				strcpy(tprecord, temp);
			}
			else
			{
				strcat(tprecord, "/");
				strcat(tprecord, cmd_buff);
			}
			break;
		case 3:printf("\n请输入文件名\n");
			scanf("%s", cmd_buff);
			open[size] = creat(user_id, cmd_buff, 01777);
			file_block = BLOCKSIZ * 10  + 5;
			buf = (char*)malloc(BLOCKSIZ * 10 + 5);
			write(open[size], buf, BLOCKSIZ * 10 + 5);
			close(user_id, open[size]);
			free(buf);
			strcpy(list[size], cmd_buff);
			size++;
			break;
		case 4:printf("\n请输入文件名\n");
			scanf("%s", cmd_buff);
			delete_f(cmd_buff);
			break;
		case 5:
			_dir();
			break;
		case 6:printf("\n请输入文件名\n");
			scanf("%s", cmd_buff);
			char cmd_buff1[1000] = "";
			printf("\n请输入新文件名\n");
			scanf("%s", cmd_buff1);
			rename(cmd_buff, cmd_buff1);
			break;
		//case 6:printf("\n请输入文件名\n");
		//	scanf("%s", cmd_buff);
		//	{
		//		char cmd_buff1[1000] = "";
		//		printf("\n请输入文件名\n");
		//		scanf("%s", cmd_buff1);
		//		char cmd_buff2[1000] = "";
		//		printf("\n请输入文件名\n");
		//		scanf("%s", cmd_buff2);
		//		cat(cmd_buff, cmd_buff1, cmd_buff2);
		//	}
		//	break;
		case 0:
			online = -1;
			logout(2118);
			halt();
		}
	} while (online == 1);
	return;
}


/*user_id = 0;
mkdir("a2118");
chdir("a2118");
ab_fd1 = creat(user_id, "ab_file0.c", 01777);
file_block = BLOCKSIZ * 6 + 5;
buf = (char*)malloc(BLOCKSIZ * 6 + 5);
write(ab_fd1, buf, BLOCKSIZ * 6 + 5);
close(user_id, ab_fd1);
free(buf);

mkdir("subdir");
chdir("subdir");
ab_fd2 = creat(user_id, "file1.c", 01777);
file_block = BLOCKSIZ * 4 + 20;
buf = (char*)malloc(BLOCKSIZ * 4 + 20);
write(ab_fd2, buf, BLOCKSIZ * 4 + 20);
close(user_id, ab_fd2);
free(buf);

chdir("..");
ab_fd3 = creat(user_id, "ab_file2.c", 01777);
file_block = BLOCKSIZ * 3 + 255;
buf = (char*)malloc(BLOCKSIZ * 10 + 255);
write(ab_fd3, buf, BLOCKSIZ * 3 + 255);
close(user_id, ab_fd3);
free(buf);

_dir();
delete_f("ab_file0.c");
ab_fd4 = creat(user_id, "ab_file3.c", 01777);
buf = (char*)malloc(BLOCKSIZ * 7 + 300);
write(ab_fd4, buf, BLOCKSIZ * 7 + 300);
close(user_id, ab_fd4);
free(buf);

_dir();
ab_fd3 = aopen(user_id, "file2.c", FAPPEND);
buf = (char*)malloc(BLOCKSIZ * 3 + 100);
write(ab_fd3, buf, BLOCKSIZ * 3 + 100);
close(user_id, ab_fd3);
free(buf);

_dir();
chdir("..");
logout(2118);
halt();
}*/