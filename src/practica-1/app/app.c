
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
pid_t pid1,pid2;
int status;
char buf[256];

	printf("Estoy encendido....\n");

	if((pid1=fork()) !=0 ) {
		//PADRE
		printf("He creado el hijo 1:%d\n",pid1);
		if((pid2=fork()) !=0 ) {
			printf("He creado el hijo 2:%d\n",pid2);
		} else {
			while(1) {
				printf("Soy el hijo 2:%ld\n",getpid());
				sleep(4);
			}
			exit(1);
			//HIJO 2
		}

		wait(&status);
		printf("Ha muerto un hijo\n");
		wait(&status);
		printf("Ha muerto otro hijo\n");
		exit(0);

	} else {
		//HIJO 1
		printf("Soy el hijo 1 y voy a esperar el teclado\n");
		
		gets(buf);
		exit(0);

	}


}
