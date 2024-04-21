CC = gcc
CFLAGS = -Wall -g
DEPS = meal.h
OBJ = kitchen.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kitchen: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lpthread

.PHONY: clean

clean:
	rm -f *.o kitchen