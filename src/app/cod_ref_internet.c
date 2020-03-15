//
//	Javier Abellán. 22 Mayo 2002
//	Programa para demostración del uso de semáforos
//	
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

struct sembuf blk_client[]={{0,-1,0},{1,-1,0},{2,1,0}};
struct sembuf un_blk_client[]={{0,1,0},{1,1,0},{2,-1,0}};
struct sembuf Operacion={0,-1,0};

//
// Esta union hay que definirla o no según el valor de los defines aqui
// indicados.
//        
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// La union ya está definida en sys/sem.h
#else
// Tenemos que definir la union
union semun 
{ 
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif


main()
{
	key_t Clave;
	int Id_Semaforo;
	union semun arg;
	int i=0;

   //
   // Igual que en cualquier recurso compartido (memoria compartida, semaforos
   // o colas) se obtien una clave a partir de un fichero existente cualquiera
   // y de un entero cualquiera. Todos los procesos que quieran compartir este
   // semaforo, deben usar el mismo fichero y el mismo entero.
   //             
	Clave = ftok ("/bin/ls", 33);
	if (Clave == (key_t)-1)
	{
		printf("No puedo conseguir clave de semáforo");
		exit(0);
	}

	//
	// Se obtiene un array de semaforos (10 en este caso, aunque solo se usara
	// uno.
	// El IPC_CREAT indica que lo  cree si no lo está ya
	// el 0600 con permisos de lectura y escritura para el usuario que lance
	// los procesos. Es importante el 0 delante para que se interprete en
	// octal.
	//               
	Id_Semaforo = semget (Clave, 10, 0600 | IPC_CREAT);
	if (Id_Semaforo == -1)
	{
		printf("No puedo crear semáforo");
		exit (0);
	}

	//
	//	Se inicializa el semáforo con un valor conocido. Si lo ponemos a 0,
	//	es semáforo estará "rojo". Si lo ponemos a 1, estará "verde".
	//	El 0 de la función semctl es el índice del semáforo que queremos
	//	inicializar dentro del array de 10 que hemos pedido.
	//
	arg.val = 3;
	semctl (Id_Semaforo, 0, SETVAL, arg);

	//
	//	Para "pasar" por el semáforo parándonos si está "rojo", debemos rellenar
	//	esta estructura.
	//	sem_num es el indice del semáforo en el array por el que queremos "pasar"
	//	sem_op es -1 para hacer que el proceso espere al semáforo.
	//	sem_flg son flags de operación. De momento nos vale un 0.
	//
	//
#if 0
	Operacion.sem_num = 0;
	Operacion.sem_op = -1;
	Operacion.sem_flg = 0;
#endif

	//
	//	Bucle infinito indicando cuando entramos al semáforo y cuándo salimos
	//	de él.
	//	i hace de contador del número de veces que hemos salido del semáforo.
	//
	while (1)
	{
		printf(" Esperando Semáforo %d\n",i);
		//
		//	Se hace la espera en el semáforo. Se le pasa un array de operaciones
		//	y el número de elementos en dicho array. En nuestro caso solo 1.
		//
		semop (Id_Semaforo, &Operacion, 1);
		printf(" Salgo de Semáforo %d \n",i);
		i++;
	}
}
