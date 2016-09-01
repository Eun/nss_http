all:
	cd nss_http && $(MAKE)
	cd nss-http-key-helper && $(MAKE)

install:
	cd nss_http && $(MAKE) install
	cd nss-http-key-helper && $(MAKE) install

clean:
	cd nss_http && $(MAKE) clean
	cd nss-http-key-helper && $(MAKE) clean

remove: uninstall

uninstall:
	cd nss_http && $(MAKE) uninstall
	cd nss-http-key-helper && $(MAKE) uninstall