CC=gcc
CCFLAGS= -Werror -Wall -pedantic -O3

all: compile install

compile: goes.c sysinfo.c
	$(CC) -o ./goes.cgi ./goes.c $(CCFLAGS)
	$(CC) -o ./sysinfo.cgi ./sysinfo.c $(CCFLAGS)

install: goes.cgi sysinfo.cgi lighttpd.conf
	sudo apt-get install lighttpd
	sudo cp ./lighttpd.conf /etc/lighttpd/lighttpd.conf
	sudo service lighttpd restart
	sudo mkdir -p /var/www/cgi-bin
	sudo cp -r ./app /var/www/html/
	sudo cp ./goes.cgi /var/www/cgi-bin/
	sudo cp ./sysinfo.cgi /var/www/cgi-bin/
	sudo cp ./file_upload.pl /var/www/cgi-bin/
	sudo mkdir -p /var/www/cgi-bin/modules
	sudo chown www-data /var/www/cgi-bin/modules
	sudo pip3 install awscli
clean:
	rm goes.cgi sysinfo.cgi 
