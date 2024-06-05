
#include <stdio.h>
#include "filesys.h"
#include <stdlib.h>
#include <conio.h>

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
	unsigned short ab_fd1, ab_fd2, ab_fd3, ab_fd4;
	unsigned short bhy_fd1;
	char* buf;
	struct hinode* temp1 = hinode;
struct dir *tmp2 = &dir;
struct file *tmp3 = sys_ofile;
struct filsys *tmp4 = &filsys;
struct pwd * tmp5 = pwd;
struct user*tmp6 = user;
struct inode* tmp7 = &cur_path_inode;

	/*printf("\nDo you want to format the disk \n");
	if (getch() == 'y')
		printf("\nFormat Will erase all context on the disk \nAre You Sure! (y(es)/n(o)! \n");
	if (getch() == 'y')*/
	format();

	install();

	printf("\nCommand : dir  \n");
	_dir();

	login(2118, "abcd");
	user_id = 0;
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
	ab_fd3 = creat(user_id, "_file2.c", 01777);
	file_block = BLOCKSIZ * 3 + 255;
	buf = (char*)malloc(BLOCKSIZ * 10 + 255);
	write(ab_fd3, buf, BLOCKSIZ * 3 + 255);
	close(user_id, ab_fd3);
	free(buf);

	_dir();
	delete("ab_file0.c");
	ab_fd4 = creat(user_id, "ab_file3.c", 01777);
	buf = (char*)malloc(BLOCKSIZ * 8 + 300);
	write(ab_fd4, buf, BLOCKSIZ * 8 + 300);
	close(ab_fd4);
	free(buf);

	ab_fd3 = aopen(user_id, "ab_file2.c", FAPPEND);
	buf = (char*)malloc(BLOCKSIZ * 3 + 100);
	write(ab_fd3, buf, BLOCKSIZ * 3 + 100);
	close(ab_fd3);
	free(buf);

	_dir();
	chdir("..");
	logout();
	halt();
}