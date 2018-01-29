whatsapp:
	gcc AddressResolver/connectorAR.c AddressResolver/listenerAR.c AddressResolver/addressResolver.c Messenger/messenger.c Messenger/sender.c Messenger/listener.c whatsapp.c -o whatsapp
	gcc NetworkSetup/adhocCFG.c -o .adhocCFG
enableAdHoc:	
	./.adhocCFG
disableAdHoc:
	sudo service network-manager start

uninstall:
	rm contacts resolver .adhocCFG whatsapp
	
