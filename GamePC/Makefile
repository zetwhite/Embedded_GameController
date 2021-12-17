
main.exe : main.o util.o 
	gcc -o main.exe main.o util.o 

main.o : main.c util.o 
	gcc -c -o main.o main.c 

util.o : util.c util.h 
	gcc -c -o util.o util.c

clean : 
	del *.o main.exe