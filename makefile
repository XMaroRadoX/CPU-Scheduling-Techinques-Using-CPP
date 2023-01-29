.ONESHELL:
CC = g++ -g
all: clean binary run
binary:
	$(CC) Scheduler.cpp -o Scheduler
run: binary
	@ bash test.sh
clean:
	rm -f Scheduler