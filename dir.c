/* dir.c*/

#include <stdio.h>
#include <string.h>
#include "filesys.h"
void _dir()	/* _dir */
{
	unsigned short int di_mode;
	int i, one;
	struct inode* temp_inode;
	printf("\nCURRENT DIRECTORY:dir.size=%d\n", dir.size);
	printf("%d", dir.size);
	for (i = 0; i < dir.size; i++)
	{
		//printf("%d", dir.size);
		if (dir.direct[i].d_ino != DIEMPTY)
		{
			printf("% 20s", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			di_mode = temp_inode->di_mode;
			if (temp_inode->di_mode & DIFILE)
				printf(" f");
			else
				printf(" d");
			for (int j = 0; j < 9; j++)
			{
				one = di_mode % 2;
				di_mode = di_mode / 2;
				if (one) printf("x");
				else printf("-");
			}
			if (temp_inode->di_mode & DIFILE)
			{
				printf("%ld", temp_inode->di_size);
				printf("block chain:");
				for (int k = 0; k < temp_inode->di_size / BLOCKSIZ + 1; k++)
					printf("%d-", node_address(temp_inode, k));
				printf("\n");
			}
			else printf("<dir>block chain:%d\n", dir.direct[i].d_ino);
			iput(temp_inode);
		}
	}
}

void mkdir_file(char* dirname)
{
	if (dirname != NULL)
	{
		char* tmp_name = GetFilename(dirname);
		int dirid;
		mkdir_file(dirname);
		mkdir(tmp_name);
		chdir(tmp_name);
	}
	else
	{
		return;
	}
}

void mkdir(char* dirname)	/* mkdir */
{

	int dirid, dirpos;

	struct inode* inode;

	struct direct buf[BLOCKSIZ / (DIRSIZ + 2)];
	unsigned int block;


	dirid = namei(dirname);
	if (dirid != NULL)
	{
		inode = iget(dirid);
		if (inode->di_mode & DIDIR)
			printf("\n%s directory already existed! ! \n");
		else
			printf("\n%s is a file name, &can't create a dir the same name", dirname);

		iput(inode);
		return;
	}

	dirpos = iname(dirname);
	inode = ialloc();
	dirid = inode->i_ino;
	dir.direct[dirpos].d_ino = dirid;
	dir.direct[dirpos + 1].d_ino = 0;
	dir.size++;
	/*	fill the new dir buf */
	strcpy(buf[0].d_name, ".");
	buf[0].d_ino = dirid;
	strcpy(buf[1].d_name, "..");
	buf[1].d_ino = cur_path_inode->i_ino;
	buf[2].d_ino = 0;
	block = balloc();
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(buf, 1, BLOCKSIZ, fd);
	inode->di_size = 2 * (DIRSIZ + 2);
	inode->di_number = 1;
	inode->di_mode = user[user_id].u_default_mode | DIDIR;
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;

	iput(inode);
	return;
}

int chdir(char* dirname) /* chdir */
{
	unsigned int dirid;
	struct inode* inode;
	unsigned short block;
	struct dir* temp = &dir;
	int i, j, low = 0, high = 0;
	dirid = namei(dirname);
	if (dirid == NULL)
	{
		printf("\n%s does not existed\n", dirname);
		return -1;
	}
	inode = iget(dirid);
	if (!access(user_id, inode, user[user_id].u_default_mode))
	{
		printf("\nhas not access to the directory %s", dirname);
		iput(inode);
		return -1;
	}
	/* pack the current directory */
	for (i = 0; i < dir.size; i++)
	{
		//�����ǰĿ¼��û��ʹ��
		if (dir.direct[i].d_ino == 0)
		{
			//��������б�ʹ�õ�Ŀ¼���Ŀ¼��ۺ���һ��
			for (j = i + 1; j < DIRNUM; j++)//?
				if (dir.direct[j].d_ino == 0)
					break;
			memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ + 2);
			dir.direct[j].d_ino = 0;
		}
	}
	//for (i = 0; i < dir.size; i++)
	//{
	//	for (j = 0; j < DIRNUM; j++) {
	//		if (dir.direct[j].d_ino == 0)
	//		{
	//			break;
	//		}
	//	}
	//	memcpy(&dir.direct[j], &dir.direct[i], DIRSIZ + 2);
	//	dir.direct[j].d_ino = 0;

	//}

	/*	write back the current directory */

	for (i = 0; i < cur_path_inode->di_size / BLOCKSIZ + 1; i++)
	{
		bfree(node_address(cur_path_inode, i));
	}

	for (i = 0, j = 0; i < dir.size; i += BLOCKSIZ / (DIRSIZ + 2), j++)
	{
		block = di_addr_create(cur_path_inode, j);
		fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
		fwrite(&dir.direct[j], 1, BLOCKSIZ, fd);
	}
	cur_path_inode->di_size = dir.size * (DIRSIZ + 2);
	iput(cur_path_inode);
	cur_path_inode = inode;
	dir.size = cur_path_inode->di_size / (DIRSIZ + 2);
	/*	read the change dir from disk */

	for (i = 0, j = 0; i < inode->di_size / BLOCKSIZ + (inode->di_size % BLOCKSIZ != 0); i++)
	{
		//��ȡ�µ�Ŀ¼���ݿ�
		fseek(fd, DATASTART + node_address(inode, i) * BLOCKSIZ, SEEK_SET);
		fread(&dir.direct[j], 1, BLOCKSIZ, fd);
		j += BLOCKSIZ / (DIRSIZ + 2);
	}

	return 0;
}