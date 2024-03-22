CC=gcc
FLAGS=-Wall -Werror
OBJS=mtll.o main.o
TARGET=mtll

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

tests: $(TARGET)
	echo "Make my tests!"
	chmod u+x test.sh

gdb:
	$(CC) $(FLAGS) -o -g *.c
	gdb ./*

asan:
	clang -fsanitize=address -g -o mtll main.c mtll.c
	./mtll

.PHONY:
run_tests: tests
	echo "Run my tests!"
	./test.sh 

.PHONY:
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
