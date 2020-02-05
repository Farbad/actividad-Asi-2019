
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define FILENAME "intercambio"
//#define TRACE 0

int flg=0;

void trap_SIG1(int sig)
{
	printf("He recibido la señal %d\n",sig);
	flg=1;
}

int creat_mess(char *msg)
{
int fd;
#ifdef TRACE
	if( (fd=creat(FILENAME,0666)) == -1) {
#else
	if( (fd=open(FILENAME,O_CREAT|O_TRUNC|O_WRONLY|0666)) == -1) {
#endif

		perror("Error creando fichero:");
		exit(1);
	}
	printf("Fichero creado\n");
	write(fd,msg,strlen(msg));
	close(fd);
}
int child1_work(int pid)
{
char buf[256];
	while(1) {
		printf("Pon una linea:\n");
		gets(buf);
		printf("RECIBIDO:%s\n",buf);
		if(!strcmp(buf,"salir"))
			break;
		creat_mess(buf);
		kill(pid,SIGUSR1);
	}
}

child2_work()
{
char buf[256];
int fd,n;
	if( (fd=open(FILENAME,O_RDONLY|0666)) == -1) {
		perror("Problemas leyendo:");
		exit(3);
	}
	n=read(fd,buf,sizeof(buf))
	close(fd);
	toupper_str(buf);
	printf("MENSAJE TRADUCIDO:<%s>\n",buf);
}

int main(int argc,char *argv[])
{
pid_t pid1,pid2;
int status;

	printf("Estoy encendido....\n");

	if((pid1=fork()) !=0 ) {
		//PADRE
		printf("He creado el hijo 1:%d\n",pid1);
		if((pid2=fork()) !=0 ) {
			printf("He creado el hijo 2:%d\n",pid2);
		} else {
			//HIJO 2
			printf("Soy el hijo 2 y mi hermano es el pid=%d\n",pid1);
		
			child1_work(pid1);
			exit(0);
		}

		wait(&status);
		printf("Ha muerto un hijo\n");
		wait(&status);
		printf("Ha muerto otro hijo\n");
		exit(0);

	} else {
		signal(SIGUSR1,trap_SIG1);
		//HIJO 1
		while(1) {
			printf("Soy el hijo 1:%ld\n",getpid());
			sleep(4);
			if(flg) {
				//Codigo
				child2_work();
				flg=0;
			}
		}
		exit(1);
	}
}
