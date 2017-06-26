CC = g++ -fopenmp
IDIR = ./include
CFLAGS = -c -Wall -std=c++11 -I $(IDIR)
OBJDIR = ./obj/

objects = $(addprefix $(OBJDIR), main.o Sequence.o Word.o misc.o calc_matches.o parser.o)

Debug: all
all: afpd

afpd: $(objects)
	$(CC) -Wall -o bin/Debug/afpd $(objects)
	cp bin/Debug/afpd ~/bin/afpd

$(OBJDIR)main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o $@

$(OBJDIR)Sequence.o: src/Sequence.cpp
	$(CC) $(CFLAGS) src/Sequence.cpp -o $@

$(OBJDIR)Word.o: src/Word.cpp
	$(CC) $(CFLAGS) src/Word.cpp -o $@

$(OBJDIR)misc.o: src/misc.cpp
	$(CC) $(CFLAGS) src/misc.cpp -o $@

$(OBJDIR)calc_matches.o: src/calc_matches.cpp
	$(CC) $(CFLAGS) src/calc_matches.cpp -o $@

$(OBJDIR)parser.o: src/parser.cpp
	$(CC) $(CFLAGS) src/parser.cpp -o $@

clean:
	rm -rf $(OBJDIR)*.o bin/Debug/afpd	~/bin/afpd




