
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

#include <utils.h>

#include "client.h"

#define PRACT2
#define MSGFLG

struct msgbuf{
	long cnl;
	char mtext[200];
};

int snd_msg(int id, char *txt)
{
#ifdef FIFO
	write(id,txt,strlen(txt)-1);
#else
char buf[256];
char *mn;
	*((long *)buf)=CNL_SRV;
	mn = buf+sizeof(long);
	*((long *)mn)=getpid();
	mn += sizeof(long);
	printf("TX:<%s>\n",txt);
	mn+=sprintf(mn,"%s",txt);
	msgsnd(id,buf,mn-buf-sizeof(long),0);
#endif
	return(1);
}

int read_msg(int id)
{
char buf[MAX_BUF];
int n;
	printf("Voy a esperar mensajes en el canal %ld\n",getpid());
	if((n=msgrcv(id,buf,MAX_BUF,getpid(),0)) == -1) {
		perror("msgrcv");
		exit(2);
	}
	buf[sizeof(long)+n]=0;
	printf("He recibido el mensaje (%d): <%s>\n",*((long*)buf),
			&buf[sizeof(long)]);
	return(n);
}

int main(int argc,char *argv[])
{
int idq;
char buf[256];
#ifdef FIFO
	printf("Voy a abrir el dispositivo FIFO %s\n",FIFONAME);
	if((idq=open(FIFONAME,O_WRONLY))== -1) {
		perror("Error en FIFO:");
		exit(1);
	}
	printf("Tengo acceso al fifo\n");
#else
	printf("Voy a abrir la cola de mensajes %lx\n",CLAVE);
	if((idq=msgget(CLAVE,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A LA COLA DE MENSAJES");
		exit(1);
	}
	printf("Tengo acceso a la cola de mensajes\n");
#endif

	while(1) {
		printf("Escribe el mensaje a enviar:\n");
		fgets(buf,sizeof(buf),stdin);
		printf("La entrada de teclado es:<%s>\n",buf);
		if(!strcmp("quit\n",buf)) {
			break;
		}
		snd_msg(idq,buf);
		printf("Voy a esperar la respuesta\n");
		read_msg(idq);
	}

}
