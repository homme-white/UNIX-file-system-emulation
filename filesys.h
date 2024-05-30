#ifndef _FILESYS_H_
#include <stdio.h>

#define BLOCKSIZ  512  //ÿ���С
#define SYSOPENFILE 40  //ϵͳ���ļ����������
#define DIRNUM  128  //ÿ��Ŀ¼�����������Ŀ¼�������ļ�����
#define DIRSIZ  14  //ÿ��Ŀ¼�����ֲ�����ռ�ֽ��������i�ڵ��2���ֽ�
#define PWDSIZ   12  //������
#define PWDNUM   32  //������32�������¼
#define NOFILE  20  //ÿ���û����ɴ�20���ļ������û����ļ�������
#define NADDR 10  //ÿ��i�ڵ����ָ��10�飬addr[0]~addr[9]
#define NHINO  128  //��128��Hash�����ṩ����i�ڵ㣨����Ϊ2���ݣ�
#define USERNUM  10  //�������10���û���¼
#define DINODESIZ  32 //ÿ������i�ڵ���ռ�ֽ�
#define DINODEBLK  32  //���д���i�ڵ㹲ռ32�������
#define FILEBLK  512  //����512��Ŀ¼�ļ������
#define NICFREE  50  //�������п��п������������
#define NICINOD  50  //�������п��нڵ��������
#define DINODESTART 2*BLOCKSIZ  //i�ڵ���ʼ��ַ
#define DATASTART (2+DINODEBLK)*BLOCKSIZ //Ŀ¼���ļ�����ʼ��ַ
#define DIEMPTY     00000
#define DIFILE      01000
#define DIDIR       02000
#define UDIREAD     00001
#define UDIWRITE    00002
#define UDIEXICUTE  00004
#define GDIREAD     00010
#define GDIWRITE    00020
#define GDIEXICUTE  00040
#define ODIREAD     00100
#define ODIWRITE    00200
#define ODIEXICUTE  00400
#define READ        1
#define WRITE       2
#define EXICUTE     3
#define DEFAULTMODE 00777
#define IUPDATE     00002
#define SUPDATE     00001
#define FREAD       00001
#define FWRITE      00002
#define FAPPEND     00004
#define DISKFULL    65535
#define SEEK_SET    0

//�ڴ�i�ڵ�
struct inode {
	struct inode* i_forw;//i�ڵ�ָ��
	struct inode* i_back;//i�ڵ�ָ��
	char i_flag;//����i�ڵ��־
	unsigned int i_ino;//����i�ڵ��־
	unsigned int i_count;//���ü���
	unsigned short di_number;//�����ļ���
	unsigned short di_mode;//�洢Ȩ��
	unsigned short di_uid;//�û�id
	unsigned short di_gid;//�û���id
	unsigned short di_size;//�ļ���С
	unsigned int di_addr[NADDR];//����ļ��������
};
//����i�ڵ�
struct dinode {
	unsigned short di_number;//�����ļ���
	unsigned short di_mode;//�洢Ȩ��
	unsigned short di_uid;//�û�id
	unsigned short di_gid;//�û���id
	unsigned long di_size;//�ļ���С
	unsigned int di_addr[NADDR];//����ļ���������
};
//Ŀ¼��
struct direct {
	char d_name[DIRSIZ];//Ŀ¼����
	unsigned int d_ino;//ID
};
//������
struct filsys {//
	unsigned short s_isize;//i�����ڵ����
	unsigned long s_fsize;//���ݿ����
	unsigned int s_nfree;//���п����
	unsigned short s_pfree;//���п�ָ��
	unsigned int s_free[NICFREE];//���п��ջ

	unsigned int s_ninode;//����i�����ڵ���
	unsigned short s_pinode;//����i�����ڵ�ָ��
	unsigned int s_inode[NICINOD];//����i�����ڵ�����
	unsigned int s_rinode;//����i�����ڵ�

	char s_fmod;//�������޸ı�־
};
//�û���¼
struct pwd {
	unsigned short p_uid;//�û�id
	unsigned short p_gid;//�û���id
	char password[PWDSIZ];//password
};
//Ŀ¼��
struct dir {
	struct direct direct[DIRNUM];//Ŀ¼�������ʾĿ¼��Ϣ
	int size;//Ŀ¼�����
};
//�ڴ�i�ڵ�ͷָ��
struct hinode {
	struct inode* i_forw;//������Hash��hinode�е�i_forwָ�� 
};
//���ļ��ṹ
struct file {
	char f_flag;//�ļ�������־
	unsigned int f_count;//���ü���
	struct inode* f_inode;//ָ���ڴ�i�ڵ��ָ��
	unsigned long f_off;//�ļ���дָ��
};

struct user {
	unsigned short u_default_mode;//�û����
	unsigned short u_uid;//�û�id
	unsigned short u_gid;//��id
	unsigned short u_ofile[NOFILE];//�û����ļ���
};

extern struct hinode hinode[NHINO];
extern struct dir dir;	//��ǰĿ¼�����ڴ���ȫ�����룩
extern struct file sys_ofile[SYSOPENFILE];	//���ļ��ṹ
extern struct filsys filsys;	//�ڴ��еĳ�����
extern struct pwd pwd[PWDNUM];
extern struct user user[USERNUM];
extern        FILE* fd;
extern struct inode* cur_path_inode;
extern int    user_id;
extern struct inode* iget(unsigned int dinodeid);
extern void iput(struct inode* pinode);
extern unsigned int balloc();
extern void  bfree();
extern struct inode* ialloc();//i�ڵ����
extern void ifree(unsigned dinodeid);//i�ڵ��ͷ�
extern unsigned int namei();
extern unsigned short iname();
extern unsigned int access();
extern void   _dir();
extern void   mkdir();
extern void   chdir();
extern unsigned short aopen();
extern void   creat();
extern unsigned int read();
extern unsigned int write();
extern int    login();
extern void   logout();
extern void   install(); 
extern void   format();
extern void   close();
extern void   halt();
extern void   delete_f();//ɾ���ļ�����delete_f

#endif // !_FILESYS_H_