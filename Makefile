CFLAGS += -O4

rspell : rspell.o
	$(CC) -o rspell rspell.o $(CFLAGS) $(LDFLAGS)

clean :
	rm rspell rspell.o

install : rspell
	install rspell /usr/bin
