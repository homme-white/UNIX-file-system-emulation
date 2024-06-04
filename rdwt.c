#include <stdio.h>
#include "filesys.h"

unsigned int read(fd, buf, size)
{
	unsigned long off;
	int block, block_off, i, j;
	struct inode* inode;
	char* temp_buf;

	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FREAD))
	{
		printf("\nthe file is not opened for read\n");
		return 0;
	}
	temp_buf = buf;
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;
	if ((off + size) > inode->di_size) size = inode->di_size - off;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;
	if (block_off + size < BLOCKSIZ)
	{
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fread(buf, 1, size, fd);
		return size;
	}
	fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;
	j = (inode->di_size - off - block_off) / BLOCKSIZ;
	for (i = 0; i < (size - block_off) / BLOCKSIZ; i++)
	{
		fseek(fd, DATASTART + inode->di_addr[j + i] * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	block_off = (size - block_off) % BLOCKSIZ;
	block = inode->di_addr[off + size / BLOCKSIZ + 1];
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fread(temp_buf, 1, block_off, fd);
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;
	return size;

}

unsigned int write(fd, buf, size)
{
	unsigned long off;
	int block, block_off, i, j;
	struct inode* inode;
	char* temp_buf;
	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FWRITE))
	{
		printf("\n the file is not opened for write\n");
		return 0;
	}
	temp_buf = buf;

	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;

	if (block_off + size < BLOCKSIZ)
	{
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(buf, 1, size, fd);
		return size;
	}

	fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fwrite(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;
	for (i = 0; i < (size - block_off) / BLOCKSIZ; i++)
	{
		inode->di_addr[block + 1 + i] = balloc();
		fseek(fd, DATASTART + inode->di_addr[block + 1 + i] * BLOCKSIZ, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}
	block_off = (size - block_off) % BLOCKSIZ;
	block = inode->di_addr[off + size / BLOCKSIZ + 1];
	fseek(fd, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(temp_buf, 1, block_off, fd);
	sys_ofile[user[user_id].u_ofile[fd]].f_off += size;
	return size;
}