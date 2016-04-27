all: profzen-server  

profzen-server: profzen-server.o profzen-http.o profzen-writer.o profzen-annotator.o  
	gcc -Wall profzen-server.o profzen-http.o profzen-writer.o profzen-annotator.o\
		classroom.o\
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

clean:
	rm  *.o ../bin/profzen-server
