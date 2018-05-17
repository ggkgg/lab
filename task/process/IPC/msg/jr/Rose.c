/* rose.c */

#include "mylib.h"

/*
** this msgbuf is used for sending and receiving
** data, it is a CRITICAL source. we should apply
** mutex or semaphore to protect it.
*/
struct message
{
	long msg_type;
	char msg_text[BUFFER_SIZE];
}msgbuf;


int main(void)
{
	key_t key = Ftok(".", 'a');
	int  qid  = Msgget(key, IPC_CREAT|0666);

	struct message msg;
	pid_t a = fork();

	// 给Jack发消息
	if(a > 0) // parent
	{
		while(1)
		{
			bzero(&msg, sizeof(msg));

			Fgets(msg.msg_text, BUFFER_SIZE, stdin);
			msg.msg_type = R2J;
	
			msgsnd(qid, &msg, strlen(msg.msg_text), 0);
		}
	}

	// 接收Jack的消息
	if(a == 0) // child
	{
		while(1)
		{
			bzero(&msg, sizeof(msg));
			msgrcv(qid, &msg, BUFFER_SIZE, J2R, 0);

			printf("from Jack: %s", msg.msg_text);
		}
	}
}
