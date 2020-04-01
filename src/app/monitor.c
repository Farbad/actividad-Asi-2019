
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

int init_resources(int *ids,int *idm,char **mem)
{
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

int ini_sem(int ids)
{
	return(1);
}

int ini_mem(char *mem)
{
	return(1);
}

int print_registers(struct st_reg *rg)
{
int i;
	for(i=0;i<MAX_REG;i++) {
		printf("REGISTRO %d: STATUS %s",i+1,rg[i].st ? "OCUPADO" : "LIBRE" );
		if(rg[i].st)
			printf(" PID: %ld",rg[i].pid);

		printf("\n");
	}
	return(1);
}

int print_counters(int *tbl_int)
{
int i;
	for(i=0;i<MAX_CNT;i++)
		printf("El contador %d es %d\n",i+1,tbl_int[i]);
	return(1);
}

int print_semvalues(int ids)
{
int i;
int val;

	for(i=0;i<NSEMS;i++) {
		val = semctl(ids,i,GETVAL);
		printf("El semaforo %d está a valor: %d\n",i+1,val);
	}
	return(1);
}

int take_snap(int ids,char *mem)
{
	print_counters((int *) (mem+DSP_CNT));
	print_registers((struct st_reg *)(mem+DSP_REG));
	print_semvalues(ids);
	return(1);
}

int main(int argc,char *argv[])
{
int idm,ids;
char *mem;
int lapsus=1;

	if(init_resources(&ids,&idm,&mem) == -1) {
		perror("PRoblemas al inicializar recursos");
		exit(1);
	}

	ini_mem(mem);

	ini_sem(ids);

	if(argc == 1) {
		take_snap(ids,mem);
	} else {
		if(atoi(argv[1]) > 1)
			lapsus = atoi(argv[1]);
		while(1) {
			take_snap(ids,mem);
			sleep(lapsus);
		}
	}
}
