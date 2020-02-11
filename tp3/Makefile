CC=gcc
CCFLAGS= -Werror -Wall -pedantic -O3

all: compile install

compile: goes.c sysinfo.c listmod.c web_insmod.c web_rmmod.c
	$(CC) -o ./goes.cgi ./goes.c $(CCFLAGS)
	$(CC) -o ./sysinfo.cgi ./sysinfo.c $(CCFLAGS)
	$(CC) -o ./listmodules.cgi ./listmod.c $(CCFLAGS)
	$(CC) -o winsmod ./web_insmod.c $(CCFLAGS)
	$(CC) -o wrmmod ./web_rmmod.c $(CCFLAGS)

install: goes.cgi sysinfo.cgi lighttpd.conf
	sudo apt-get install lighttpd
	sudo cp ./lighttpd.conf /etc/lighttpd/lighttpd.conf
	sudo service lighttpd restart
	sudo mkdir -p /var/www/cgi-bin
	sudo cp -r ./app /var/www/html/
	sudo cp ./goes.cgi /var/www/cgi-bin/
	sudo cp ./sysinfo.cgi /var/www/cgi-bin/
	sudo cp ./listmodules.cgi /var/www/cgi-bin/
	sudo cp ./file_upload.pl /var/www/cgi-bin/
	sudo cp ./remove_mod.pl /var/www/cgi-bin/
	sudo mkdir -p /var/www/cgi-bin/modules
	sudo chown www-data /var/www/cgi-bin/modules
	sudo cp ./winsmod /var/www/cgi-bin/
	sudo cp ./wrmmod /var/www/cgi-bin/
	sudo chmod a+s /var/www/cgi-bin/winsmod
	sudo chmod a+s /var/www/cgi-bin/wrmmod
	sudo pip3 install awscli
clean:
	rm goes.cgi sysinfo.cgi listmodules.cgi 
