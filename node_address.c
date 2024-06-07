#include<stdlib.h>
#include<stdio.h>
#include"filesys.h"
unsigned short node_address(struct inode* inode, unsigned short num)//输入节点和第几个，返回对应的块号
{
	int i, j;
	unsigned short buf;
	if (num < NADDR)
	{
		return inode->di_addr[num];
	}
	else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned short)) * 1 && NADDR_OFFSET >= 1)//一层间接索引
	{
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR) * sizeof(unsigned short), SEEK_SET);//找到对应的便宜位置
		fread(&buf, sizeof(unsigned short), 1, fd);//读出块号
		return buf;
	}
	else
	{
		printf("overflow\n");
		return NULL;
	}
	//未定义所以先不开发
	//else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFF >= 2)//二层间接索引
	//{
	//	fseek(fd, DATASTART + inode->di_addr[NADDR + 2 - 1]* BLOCKSIZ + )
	//}

}

unsigned short di_addr_create(struct inode* inode, unsigned short num)//输入节点和第几个创一个新的
{
	int i, j;
	unsigned short buf;
	if (num < NADDR)
	{
		inode->di_addr[num] = balloc();
		return inode->di_addr[num];
	}
	else if (num == NADDR)
	{
		inode->di_addr[NADDR + 1 - 1] = balloc();
		buf = balloc();
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR)*sizeof(unsigned short), SEEK_SET);//找到对应的便宜位置
		fwrite(&buf, sizeof(unsigned short), 1, fd);//写入块号
		return inode->di_addr[num];
	}
	else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned short)) * 1 && NADDR_OFFSET >= 1)
	{
		buf = balloc();
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR) * sizeof(unsigned short), SEEK_SET);//找到对应的便宜位置
		fwrite(&buf, sizeof(unsigned short), 1, fd);//写入块号
		return inode->di_addr[num];
	}
	else
	{
		printf("overflow\n");
		return;
	}
}

unsigned short empty_addr(struct inode* inode)//输入节点返回下一个空的addr
{
	return (inode->di_size - 1) / BLOCKSIZ  + 1;
}
//void node_revise(struct inode* inode, unsigned short num, unsigned short ino)//输入节点和第几个还有改后的ino，返回对应的块号
//{
//	int i, j;
//	unsigned short buf;
//	if (num < NADDR)
//	{
//		inode->di_addr[num] = ino;
//	}
//	else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned short)) * 1 && NADDR_OFFSET >= 1)//一层间接索引
//	{
//		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR) * sizeof(unsigned short), SEEK_SET);//找到对应的便宜位置
//		fread(&buf, sizeof(unsigned short), 1, fd);//读出块号
//		
//	}
//	else
//	{
//		printf("overflow\n");
//		return NULL;
//	}
//	//未定义所以先不开发
//	//else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFF >= 2)//二层间接索引
//	//{
//	//	fseek(fd, DATASTART + inode->di_addr[NADDR + 2 - 1]* BLOCKSIZ + )
//	//}
//
//}
//
