#include<stdlib.h>
#include<stdio.h>
#include"filesys.h"
int block_address(struct inode* inode, unsigned int block_num)//输入节点和第几个，返回对应的块号
{
	int i, j;
	unsigned int buf;
	if (block_num < NADDR) 
	{
		return inode->di_addr[block_num];
	}
	else if (block_num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFFSET >= 1)//一层间接索引
	{
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + block_num - NADDR, SEEK_SET);//找到对应的便宜位置
		fread(buf, 1, sizeof(unsigned int), fd);//读出块号
		return buf;
	}
	else
	{
		printf("overflow\n");
		return NULL;
	}
	//未定义所以先不开发
	//else if (block_num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFF >= 2)//二层间接索引
	//{
	//	fseek(fd, DATASTART + inode->di_addr[NADDR + 2 - 1]* BLOCKSIZ + )
	//}

}