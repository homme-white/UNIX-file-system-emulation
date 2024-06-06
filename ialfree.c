/* i节点分配与释放函数*/
#include <stdio.h>
#include "filesys.h"
struct inode* ialloc()
{
	struct dinode block_buf[BLOCKSIZ / DINODESIZ];
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
				fseek(fd, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
				fread(block_buf, 1, BLOCKSIZ, fd);
				block_end_flag = 0;
				i = 0;
			}
			while (block_buf[i].di_mode == DIEMPTY)
			{
				cur_di++;
				i++;
			}
			if (i == NICINOD)
				block_end_flag = 1;
			else
			{
				filsys.s_inode[filsys.s_pinode--] = cur_di;
				count++;
			}
		}
		filsys.s_rinode = cur_di;
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
			filsys.s_inode[NICINOD] = dinodeid;
			filsys.s_rinode = dinodeid;
		}
	}
}
