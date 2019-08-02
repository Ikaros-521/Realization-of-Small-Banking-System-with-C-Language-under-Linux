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

long user = 1;//帐号

//将一个新帐号 存到文件中
int createUser(Account acc){
    char filename[40]={"./data/"};
    sprintf(filename,"./data/%ld.dat",user);//为每个帐号建立一个文件
	
	acc.user = user;
	acc.money = 0;

    int fd = open(filename,O_CREAT|O_RDWR|O_EXCL,0644);
    if(fd == -1)
	{
        perror("创建帐户文件失败");
        return -1;
    }
    if(write(fd,&acc,sizeof(Account)) < 0)
	{
        return -1;
    }
    close(fd);
    printf("创建用户成功\n");
    return 0;
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

	for(;;)
	{
		Msg msg = {};
		// 接收消息
		msgrcv(msgid_ctos,&msg,sizeof(Msg),111,MSG_NOERROR);
		//printf("type:%ld\n",msg.type);
		printf("name:%s\n",msg.acc.name);
		printf("id:%s\n",msg.acc.id);
		printf("pw:%s\n",msg.acc.pw);
		printf("user:%ld\n",user);
		char name[20] = {};
		strcpy(name,msg.acc.name);
		//printf("user:%ld\n",msg.acc.user);
		int result = createUser(msg.acc);
	
		//printf("name_hou:%s,msg:\n",name,msg.acc.name);

		Msg msg2 = {221};
		if(result == 0)
		{
			strcpy(msg2.acc.name,name);
			msg2.acc.user = user++;
			msg2.acc.money = 0;
		}		
		else
		{
			char temp[6] = "error";
			strcat(temp,name);
			strcpy(msg2.acc.name,temp);
		}
	
		printf("name:%s,msg2:%s\n",name,msg2.acc.name);

		msgsnd(msgid_stoc,&msg2,sizeof(Msg)-sizeof	(msg2.type),0);	

		//pause();
	}
	pause();
}
