#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include "msg_queue.c"
//#include "status.h"
#include "struct.h"
#include "tools.h"

double select_money(Account acc)
{
    char path[50] = "./data/";
	char ex[8] = ".dat";
	sprintf(path,"%s%ld%s",path,acc.user,ex);

	Account buf = {};
	int fd = open(path,O_RDWR);
	if(0 != read(fd,&buf,sizeof(Account)))
	{
		printf("现:%.2lf\n",buf.money);
		close(fd);
		return buf.money;
	}
	else
	{
		printf("读取数据丢失\n");
		return -1;
	}
}

int main()
{
	int msgid_ctos = msgget(ftok(".",100),IPC_CREAT|0644);
	if(0 > msgid_ctos)
	{
		perror("msgget");
		return -1;
	}
	int msgid_stoc = msgget(ftok(".",200),IPC_CREAT|0644);
	if(0 > msgid_stoc)
	{
		perror("msgget");
		return -1;
	}

	long user = 0;

	for(;;)
	{
		Msg msg = {};
		// 接收消息
		msgrcv(msgid_ctos,&msg,sizeof(Msg),115,MSG_NOERROR);
		//printf("type:%ld\n",msg.type);
		printf("user:%ld\n",msg.acc.user);
		user = msg.acc.user;
	
		double result = select_money(msg.acc);

		Msg msg2 = {225};
		if(result != -1)
		{
			msg2.acc.user = user;
			msg2.acc.money = result;
		}		
		else
		{
			msg2.acc.user = user + 1;
		}
		msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof	(msg2.type),0);	

		//pause();
	}
	pause();
}
