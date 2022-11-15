.ONESHELL:
CC = g++ -g
all: clean binary 
binary:
	$(CC) lab.cpp -o lab4
run: binary
	@ ./lab4
clean:
	rm -f lab4