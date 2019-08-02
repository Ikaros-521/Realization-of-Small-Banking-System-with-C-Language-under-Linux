#ifndef STRUCT_H
#define STRUCT_H

typedef struct Account
{
	long user;//帐号
	char name[20];//姓名
	char id[20];//身份证号
	char pw[20];//密码
	double money;//金额
}Account;

typedef struct Msg
{
	long type;
	int flag;
	Account acc;
}Msg;

#endif//STRUCT_H

