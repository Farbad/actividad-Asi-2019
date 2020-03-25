
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
#include <sys/shm.h>
#include <sys/sem.h>

#include <utils.h>

#include "client.h"

#define PRACT2
#define MSGFLG

struct sembuf blk_cli[]={{0,-1,0},{1,1,0}};
struct sembuf unblk_cli[]={{0,1,0},{1,-1,0}};
struct sembuf blk_read[]={{2,-1,0}};
struct sembuf unblk_read[]={{2,1,0}};

char *msg_lst[MAX_MSG]={
	"Primer mensaje",
	"segundo mensaje",
	"tercer mensaje",
	"cuarto mensaje",
	"quinto mensaje",
	"sexto mensaje",
	"septimo mensaje",
};

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

actualizar_contador(int *cnt, int ids)
{
int val2;
/***/
//	semop(ids,blk_read,1);
	printf("El contador mensajes=%d\n",val2=*cnt);
	sleep(1);
	*cnt=val2+1;
	printf("El contador lo pongo mensajes=%d\n",*cnt);
	sleep(1);
	printf("El contador queda mensajes=%d\n",*cnt);
//	semop(ids,unblk_read,1);
/****/
}

int main(int argc,char *argv[])
{
int pid_hijo;
int idq,ids,idm;
char buf[256];
char *mem;
int val;
int *tbl_cnt;
int i,status;
#ifdef FIFO
	printf("Voy a abrir el dispositivo FIFO %s\n",FIFONAME);
	if( getpid() == CNL_SRV) {
		perror("Coincide con el canla del servidor\n");
		if((pid_hijo=fork())) {
			waitpid(pid_hijo,&status,0);
			exit(0);
		} 
	}
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

	printf("Voy a abrir la memoria compartida %lx\n",CLAVE);
	if((idm=shmget(CLAVE,SIZE_SHM,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A LA MEMORIA COMPARTIDA");
		exit(1);
	}
	mem = (char *)shmat(idm,NULL,0);
	printf("Tengo acceso a la memoria compartida mem=%p\n",mem);

	printf("Voy a abrir los semaforos %lx\n",CLAVE);
	if((ids=semget(CLAVE,NSEMS,IPC_CREAT | 0666)) == -1) {
		perror("ACCESO A SEMAFOROS");
		exit(1);
	}
	printf("Tengo acceso al semaforo\n");

	printf("Puedo entrar?......\n");
	semop(ids,blk_cli,1);
	printf("Gracias...\n");
	printf("EL mensaje de bienvenida es:<%s>\n",mem);
	val = *((int *)(mem+320));
	printf("El numero secreto es:%d\n",val);
	printf("EL mensaje ASCII es: <%s>\n",mem+328);
	tbl_cnt = (int *)(mem+DSP_CNT);

	while(argc==1) {
		printf("Escribe el mensaje a enviar:\n");
		fgets(buf,sizeof(buf),stdin);
		printf("La entrada de teclado es:<%s>\n",buf);
		if(!strcmp("quit\n",buf)) {
			break;
		}
		
		actualizar_contador(tbl_cnt,ids);
		snd_msg(idq,buf);
		printf("Voy a esperar la respuesta\n");
		read_msg(idq);
	}
	while(argc!=1) {
		for(i=0;i<MAX_MSG;i++) {
			actualizar_contador(&tbl_cnt[0],ids);
			snd_msg(idq,msg_lst[i]);
			sleep(1);
		}
	}
	printf("Voy a salir en 2 segundos\n");
	sleep(2);
	semop(ids,unblk_cli,1);

}
