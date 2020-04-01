
LIBS=../libraries
OBJ=app.o
BINS= app1 app2 cli srv cli-fifo srv-fifo monitor

all: ${BINS}
	
app1: app1.o
	gcc app1.o -L ${LIBS} -lutils -pthread -o app1

app2: app2.o
	gcc app2.o -L ${LIBS} -lutils -pthread -o app2

app1.o: app.c
	gcc -c -DPRACT1 -I ../libraries/inc app.c -oapp1.o

app2.o: app.c
	gcc -c -DPRACT2 -I ../libraries/inc app.c -oapp2.o

cli: cli.c client.h
	gcc -I ../libraries/inc cli.c -o cli

cli-fifo: cli.c client.h
	gcc -DFIFO -I ../libraries/inc cli.c -o cli-fifo

srv: srv.c client.h
	gcc -L ${LIBS} -I ../libraries/inc srv.c -lutils -o srv

srv-fifo: srv.c client.h
	gcc -DFIFO -L ${LIBS} -I ../libraries/inc srv.c -lutils -o srv-fifo

monitor: monitor.c client.h
	gcc -DFIFO -L ${LIBS} -I ../libraries/inc monitor.c -lutils -o monitor

clean:
	rm ${OBJ} *.o
	rm ${BINS}
