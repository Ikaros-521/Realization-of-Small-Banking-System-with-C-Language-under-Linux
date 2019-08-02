#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include "struct.h"
#include "tools.h"
//#include "msg_queue.c"

static long online = 0;
static int lock = 0;

int msgid_ctos;
int msgid_stoc;

int init();
void open_acc();
void destory();
void login();
void unlock();
void user_menu(void);
void menu(void);
void save();
void take();
void transf();
void select_money();
void change_pw();

int main()
{
	init();
	menu();
}

int init()
{
	//msgctl(msgid_ctos,IPC_RMID,NULL);
	// 创建消息队列
	msgid_ctos = msgget(ftok(".",100),0);
	if(0 > msgid_ctos)
	{
		perror("msgget");
		return -1;
	}
	msgid_stoc = msgget(ftok(".",200),0);
	if(0 > msgid_stoc)
	{
		perror("msgget");
		return -1;
	}
	return 0;
}

void login()//0
{
	if(lock >= 3)
	{
		printf("您已被锁定，请解锁\n");
		getchar();
		return;
	}
	Account acc = {};
	Msg msg = {110};
	printf("请输入帐号：");
	scanf("%ld",&acc.user);
	getchar();
	printf("请输入密码：");
	get_str(acc.pw,20);
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),220,MSG_NOERROR);
	
	if(acc.user == msg2.acc.user)
	{
		online = acc.user;
		printf("%ld,登陆成功\n",online);
		getch();
		user_menu();
	}
	else
	{	
		lock++;	
		printf("帐号或密码错误\n");
		getch();
	}
}

void open_acc()//1
{

	Account acc = {};
	Msg msg = {111};
	printf("请输入姓名：");
	get_str(acc.name,20);
	printf("请输入身份证：");
	get_str(acc.id,20);
	printf("请输入密码：");
	get_str(acc.pw,20);
	msg.acc = acc;
	//printf("msg:%s\n",msg.acc.name);
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),221,MSG_NOERROR);
	printf("msg2:%s,acc:%s\n",msg2.acc.name,acc.name);
	if(strcmp(acc.name,msg2.acc.name) == 0)
	{
		//printf("%s\n",msg2.acc.name);
		//printf("%s\n",msg2.acc.id);
		printf("帐号:%ld,开户成功\n",msg2.acc.user);
	}
	else
	{		
		printf("开户失败\n");
	}
	getch();
}

void save()//2
{
	Account acc = {};
	Msg msg = {112};
	//printf("当前用户:%ld\n",online);
	printf("请输入存款金额:");
	scanf("%lf",&acc.money);
	getchar();
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),222,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("存钱成功\n");
	}
	else
	{
		printf("存钱失败\n");
	}
	getch();
}

void take()//3
{
	Account acc = {};
	Msg msg = {113};
	printf("请输入取款金额:");
	scanf("%lf",&acc.money);
	getchar();
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),223,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("取钱成功\n");
	}
	else if(msg2.acc.user == online + 2)
	{
		printf("余额不足，请充值\n");
	}
	else
	{
		printf("取钱失败\n");
	}
	getch();
}
void transf()//4
{
	Account acc = {};
	Msg msg = {114};
	printf("请输入转帐金额:");
	scanf("%lf",&acc.money);
	getchar();
	acc.user = online;	
	msg.acc = acc;
	msg.flag = 0;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),224,MSG_NOERROR);
	if(msg2.flag == 0)
	{
		printf("请输入要转帐的帐号：");
		scanf("%ld",&acc.user);
		getchar();
		msg.acc = acc;
		msg.flag = 1;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);
		
		// 接收消息
		msgrcv(msgid_stoc,&msg2,sizeof(Msg),224,MSG_NOERROR);
		
		if(msg2.flag == 2)
		{
			printf("转帐成功\n");
		}
		else
		{
			printf("转帐失败\n");
		}
			
	}
	else
	{
		printf("余额不足，请充值\n");
	}
	getch();
}
void select_money()//5
{
	Account acc = {};
	Msg msg = {115};
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),225,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("余额：%.2lf\n",msg2.acc.money);
	}
	else
	{
		printf("查询失败\n");
	}
	getch();
}
void change_pw()//6
{
	Account acc = {};
	Msg msg = {116};
	printf("请输入新密码:");
	get_str(acc.pw,20);
	acc.user = online;	
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),226,MSG_NOERROR);
	if(msg2.acc.user == online)
	{
		printf("改密成功\n");
	}
	else
	{
		printf("改密失败\n");
	}
	getch();
}

void destory()//7
{
	Account acc = {};
	Msg msg = {117};
	printf("请输入销户帐号:");
	scanf("%ld",&acc.user);
	getchar();
	printf("请输入姓名：");
	get_str(acc.name,20);
	printf("请输入身份证：");
	get_str(acc.id,20);
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),227,MSG_NOERROR);
	if(msg2.acc.user == 0)
	{
		printf("销户成功\n");
	}
	else if(msg2.acc.user == 1)
	{
		printf("相关信息错误\n");
	}
	else if(msg2.acc.user == 2)
	{
		printf("帐号不存在\n");
	}
	getch();
}

void unlock()//8
{
	if(lock < 3)
	{
		printf("您没有被锁定，sorry\n");
		getchar();
		return;
	}
	Account acc = {};
	Msg msg = {118};
	printf("请输入帐号：");
	scanf("%ld",&acc.user);
	getchar();
	printf("请输入姓名：");
	get_str(acc.name,20);
	printf("请输入身份证：");
	get_str(acc.id,20);
	msg.acc = acc;
	msgsnd(msgid_ctos,&msg,sizeof(Msg)-sizeof(msg.type),0);

	//printf("消息已发送到队列\n");

	Msg msg2 = {};
	// 接收消息
	msgrcv(msgid_stoc,&msg2,sizeof(Msg),228,MSG_NOERROR);
	
	//printf("已接收到队列的消息\n");
	printf("in:%ld,back:%ld\n",acc.user,msg2.acc.user);
	
	if(acc.user == msg2.acc.user)
	{
		printf("%ld,解锁成功\n",acc.user);
		lock = 0;
		online = acc.user;
		change_pw();
	}
	else
	{	
		printf("相关信息错误\n");
		getch();
	}

/*	long user = 0;
	char id[20] = {};
	char name[20] = {};
	printf("请输入要解锁的帐号:");
	scanf("%ld",&user);
	getchar();

	char path[50] = "./data/";
	char ex[8] = ".dat";
	sprintf(path,"%s%ld%s",path,user,ex);

	if(0 == access(path,F_OK))
	{
		Account buf = {};
		int fd = open(path,O_RDWR);
		if(0 != read(fd,&buf,sizeof(Account)))
		{
			char name[20] = {};
			char id[20] = {};
			printf("请输入此帐号开户姓名:");
			get_str(name,20);
			printf("请输入此帐号开户身份证:");
			get_str(id,20);

			if(strcmp(name,buf.name) == 0 && strcmp(id,buf.id) == 0)
			{
				online = user;
				change_pw();
				lock = 0;
			}
			else
			{
				printf("输入信息有误\n");
				close(fd);
				getch();
			}
		}
		else
		{
			printf("读取信息失败\n");
			getch();
		}
	}
	else
	{
		printf("此帐号不存在\n");
		getch();
	}
*/
}

void user_menu(void)
{
	while(1)
	{
		system("clear");
		printf("---  银行  ---\n");
		printf("1、  存钱\n");
		printf("2、  取钱\n");
		printf("3、  转帐\n");
		printf("4、  查询\n");
		printf("5、  改密\n");
		printf("0、  返回\n");
		switch(get_cmd('0','5'))
		{
			case '1':save(); break;
			case '2':take(); break;
			case '3':transf(); break;
			case '4':select_money(); break;
			case '5':change_pw();break;
			case '0':return;
		}
	}
}

void menu(void)
{
	while(1)
	{
		system("clear");
		printf("---  银行  ---\n");
		printf("1、  开户\n");
		printf("2、  销户\n");
		printf("3、  登陆\n");
		printf("4、  解锁\n");
		printf("0、  退出\n");
		switch(get_cmd('0','4'))
		{
			case '1':open_acc(); break;
			case '2':destory(); break;
			case '3':login(); break;
			case '4':unlock(); break;
			case '0':return;
		}
	}
}
