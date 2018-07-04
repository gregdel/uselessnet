all:
	gcc -std=gnu18 *.c -o uselessnet -g -O0

cscope:
	cscope -cqbR

clean:
	rm -f *.o cscope.* uselessnet
