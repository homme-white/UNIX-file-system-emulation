/* dir.c*/

#include <stdio.h>
#include <string.h>
#include "filesys.h"
void _dir()	/* _dir */
{
	unsigned int di_mode;
	int i, one;
	struct inode* temp_inode;
	printf("\nCURRENT DIRECTORY ..\n");
	for (i = 0; i < dir.size; i++)
	{
		//printf("%d", dir.size);
		if (dir.direct[i].d_ino != DIEMPTY)
		{
			printf("%DIRSIZs", dir.direct[i].d_name);
			temp_inode = iget(dir.direct[i].d_ino);
			di_mode = temp_inode->di_mode;
			for (int j = 0; j < 9; j++)
			{
				one = di_mode % 2;
				di_mode = di_mode / 2;
				if (one) printf("x");
				else printf("-");
			}
			if (temp_inode->di_mode && DIFILE == 1)
			{
				printf("%ld\n", temp_inode->di_size);
				printf("block chain:");
				for (i = 0; i < temp_inode->di_size / BLOCKSIZ + 1; i++)
					printf("%4d", temp_inode->di_addr[i]);
				printf("\n");
			}
			else printf("<dir>\n");
			iput(temp_inode);
		}
	}
}

void mkdir(dirname)	/* mkdir */
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
	inode->i_ino = dirid;
	dir.direct[dirpos].d_ino = inode->i_ino;
	dir.size++;
	/*	fill the new dir buf */
	strcpy(buf[0].d_name, ".");
	buf[0].d_ino = dirid;
	strcpy(buf[1].d_name, "..");
	buf[1].d_ino = cur_path_inode->i_ino;
	block = balloc();
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(buf, 1, BLOCKSIZ, fd);
	inode->di_size = 2 * (DIRSIZ + 2);
	inode->di_number = 1;
	inode->di_mode = user[user_id].u_default_mode;
	inode->di_uid = user[user_id].u_uid;
	inode->di_gid = user[user_id].u_gid;
	inode->di_addr[0] = block;
	iput(inode);
	return;
}

void chdir(dirname) /* chdir */
{
	unsigned int dirid;
	struct inode* inode;
	unsigned short block;
	int i, j, low = 0, high = 0;
	dirid = namei(dirname);
	if (dirid == NULL)
	{
		printf("\n%s does not existed\n", dirname);
		return;
	}
	inode = iget(dirid);
	if (!access(user_id, inode, user[user_id].u_default_mode))
	{
		printf("\nhas not access to the directory %s", dirname);
		iput(inode);
		return;
	}
	/* pack the current directory */
	for (i = 0; i < dir.size; i++)
	{
		for (j = 0; j < DIRNUM; j++)
			if (dir.direct[j].d_ino == 0) break;
		memcpy(&dir.direct[i], &dir.direct[j], DIRSIZ + 2);
		dir.direct[j].d_ino = 0;
	}

	/*	write back the current directory */

	for (i = 0; i < cur_path_inode->di_size / BLOCKSIZ + 1; i++)
	{
		bfree(cur_path_inode->di_addr[i]);
	}

	for (i = 0; i < dir.size; i += BLOCKSIZ / (DIRSIZ + 2))
	{

		block = balloc();
		cur_path_inode->di_addr[i] = block;
		fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
		fwrite(&dir.direct[j], 1, BLOCKSIZ, fd);
	}
	cur_path_inode->di_size = dir.size * (DIRSIZ + 2);
	iput(cur_path_inode);
	cur_path_inode = inode;

	/*	read the change dir from disk */
	j = 0;

	for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
	{
		fseek(fd, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
		fread(&dir.direct[j], 1, BLOCKSIZ, fd);
		j += BLOCKSIZ / (DIRSIZ + 2);
	};

	return;
}