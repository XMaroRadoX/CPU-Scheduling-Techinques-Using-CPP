.ONESHELL:
CC = g++ -g
all: clean binary run
binary:
	$(CC) lab.cpp -o lab4
run: binary
	@ ./lab4
clean:
	rm -f lab4