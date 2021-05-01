PROGRAM = web_server
OBJS = generate_response.o send_response.o receive_request.o
CC = gcc
CFLAGS = -O2

$(PROGRAM): $(PROGRAM).o $(OBJS)
	$(CC) -o $(PROGRAM) $(PROGRAM).o $(OBJS) 

$(OBJS) $(PROGRAM).o: header.h

clean:
	rm -f *.o 