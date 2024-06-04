#include <stdio.h>
#include "filesys.h"

/*unsigned int read(fd, buf, size)//fd:�ļ������� ,buf:������,size:��ȡ�Ĵ�С
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
*/



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
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fread(buf, 1, size, fd);
		sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // �����ļ�ƫ����
		return size;
	}

	// ��ȡʣ��Ŀ鲿��
	fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;

	// ��ȡ�����Ŀ�
	j = (size - (BLOCKSIZ - block_off)) / BLOCKSIZ;
	for (i = 0; i < j; i++) {
		block++;
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	// ��ȡ���ʣ�ಿ��
	if ((size - (BLOCKSIZ - block_off)) % BLOCKSIZ > 0) {
		block++;
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, (size - (BLOCKSIZ - block_off)) % BLOCKSIZ, fd);
	}

	sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // �����ļ�ƫ����
	return size;
}

// д�ļ�����
unsigned int write(int fd, char* buf, unsigned int size) {
	unsigned long off; // �ļ�ƫ����
	int block, block_off, i;
	struct inode* inode;
	char* temp_buf;

	// ��ȡ�ļ���inode�ڵ�
	inode = sys_ofile[user[user_id].u_ofile[fd]].f_inode;

	// ����ļ��Ƿ���д��ʽ��
	if (!(sys_ofile[user[user_id].u_ofile[fd]].f_flag & FWRITE)) {
		printf("\n the file is not opened for write\n");
		return 0;
	}

	// ��ȡ��ǰ�ļ�ƫ����
	off = sys_ofile[user[user_id].u_ofile[fd]].f_off;
	block_off = off % BLOCKSIZ; // ����ƫ��
	block = off / BLOCKSIZ; // ��ǰ���

	temp_buf = buf; // ���ƻ�����ָ��

	// ���д�����ݿ�Խ����һ����
	if (block_off + size < BLOCKSIZ) {
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(buf, 1, size, fd);
		sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // �����ļ�ƫ����
		return size;
	}

	// д��ʣ��Ŀ鲿��
	fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fwrite(temp_buf, 1, BLOCKSIZ - block_off, fd);
	temp_buf += BLOCKSIZ - block_off;

	// ����д�����ݣ���Ϊ�¿����洢�ռ�
	for (i = 0; i < (size - (BLOCKSIZ - block_off)) / BLOCKSIZ; i++) {
		block++;
		if (inode->di_addr[block] == 0) { // Ϊ�µĿ����洢�ռ�
			inode->di_addr[block] = balloc();
		}
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ, fd);
		temp_buf += BLOCKSIZ;
	}

	// д�����ʣ�ಿ��
	if ((size - (BLOCKSIZ - block_off)) % BLOCKSIZ > 0) {
		block++;
		if (inode->di_addr[block] == 0) { // Ϊ�µĿ����洢�ռ�
			inode->di_addr[block] = balloc();
		}
		fseek(fd, DATASTART + inode->di_addr[block] * BLOCKSIZ, SEEK_SET);
		fwrite(temp_buf, 1, (size - (BLOCKSIZ - block_off)) % BLOCKSIZ, fd);
	}

	sys_ofile[user[user_id].u_ofile[fd]].f_off += size; // �����ļ�ƫ����
	return size;
}