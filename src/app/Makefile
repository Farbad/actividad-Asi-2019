
LIBS=../libraries
OBJ=app.o
BINS= app

all: app
	
app: app.o
	gcc app.o -L ${LIBS} -lutils -pthread -o app

app.o: app.c
	gcc -c -I ../libraries/inc app.c -oapp.o

clean:
	rm ${OBJ}
	rm ${BINS}
