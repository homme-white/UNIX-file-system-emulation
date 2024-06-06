#include <stdio.h>
#include "filesys.h"

// 读取文件内容
unsigned int read(int fd, char* buf, unsigned int size) {
	unsigned long off; // 文件偏移量
	int block, block_off, i, j;
	struct inode* inode;
	char* temp_buf;

	// 获取文件的inode节点
	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;

	// 检查文件是否以读方式打开
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD)) {
		printf("\nthe file is not opened for read\n");
		return 0;
	}

	// 获取当前文件偏移量
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;

	// 如果读取大小超过文件大小，调整读取大小
	if ((off + size) > inode->di_size) {
		size = inode->di_size - off;
	}

	temp_buf = buf; // 复制缓冲区指针
	block_off = off % BLOCKSIZ; // 块内偏移
	block = off / BLOCKSIZ; // 当前块号

	// 如果读取内容跨越不到一个块
	if (block_off + size < BLOCKSIZ) {
		fseek(fd, DATASTART + node_address(inode,block) * BLOCKSIZ + block_off, SEEK_SET);
		fread(buf, 1, size, fd);
		sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // 更新文件偏移量
		return size;
	}

	fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ + block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;

	// 读取完整的块
	j = (size - (BLOCKSIZ - block_off)) / BLOCKSIZ;
	for (i = 0; i < j; i++) {
		block++;
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	// 读取最后剩余部分
	if ((size - (BLOCKSIZ - block_off)) % BLOCKSIZ > 0) {
		block++;
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, (size - (BLOCKSIZ - block_off)) % BLOCKSIZ, fd);
	}

	sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // 更新文件偏移量
	return size;
}

// 写文件内容
unsigned int write(int fdu, char* buf, unsigned int size) {
	unsigned long off; // 文件偏移量
	int block, block_off, i, j, k = 0;
	struct inode* inode;
	char* temp_buf;

	// 获取文件的inode节点
	inode = sys_ofile[user[user_id].u_ofile[fdu]].f_inode;

	// 检查文件是否以写方式打开
	if (!(sys_ofile[user[user_id].u_ofile[fdu]].f_flag & FWRITE)) {
		printf("\n the file is not opened for write\n");
		return 0;
	}
	temp_buf = buf; // 复制缓冲区指针
	// 获取当前文件偏移量
	off = sys_ofile[user[user_id].u_ofile[fdu]].f_off;
	block_off = off % BLOCKSIZ; // 块内偏移
	block = off / BLOCKSIZ; 

	// 如果写入内容跨越不到一个块
	if (block_off + size < BLOCKSIZ) {
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(buf, 1, size, fd);
		printf("Write Success!\n");
		return size;
	}
	if (sys_ofile[user[user_id].u_ofile[fdu]].f_flag == FAPPEND)
	{
		// 写入剩余的块部分
		fseek(fd, DATASTART + node_address(inode, block) * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ - block_off, fd);
		temp_buf += BLOCKSIZ - block_off;
		k = 1;
	}
	// 按块写入数据，并为新块分配存储空间
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
	sys_ofile[user[user_id].u_ofile[fdu]].f_off += size; // 更新文件偏移量
	inode->di_size = sys_ofile[user[user_id].u_ofile[fdu]].f_off;
	return size;
}