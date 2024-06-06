/* i节点分配与释放函数*/
#include <stdio.h>
#include "filesys.h"
static struct dinode block_buf[BLOCKSIZ / DINODESIZ];
struct inode* ialloc()
{
	struct inode* temp_inode;
	unsigned int cur_di;
	int i, count, block_end_flag;
	if (filsys.s_pinode == NICINOD)
	{
		i = 0;
		count = 0;
		block_end_flag = 1;
		filsys.s_pinode = NICINOD - 1;
		cur_di = filsys.s_rinode;
		while ((count < NICINOD) || (count <= filsys.s_ninode))
		{
			if (block_end_flag)
			{
				if (cur_di <= (BLOCKSIZ * (DINODEBLK - 1) / DINODESIZ))
				{
					// 将铭记节点后的一个磁盘大小的内容直接读入block_buf中，注意这里包括铭记i节点
					fseek(fd, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					// 读blocksiz大小的内容到block_buf中，此时刚好读入512个字节给16大小的数组，每个元素32大小
					fread(block_buf, BLOCKSIZ, 1, fd);
					// 标志block_buf已经满，可以开始处理这一个磁盘，查看有多少空闲节点
					block_end_flag = 0;
					i = 0;
				}
				else
				{
					fseek(fd, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					// 读不足512大小的内容给block_buf，因为512x32-496x32=512
					fread(block_buf, BLOCKSIZ * DINODEBLK - cur_di * DINODESIZ, 1, fd);
					block_end_flag = 0;
					i = 0;
				}
			}
			while (block_buf[i].di_mode != DIEMPTY && i < (BLOCKSIZ / DINODESIZ))
			{
				cur_di++;
				i++;
			}
			if (i == BLOCKSIZ / DINODESIZ)
				block_end_flag = 1;
			else
			{
				if (count != NICINOD)
				{
					filsys.s_inode[filsys.s_pinode++] = cur_di;
					count++;
					cur_di++;
					i++;
				}
				else
					count++;
			}
		}
		filsys.s_rinode = cur_di;
		filsys.s_pinode++;
	}
	temp_inode = iget(filsys.s_inode[filsys.s_pinode]);
	fseek(fd, DINODESTART + filsys.s_inode[filsys.s_pinode] * DINODESIZ, SEEK_SET);
	fwrite(&temp_inode->di_number, 1, sizeof(struct dinode), fd);
	filsys.s_pinode++;
	filsys.s_ninode--;
	filsys.s_fmod = SUPDATE;
	return temp_inode;
}
void ifree(unsigned dinodeid)	 /* ifree */
{
	filsys.s_ninode++;
	if (filsys.s_pinode != NICINOD)    /* notfull */
	{
		filsys.s_pinode--;//?
		filsys.s_inode[filsys.s_pinode] = dinodeid;
		
	}
	else /* full */
	{
		if (dinodeid < filsys.s_rinode)
		{
			filsys.s_rinode = dinodeid;
		}
	}
	block_buf[0].di_mode = DIEMPTY;
	// 将新的空闲i节点（本质是其内部修改后的信息）写入磁盘
	fseek(fd, DINODESTART + dinodeid * DINODESIZ, SEEK_SET);
	//这里由于什么信息都没添加，所以是空闲i节点
	fwrite(&(block_buf[0].di_number), DINODESIZ, 1, fd);
}
