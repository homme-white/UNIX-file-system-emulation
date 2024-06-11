#include<stdlib.h>
#include<stdio.h>
#include"filesys.h"
unsigned short node_address(struct inode* inode, unsigned short num)//����ڵ�͵ڼ��������ض�Ӧ�Ŀ��
{
	int i, j;
	unsigned short buf;
	if (num < NADDR)
	{
		return inode->di_addr[num];
	}
	else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned short)) * 1 && NADDR_OFFSET >= 1)//һ��������
	{
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR) * sizeof(unsigned short), SEEK_SET);//�ҵ���Ӧ�ı���λ��
		fread(&buf, sizeof(unsigned short), 1, fd);//�������
		return buf;
	}
	else
	{
		printf("overflow\n");
		return NULL;
	}
	//δ���������Ȳ�����
	//else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFF >= 2)//����������
	//{
	//	fseek(fd, DATASTART + inode->di_addr[NADDR + 2 - 1]* BLOCKSIZ + )
	//}

}

unsigned short di_addr_create(struct inode* inode, unsigned short num)//����ڵ�͵ڼ�����һ���µ�
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
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR)*sizeof(unsigned short), SEEK_SET);//�ҵ���Ӧ�ı���λ��
		fwrite(&buf, sizeof(unsigned short), 1, fd);//д����
		return inode->di_addr[num];
	}
	else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned short)) * 1 && NADDR_OFFSET >= 1)
	{
		buf = balloc();
		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR) * sizeof(unsigned short), SEEK_SET);//�ҵ���Ӧ�ı���λ��
		fwrite(&buf, sizeof(unsigned short), 1, fd);//д����
		return inode->di_addr[num];
	}
	else
	{
		printf("overflow\n");
		return;
	}
}

unsigned short empty_addr(struct inode* inode)//����ڵ㷵����һ���յ�addr
{
	return (inode->di_size - 1) / BLOCKSIZ  + 1;
}
//void node_revise(struct inode* inode, unsigned short num, unsigned short ino)//����ڵ�͵ڼ������иĺ��ino�����ض�Ӧ�Ŀ��
//{
//	int i, j;
//	unsigned short buf;
//	if (num < NADDR)
//	{
//		inode->di_addr[num] = ino;
//	}
//	else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned short)) * 1 && NADDR_OFFSET >= 1)//һ��������
//	{
//		fseek(fd, DATASTART + inode->di_addr[NADDR + 1 - 1] * BLOCKSIZ + (num - NADDR) * sizeof(unsigned short), SEEK_SET);//�ҵ���Ӧ�ı���λ��
//		fread(&buf, sizeof(unsigned short), 1, fd);//�������
//		
//	}
//	else
//	{
//		printf("overflow\n");
//		return NULL;
//	}
//	//δ���������Ȳ�����
//	//else if (num < NADDR + (BLOCKSIZ / sizeof(unsigned int)) * 1 && NADDR_OFF >= 2)//����������
//	//{
//	//	fseek(fd, DATASTART + inode->di_addr[NADDR + 2 - 1]* BLOCKSIZ + )
//	//}
//
//}
//
