#include<stdlib.h>
#include<stdio.h>
#include"filesys.h"
int block_address(struct inode* inode, unsigned int block_num)//����ڵ�͵ڼ��������ض�Ӧ�Ŀ��
{
	int i, j;
	unsigned int buf;
	if (block_num < NADDR) 
	{
		return inode->di_addr[block_num];
	}
	else if (block_num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFFSET >= 1)//һ��������
	{
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + block_num - NADDR, SEEK_SET);//�ҵ���Ӧ�ı���λ��
		fread(buf, 1, sizeof(unsigned int), fd);//�������
		return buf;
	}
	else
	{
		printf("overflow\n");
		return NULL;
	}
	//δ���������Ȳ�����
	//else if (block_num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFF >= 2)//����������
	//{
	//	fseek(fd, DATASTART + inode->di_addr[NADDR + 2 - 1]* BLOCKSIZ + )
	//}

}