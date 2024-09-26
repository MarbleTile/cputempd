
PROG=cputempd
CC=clang
LINK=-lsensors

all: cputempd cputempctl enumerate

cputempd: cputempd.c
	${CC} -o $@ $^ ${LINK}

enumerate: enumerate.c
	${CC} -o $@ $^ ${LINK}

cputempctl: cputempctl.c
	${CC} -o $@ $^ -g

.PHONY: journal getps clean

journal:
	journalctl -e

getps:
	ps -efj | head -n 1; ps -efj | grep ${PROG} | grep -v tmux 

clean:
	rm -f cputempd enumerate cputempctl

