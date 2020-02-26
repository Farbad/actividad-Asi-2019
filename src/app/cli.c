
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "utils.h"
#include "client.h"

#define PRACT2
#define MSGFLG

struct msgbuf{
	long cnl;
	char mtext[200];
};

int snd_msg(int id, char *txt)
{
//char buf[256];
struct msgbuf msg;
static int val=0;
	msg.cnl=CNL_SRV;
	sprintf(msg.mtext,"MSG %d<%s>FIN\n",val++,txt);
	printf("TX:<%s>\n",msg.mtext);
	msgsnd(id,&msg,strlen(msg.mtext),0);
}
#if 0
int read_msg(int id)
{
char buf[MAX_BUF];
int n;
struct msgbuf *msg;
	msg= (struct msgbuf *) buf;
	printf("Voy a esperar mensajes en el canal %ld\n",CNL_SRV);
	if((n=msgrcv(id,buf,MAX_BUF,CNL_SRV)) == -1) {
		perror("msgrcv");
		exit(2);
	}
	printf("He recibido el mensaje (%d): <%s>\n", msg->mtext);
	return(n);
}
#endif

int main(int argc,char *argv[])
{
int idq;
	printf("Voy a abrir la cola de mensajes %lx\n",CLAVE);
	if((idq=msgget(CLAVE,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A LA COLA DE MENSAJES");
		exit(1);
	}
	printf("Tengo acceso a la cola de mensajes\n");

	while(1) {
		snd_msg(idq,"Mi primer mensaje");
//		read_msg(idq);
		sleep(2);
	}

}
