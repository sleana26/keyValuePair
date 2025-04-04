CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS +=

driver: driver.o value.o input.o map.o

driver.o: driver.c map.h value.h input.h

value.o: value.c value.h

input.o: input.c input.h map.h

map.o: map.c map.h value.h

clean:
	rm driver *.o *.gcda *.gcno *.gcov