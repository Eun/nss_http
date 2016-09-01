LIBS = -lcrypt -lhiredis -ljson-c -lcurl
CFLAGS = -std=c99

OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
OBJSHARED = $(patsubst %.c,%.o,$(wildcard ../shared/*.c))
all: nss-custom-data-example


%.o: %.c
	gcc -o $@ -fPIC -shared -c $(CFLAGS) -Wall -DAPP_NAME='"nss-custom-data-example"' $<

../shared/%.o: ../shared/%.c
	gcc -o $@ -fPIC -shared -c $(CFLAGS) -Wall -DAPP_NAME='"nss-custom-data-example"' $<

nss-custom-data-example: $(OBJ) $(OBJSHARED)
	gcc $(LIBS) -o $@ $(OBJ) $(OBJSHARED)

clean:
	rm -rf ../shared/*.o
	rm -rf *.o
	rm -rf nss-custom-data-example

uninstall:
	sudo rm -rf /usr/bin/nss-custom-data-example

install: all
	sudo install -m 755 nss-custom-data-example /usr/bin

prepare:
	apt-get install libhiredis-dev build-essential libjson-c-dev libcurl4-openssl-dev