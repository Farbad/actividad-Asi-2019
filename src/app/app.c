
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "utils.h"

#define FILENAME "intercambio"
//#define TRACE 0
//
#define MAX_THR 4

//#define PRACT1
#define PRACT2

int flg=1;
int nthr=0;
int nthrlaunched=0;
int nthrexited=0;

pthread_t pthread[MAX_THR];

void trap_SIG2(int sig)
{
	printf("He recibido la señal %d\n",sig);
	nthr++;
}

void trap_SIG1(int sig)
{
	printf("He recibido la señal %d\n",sig);
	flg=1;
}

#ifdef PRACT1
int creat_mess(char *msg)
{
int fd;
#ifdef TRACE
	if( (fd=creat(FILENAME,0666)) == -1) {
#else
	if( (fd=open(FILENAME,O_CREAT|O_TRUNC|O_WRONLY,0666)) == -1) {
#endif

		perror("Error creando fichero:");
		exit(1);
	}
	printf("Fichero creado\n");
	write(fd,msg,strlen(msg));
	close(fd);
}
#endif

#ifdef PRACT1
int child2_work(int pid)
{
char buf[256];
	while(1) {
		printf("Pon una linea:\n");
		fgets(buf,sizeof(buf),stdin);
		printf("RECIBIDO:%s\n",buf);
		if(!strcmp(buf,"salir"))
			break;
		creat_mess(buf);
		kill(pid,SIGUSR1);
	}
	kill(pid,SIGKILL);
	return(1);
}
#else
int child2_work(int fd)
{
char buf[256];
	while(1) {
		printf("Pon una linea:\n");
		fgets(buf,sizeof(buf),stdin);
		printf("RECIBIDO:%s\n",buf);
		if(!strcmp(buf,"salir"))
			break;
		write(fd,buf,strlen(buf));
	}

}
#endif

#ifdef PRACT1
int child1_work()
{
char buf[256];
int fd,n;
	if( (fd=open(FILENAME,O_RDONLY,0666)) == -1) {
		perror("Problemas leyendo:");
		exit(3);
	}
	n=read(fd,buf,sizeof(buf));
	close(fd);
	buf[n]=0;
	toupper_str(buf);
	printf("MENSAJE TRADUCIDO:<%s>\n",buf);
	return(1);
}
#else
int child1_work(int fd)
{
char buf[256];
int n;
	n=read(fd,buf,sizeof(buf));
	buf[n]=0;
	toupper_str(buf);
	printf("MENSAJE TRADUCIDO:<%s>\n",buf);
	return(1);
}
#endif

void *start_thr(void *arg)
{
	int id;
	printf("Estoy ejecutando el thread %d\n", id=pthread_self());
	printf("Voy a esperar 4 segundos anets de morir %d\n",id);
	sleep(4);
	printf("Acabe mi ejecución %d.\n",id);
}

int main(int argc,char *argv[])
{
pid_t pid1,pid2;
int status;
#ifdef PRACT2
int fd[2];
#endif

	printf("Estoy encendido.... LA SEÑAL SUGUSR2 es:%d\n",SIGUSR2);

#ifdef PRACT2
	pipe(fd);
	printf("Los descriptores del pipe son: %d %d\n",fd[0],fd[1]);
#endif
	if((pid1=fork()) !=0 ) {
		//PADRE
		printf("He creado el hijo 1:%d\n",pid1);
		if((pid2=fork()) !=0 ) {
			printf("He creado el hijo 2:%d\n",pid2);
		} else {
			//HIJO 2
//			printf("Soy el hijo 2 y mi hermano es el pid=%d\n",pid1);
		
#ifdef PRACT1
			child2_work(pid1);
#else
			close(fd[0]);
			child2_work(fd[1]);
#endif
			exit(0);
		}

		wait(&status);
		printf("Ha muerto un hijo\n");
		wait(&status);
		printf("Ha muerto otro hijo\n");
		exit(0);

	} else {
		signal(SIGUSR1,trap_SIG1);
		printf("Preparado para recibir la señal SIGUSR2\n");
		signal(SIGUSR2,trap_SIG2);
		//HIJO 1
		while(1) {
			printf("Soy el hijo 1:%ld\n",getpid());
			sleep(1);
			if(flg) {
				//Codigo
#ifdef PRACT1
				child1_work();
#else
				close(fd[1]);
				printf("SOY CHILD1 %d\n",fd[0]);
				child1_work(fd[0]);
#endif
				flg=0;
			}
			if((nthr > nthrlaunched) && (nthr<MAX_THR)) {
				printf("Lanzando un nuevo thread: %d\n",nthrlaunched+1);
				pthread_create(&pthread[nthr-1],NULL,start_thr,NULL);
				nthrlaunched++;
			}
		}
		exit(1);
	}
}
