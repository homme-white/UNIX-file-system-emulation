#include<stdlib.h>
#include<stdio.h>
#include"filesys.h"

void cat(char* name1, char* name2, char* name3)
{
	int n1, n2, n3;
	int i, j, k = 0;
	char name[FILENAME_MAX];
	unsigned short block;
	unsigned short buf;
	struct inode* ino1;
	struct inode* ino2;
	struct inode* ino3;
	n1 = namei(name1);
	n2 = namei(name2);
	ino1 = iget(n1);

	ino2 = iget(n2);

	n3 = creat(0, name3, 01777);
	ino3 = sys_ofile[user[user_id].u_ofile[n3]].f_inode;
	//close(0, 0);
	ino3->di_size = ino1->di_size;
	for (i = 0; i < ((ino1->di_size - 1) / BLOCKSIZ + 1) && i < NADDR + NADDR_OFFSET; i++, k++)
	{

		ino3->di_addr[i] = ino1->di_addr[i];
		ino3->di_gid = ino1->di_gid;
	}
	i = (ino1->di_size - 1) / BLOCKSIZ + 1;
	k = (ino1->di_size - 1) / BLOCKSIZ + 1;
	j = 0;
	if (empty_addr(ino3) < NADDR + NADDR_OFFSET) {
		for (; j < ((ino2->di_size - 1) / BLOCKSIZ + 1) && k < NADDR; k++, j++) {
			ino3->di_addr[k] = ino2->di_addr[j];
		}
	}
	if (k == NADDR + NADDR_OFFSET) {
		ino3->di_addr[NADDR + 1 - 1] = di_addr_create(ino3, ino2->di_addr[j]);
	}
	for (; j < (ino2->di_size - 1) / BLOCKSIZ + 1; j++, k++)
	{
		buf = node_address(ino2, j);
		fseek(fd, DATASTART + ino3->di_addr[NADDR + 1 - 1] * BLOCKSIZ + sizeof(unsigned short) * (j + i - NADDR), SEEK_SET);
		fwrite(&buf, sizeof(unsigned short), 1, fd);
	}
	ino3->di_size += ((ino2->di_size - 1) / BLOCKSIZ + 1) * BLOCKSIZ;
	iput(ino1);
	iput(ino2);
	iput(ino3);
}