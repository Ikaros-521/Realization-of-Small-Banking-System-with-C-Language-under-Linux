#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "struct.h"
#include "tools.h"

static int msgid_ctos;
static int msgid_stoc;
static pid_t sub_pid[9];

void sigint(int sig)
{
	msgctl(msgid_ctos,IPC_RMID,NULL);
	msgctl(msgid_stoc,IPC_RMID,NULL);
	
	int i;
	for(i=0; i<9; i++)
	{
		kill(sub_pid[i],SIGKILL);
	}
}

int main()
{
	signal(SIGINT,sigint);

	msgctl(msgid_ctos,IPC_RMID,NULL);
	msgctl(msgid_stoc,IPC_RMID,NULL);

	msgid_ctos = msgget(ftok(".",100),IPC_CREAT|IPC_EXCL|0644);
	msgid_stoc = msgget(ftok(".",200),IPC_CREAT|IPC_EXCL|0644);
	
	sub_pid[0] = vfork();
	if(0 == sub_pid[0])
	{
		execl("login","login",NULL);
		printf("登陆子进程创建成功...\n");
	}

	sub_pid[1] = vfork();
	if(0 == sub_pid[1])
	{
		execl("open","open",NULL);
		printf("开户子进程创建成功...\n");
	}
	sub_pid[2] = vfork();
	if(0 == sub_pid[2])
	{
		execl("save","save",NULL);
	}
	
	sub_pid[3] = vfork();
	if(0 == sub_pid[3])
	{
		execl("take","take",NULL);
	}
	
	sub_pid[4] = vfork();
	if(0 == sub_pid[4])
	{
		execl("transf","transf",NULL);
	}

	sub_pid[5] = vfork();
	if(0 == sub_pid[5])
	{
		execl("select","select",NULL);
	}

//login open save take transf select change destory unlock

	sub_pid[6] = vfork();
	if(0 == sub_pid[6])
	{
		execl("change","change",NULL);
	}	

	sub_pid[7] = vfork();
	if(0 == sub_pid[7])
	{
		execl("destory","destory",NULL);
	}

	sub_pid[8] = vfork();
	if(0 == sub_pid[8])
	{
		execl("unlock","unlock",NULL);
	}

	//pause();
	while(-1 != wait(NULL));
}

