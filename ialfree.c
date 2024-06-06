/* i�ڵ�������ͷź���*/
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
					// �����ǽڵ���һ�����̴�С������ֱ�Ӷ���block_buf�У�ע�������������i�ڵ�
					fseek(fd, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					// ��blocksiz��С�����ݵ�block_buf�У���ʱ�պö���512���ֽڸ�16��С�����飬ÿ��Ԫ��32��С
					fread(block_buf, BLOCKSIZ, 1, fd);
					// ��־block_buf�Ѿ��������Կ�ʼ������һ�����̣��鿴�ж��ٿ��нڵ�
					block_end_flag = 0;
					i = 0;
				}
				else
				{
					fseek(fd, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					// ������512��С�����ݸ�block_buf����Ϊ512x32-496x32=512
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
	// ���µĿ���i�ڵ㣨���������ڲ��޸ĺ����Ϣ��д�����
	fseek(fd, DINODESTART + dinodeid * DINODESIZ, SEEK_SET);
	//��������ʲô��Ϣ��û��ӣ������ǿ���i�ڵ�
	fwrite(&(block_buf[0].di_number), DINODESIZ, 1, fd);
}
