
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

//#define TRACE 0
//

#define PRACT2
#define MSGFLG

struct msgbuf{
	long cnl;
	char mtext[200];
};

int read_msg(int id)
{
char buf[MAX_BUF];
int n;
struct msgbuf *msg;
	msg= (struct msgbuf *) buf;
	printf("Voy a esperar mensajes en el canal %ld\n",CNL_SRV);
	if((n=msgrcv(id,buf,MAX_BUF,CNL_SRV,0)) == -1) {
		perror("msgrcv");
		exit(2);
	}
	printf("Recibido\n");
	printf("He recibido el mensaje (%d): <%s>\n", msg->mtext);
	return(n);
}

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
		read_msg(idq);
	}

}
