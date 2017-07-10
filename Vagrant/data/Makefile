SRCFOLDER=src/
CLIENTNAME=daemontify
SERVERNAME=daemonwatch
BUILD=build/

SERVICEPATH=/etc/init.d/
SERVICEFOLDER=init.d/
DAEMONPATH=/usr/local/bin/

all: installInotify installInotifyServer

installInotify: inotify
	cp $(SERVICEFOLDER)clientd $(SERVICEPATH)$(CLIENTNAME)
	chmod 755 $(SERVICEPATH)$(CLIENTNAME)
	cp $(BUILD)$(CLIENTNAME) $(DAEMONPATH)$(CLIENTNAME)

inotify:
	gcc $(SRCFOLDER)$(CLIENTNAME).c -o $(BUILD)$(CLIENTNAME)

daemonwatch:
	gcc -pthread $(SRCFOLDER)$(SERVERNAME).c -o $(BUILD)$(SERVERNAME)

installInotifyServer: inotify daemonwatch
	cp $(BUILD)$(SERVERNAME) $(DAEMONPATH)$(SERVERNAME)
	cp $(SERVICEFOLDER)serverd $(SERVICEPATH)$(SERVERNAME)
	chmod 755 $(SERVICEPATH)$(SERVERNAME)
