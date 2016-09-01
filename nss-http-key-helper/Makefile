LIBS = -lcrypt -lhiredis -ljson-c -lcurl
CFLAGS = -std=c99

OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
OBJSHARED = $(patsubst %.c,%.o,$(wildcard ../shared/*.c))
all: nss-http-key-helper


%.o: %.c
	gcc -o $@ -fPIC -shared -c $(CFLAGS) -Wall -DAPP_NAME='"nss-http-key-helper"' $<

../shared/%.o: ../shared/%.c
	gcc -o $@ -fPIC -shared -c $(CFLAGS) -Wall -DAPP_NAME='"nss-http-key-helper"' $<

nss-http-key-helper: $(OBJ) $(OBJSHARED)
	gcc $(LIBS) -o $@ $(OBJ) $(OBJSHARED)

clean:
	rm -rf ../shared/*.o
	rm -rf *.o
	rm -rf nss-http-key-helper

uninstall:
	sudo rm -rf /usr/bin/nss-http-key-helper

install: all
	sudo install -m 755 nss-http-key-helper /usr/bin

prepare:
	apt-get install libhiredis-dev build-essential libjson-c-dev libcurl4-openssl-dev