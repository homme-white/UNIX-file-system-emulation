#ifndef _FILESYS_H_
#define _FILESYS_H_

#include <stdio.h>

#define BLOCKSIZ  512  //每块大小
#define SYSOPENFILE 40  //系统打开文件表最大项数
#define DIRNUM  128  //每个目录所包含的最大目录项数（文件数）
#define DIRSIZ  14  //每个目录项名字部分所占字节数，另加i节点号2个字节
#define PWDSIZ   12  //口令字
#define PWDNUM   32  //最多可设32个口令登录
#define NOFILE  20  //每个用户最多可打开20个文件，即用户打开文件最大次数
#define NADDR 10  //每个i节点最多指向10块，addr[0]~addr[9]
#define NADDR_OFF 1	//间接索引的个数
#define NHINO  128  //共128个Hash链表，提供索引i节点（必须为2的幂）
#define USERNUM  10  //最多允许10个用户登录
#define DINODESIZ  32 //每个磁盘i节点所占字节
#define DINODEBLK  32  //所有磁盘i节点共占32个物理块
#define FILEBLK  512  //共有512个目录文件物理块
#define NICFREE  50  //超级块中空闲块数组的最大块数
#define NICINOD  50  //超级块中空闲节点的最大块数
#define DINODESTART 2*BLOCKSIZ  //i节点起始地址
#define DATASTART (2+DINODEBLK)*BLOCKSIZ //目录、文件区起始地址
#define DIEMPTY     00000
#define DIFILE      01000
#define DIDIR       02000
#define UDIREAD     00001 //0000000001
#define UDIWRITE    00002 //0000000010
#define UDIEXICUTE  00004 //0000000100
#define GDIREAD     00010 //0000001010
#define GDIWRITE    00020 //0000010100
#define GDIEXICUTE  00040 //0000101000
#define ODIREAD     00100 //0001100100
#define ODIWRITE    00200 //0011001000
#define ODIEXICUTE  00400 //0110010000
#define READ        1
#define WRITE       2
#define EXICUTE     3
#define DEFAULTMODE 00777 //1100001001
#define IUPDATE     00002
#define SUPDATE     00001
#define FREAD       00001
#define FWRITE      00002
#define FAPPEND     00004
#define DISKFULL    65535
#define SEEK_SET    0

//内存i节点
struct inode {
	struct inode* i_forw;//i节点指针
	struct inode* i_back;//i节点指针
	char i_flag;//磁盘i节点标志
	unsigned int i_ino;//内存i节点号
	unsigned int i_count;//引用计数
	unsigned short di_number;//关联文件数
	unsigned short di_mode;//存储权限
	unsigned short di_uid;//用户id
	unsigned short di_gid;//用户组id
	unsigned short di_size;//文件大小
	unsigned int di_addr[NADDR + NADDR_OFF];//存放文件的物理块
	
};

//磁盘i节点
struct dinode {
	unsigned short di_number;//关联文件数
	unsigned short di_mode;//存储权限
	unsigned short di_uid;//用户id
	unsigned short di_gid;//用户组id
	unsigned long di_size;//文件大小
	unsigned int di_addr[NADDR + NADDR_OFF];//存放文件的物理块号
};

//目录项
struct direct {
	char d_name[DIRSIZ];//目录名称
	unsigned short d_ino;//指向i节点
};
//超级块
struct filsys {//
	unsigned short s_isize;//i索引节点块数
	unsigned long s_fsize;//数据块块数
	unsigned int s_nfree;//空闲块块数
	unsigned short s_pfree;//空闲块指针
	unsigned int s_free[NICFREE];//空闲块堆栈

	unsigned int s_ninode;//空闲i索引节点数
	unsigned short s_pinode;//空闲i索引节点指针
	unsigned int s_inode[NICINOD];//空闲i索引节点数组
	unsigned int s_rinode;//铭记i索引节点

	char s_fmod;//超级块修改标志
};
//用户登录
struct pwd {
	unsigned short p_uid;//用户id
	unsigned short p_gid;//用户组id
	char password[PWDSIZ];//password
};
//目录表
struct dir {
	struct direct direct[DIRNUM];//目录项数组表示目录信息
	int size;//目录项个数
};
//内存i节点头指针
struct hinode {
	struct inode* i_forw;//队首由Hash表hinode中的i_forw指出 
};
//打开文件结构
struct file {
	char f_flag;//文件操作标志
	unsigned int f_count;//引用计数
	struct inode* f_inode;//指向内存i节点的指针
	unsigned long f_off;//文件读写偏移
};

struct user {
	unsigned short u_default_mode;//用户类别
	unsigned short u_uid;//用户id
	unsigned short u_gid;//组id
	unsigned short u_ofile[NOFILE];//用户打开文件表
};

struct hinode hinode[NHINO];
struct dir dir;	//当前目录（在内存中全部读入）
struct file sys_ofile[SYSOPENFILE];	//打开文件结构
struct filsys filsys;	//内存中的超级块
struct pwd pwd[PWDNUM];
struct user user[USERNUM];
FILE* fd;
struct inode* cur_path_inode;
int    user_id;
struct inode* iget(unsigned int dinodeid);
void iput(struct inode* pinode);
extern unsigned int balloc();
extern void  bfree(unsigned int block_num);
extern struct inode* ialloc();//i节点分配
extern void ifree(unsigned dinodeid);//i节点释放
extern unsigned int namei();
extern unsigned short iname();
extern unsigned int access();
extern void   _dir();
extern void   mkdir();
extern void   chdir();
extern unsigned short aopen();
extern int   creat();
extern unsigned int read();
extern unsigned int write();
int    login();
int  logout();
void   install();
void   format();
extern void   close();
extern void   halt();
extern void   delete_f();//删除文件函数delete_f
char* GetFilename(char* p);//获取文件名
#endif // !_FILESYS_H_