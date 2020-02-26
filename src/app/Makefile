
LIBS=../libraries
OBJ=app.o
BINS= app

all: app
	
app1: app1.o
	gcc app1.o -L ${LIBS} -lutils -pthread -o app

app2: app2.o
	gcc app2.o -L ${LIBS} -lutils -pthread -o app

app1.o: app.c
	gcc -c -DPRACT1 -I ../libraries/inc app.c -oapp1.o

app2.o: app.c
	gcc -c -DPRACT2 -I ../libraries/inc app.c -oapp2.o

clean:
	rm ${OBJ}
	rm ${BINS}
