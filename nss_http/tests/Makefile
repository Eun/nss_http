LIBS = -lcrypt -lhiredis -ljson-c -lcurl
CFLAGS = -std=gnu99

SRC = $(wildcard ../*.c)
SRCSHARED = $(wildcard ../../shared/*.c)

all: $(patsubst %.c,%.out,$(wildcard *.c))

%.out: %.c
	gcc -g -o $@ -Wall $(CFLAGS) $(AFLAGS) $(LIBS) $< $(SRC) $(SRCSHARED) -DAPP_NAME='"nss-http-$@"'
	./$@

clean:
	rm -rf *.out
