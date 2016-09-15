LIBS = -lcrypt -lhiredis -ljson-c -lcurl
CFLAGS = -std=gnu99

OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
OBJSHARED = $(patsubst %.c,%.o,$(wildcard ../shared/*.c))
all: libnss_http.so.2


%.o: %.c
	gcc -o $@ -fPIC -shared -c $(CFLAGS) -Wall $(AFLAGS) -DAPP_NAME='"nss_http"' $<

../shared/%.o: ../shared/%.c
	gcc -o $@ -fPIC -shared -c $(CFLAGS) -Wall $(AFLAGS) -DAPP_NAME='"nss_http"' $<

libnss_http.so.2: $(OBJ) $(OBJSHARED)
	gcc -fPIC -shared -Wl,-soname,libnss_http.so.2 $(LIBS) -o $@ $(OBJ) $(OBJSHARED)

clean:
	rm -rf ../shared/*.o
	rm -rf *.o
	rm -rf libnss_http.so.2

uninstall:
	sudo rm -rf /lib/libnss_http.so.2

install: all
	sudo install -m 755 libnss_http.so.2 /lib

prepare:
	apt-get install libhiredis-dev build-essential libjson-c-dev libcurl4-openssl-dev