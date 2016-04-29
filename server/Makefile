all: profzen-server  

profzen-server: profzen-server.o profzen-http.o profzen-writer.o profzen-annotator.o classroom.o\
  			annotator.o writer.o	
	gcc -Wall profzen-server.o profzen-http.o profzen-writer.o profzen-annotator.o\
		classroom.o annotator.o writer.o\
		../lib/libwebsockets.a -lssl -lcrypto -o ../bin/profzen-server	   	

profzen-server.o: profzen-server.c
	gcc -Wall -c profzen-server.c

profzen-http.o: profzen-http.c
	gcc -Wall -c profzen-http.c

profzen-writer.o: profzen-writer.c
	gcc -Wall -c profzen-writer.c

profzen-annotator.o: profzen-annotator.c
	gcc -Wall -c profzen-annotator.c 

classroom.o : classroom.c
	gcc -Wall -c classroom.c

annotator.o: annotator.c
	gcc -Wall -c annotator.c

writer.o: writer.c
	gcc -Wall -c writer.c

clean:
	rm  *.o ../bin/profzen-server
