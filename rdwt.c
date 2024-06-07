#include <stdio.h>
#include "filesys.h"

// ��ȡ�ļ�����
unsigned int read(int fd, char* buf, unsigned int size) {
	unsigned long off; // �ļ�ƫ����
	int block, block_off, i, j;
	struct inode* inode;
	char* temp_buf;

	// ��ȡ�ļ���inode�ڵ�
	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;

	// ����ļ��Ƿ��Զ���ʽ��
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD)) {
		printf("\nthe file is not opened for read\n");
		return 0;
	}

	// ��ȡ��ǰ�ļ�ƫ����
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;

	// �����ȡ��С�����ļ���С��������ȡ��С
	if ((off + size) > inode->di_size) {
		size = inode->di_size - off;
	}

	temp_buf = buf; // ���ƻ�����ָ��
	block_off = off % BLOCKSIZ; // ����ƫ��
	block = off / BLOCKSIZ; // ��ǰ���

	// �����ȡ���ݿ�Խ����һ����
	if (block_off + size < BLOCKSIZ) {
		fseek(fd, DATASTART + node_address(inode,block) * BLOCKSIZ + block_off, SEEK_SET);
		fread(buf, 1, size, fd);
		sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // �����ļ�ƫ����
		return size;
	}

	fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ + block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;

	// ��ȡ�����Ŀ�
	j = (size - (BLOCKSIZ - block_off)) / BLOCKSIZ;
	for (i = 0; i < j; i++) {
		block++;
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	// ��ȡ���ʣ�ಿ��
	if ((size - (BLOCKSIZ - block_off)) % BLOCKSIZ > 0) {
		block++;
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, (size - (BLOCKSIZ - block_off)) % BLOCKSIZ, fd);
	}

	sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // �����ļ�ƫ����
	return size;
}

// д�ļ�����
unsigned int write(int fdu, char* buf, unsigned int size) {
	unsigned long off; // �ļ�ƫ����
	int block, block_off, i, j, k = 0;
	struct inode* inode;
	char* temp_buf;

	// ��ȡ�ļ���inode�ڵ�
	inode = sys_ofile[user[user_id].u_ofile[fdu]].f_inode;

	// ����ļ��Ƿ���д��ʽ��
	if (!(sys_ofile[user[user_id].u_ofile[fdu]].f_flag & FWRITE)) {
		printf("\n the file is not opened for write\n");
		return 0;
	}
	temp_buf = buf; // ���ƻ�����ָ��
	// ��ȡ��ǰ�ļ�ƫ����
	off = sys_ofile[user[user_id].u_ofile[fdu]].f_off;
	block_off = off % BLOCKSIZ; // ����ƫ��
	block = off / BLOCKSIZ; 

	// ���д�����ݿ�Խ����һ����
	if (block_off + size < BLOCKSIZ) {
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(buf, 1, size, fd);
		printf("Write Success!\n");
		return size;
	}
	if (sys_ofile[user[user_id].u_ofile[fdu]].f_flag == FAPPEND)
	{
		// д��ʣ��Ŀ鲿��
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ - block_off, fd);
		temp_buf += BLOCKSIZ - block_off;
		k = 1;
	}
	// ����д�����ݣ���Ϊ�¿����洢�ռ�
	for (i = 0; i < (size - k * (BLOCKSIZ - block_off)) / BLOCKSIZ; i++) {
		di_addr_create(inode, block + k + i);
		fseek(fd, DATASTART + node_address(inode, block + k + i) * BLOCKSIZ, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	block_off = (size - k * (BLOCKSIZ - block_off)) % BLOCKSIZ;
	di_addr_create(inode, block + k + i);
	block = node_address(inode, block + k + i);
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(temp_buf, 1, block_off, fd);
	sys_ofile[user[user_id].u_ofile[fdu]].f_off += size; // �����ļ�ƫ����
	inode->di_size = sys_ofile[user[user_id].u_ofile[fdu]].f_off;
	return size;
}