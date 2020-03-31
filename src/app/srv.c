
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
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

#include <utils.h>
#include "client.h"

//#define TRACE 0
//

#define PRACT2

union semun {
	int val;
};

int snd_msg(int id, long cnl, char *txt)
{
#ifdef FIFO
	write(id,txt,strlen(txt));
#else
char buf[256];
char *mn;
	*((long *)buf)=cnl;
	mn = buf + sizeof(long);
	mn += sprintf(mn,"%s",txt);
	printf("TX(%ld):<%s>\n",*((long *)buf),txt);
	msgsnd(id,buf,mn-buf-sizeof(long),0);
#endif
	return(1);
}

int read_msg(int id)
{
char buf[MAX_BUF];
int n;
#ifdef FIFO
	if((n=read(id,buf,sizeof(buf))) == -1) {
		perror("read");
		exit(2);
	}
	if(!n) return(0);
	buf[n]=0;
	printf("RX:<%s>\n",toupper_str(buf));
	
#else
char *mn;
long *pid;
	printf("Voy a esperar mensajes en el canal %ld\n",CNL_SRV);
	if((n=msgrcv(id,buf,MAX_BUF,CNL_SRV,0)) == -1) {
		perror("msgrcv");
		exit(2);
	}
	buf[n+sizeof(long)]='\0';

	pid=(long *)(buf+sizeof(long));
	mn= buf+2*sizeof(long);
	printf("He recibido el mensaje (%ld): <%s>\n", *((long *)buf),mn);
	toupper_str(mn);
	snd_msg(id,*pid,mn);
#endif
	return(n);
}

int init_resources_srv(int *idq,int *ids,int *idm,char **mem)
{
#ifdef FIFO
	printf("Voy a abrir el dispositivo FIFO %s\n",FIFONAME);
	if((*idq=open(FIFONAME,O_RDONLY))== -1) {
		perror("Error en FIFO:");
		return(-1);
	}
	printf("Tengo acceso al fifo\n");
#else
	printf("Voy a abrir la cola de mensajes %lx\n",CLAVE);
	if((*idq=msgget(CLAVE,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A LA COLA DE MENSAJES");
		return(-1);
	}
	printf("Tengo acceso a la cola de mensajes\n");
#endif

	printf("Voy a abrir la memoria compartida %lx\n",CLAVE);
	if((*idm=shmget(CLAVE,SIZE_SHM,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A LA MEMORIA COMPARTIDA");
		return(-1);
	}
	*mem = (char *)shmat(*idm,NULL,0);
	printf("Tengo acceso a la memoria compartida mem=%p\n",*mem);

	printf("Voy a abrir los semaforos %lx\n",CLAVE);
	if((*ids=semget(CLAVE,NSEMS,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A SEMAFOROS");
		return(-1);
	}
	printf("Tengo acceso al semaforo\n");
	return(1);
}

int main(int argc,char *argv[])
{
int idq,idm,ids;
char *mem;
union semun ini={.val=3};

	if(init_resources_srv(&idq,&ids,&idm,&mem) == -1) {
		perror("PRoblemas al inicializar recursos");
		exit(1);
	}

	sprintf(mem,"Hola que tal estais. Soy el servidor %ld\n",(long)getpid());
	*((int *)(mem+320))=100;
	sprintf(mem+328,"Escribo el valor 200");
	semctl(ids,0,SETVAL,ini);
	while(1) {
		read_msg(idq);
	}
}
