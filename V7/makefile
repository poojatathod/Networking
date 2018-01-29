whatsapp:
	gcc messenger.c sender.c listener.c  -o whatsapp
	gcc NetworkSetup/adhocCFG.c -o .adhocCFG
enableAdHoc:	
	./.adhocCFG
disableAdHoc:
	sudo service network-manager start

uninstall:
	rm .adhocCFG whatsapp
	
