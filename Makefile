CC = g++
IDIR = ./include
CFLAGS = -c -Wall -std=c++11 -I $(IDIR)
OBJDIR = ./obj/

objects = $(addprefix $(OBJDIR), main.o score.o Sequence.o Word.o rand_pattern.o)

Debug: all
all: afpd

afpd: $(objects)
	$(CC) -Wall -o bin/Debug/afpd $(objects)
	cp bin/Debug/afpd ~/bin/

$(OBJDIR)main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o $@

$(OBJDIR)score.o: src/score.cpp
	$(CC) $(CFLAGS) src/score.cpp -o $@

$(OBJDIR)Sequence.o: src/Sequence.cpp
	$(CC) $(CFLAGS) src/Sequence.cpp -o $@

$(OBJDIR)Word.o: src/Word.cpp
	$(CC) $(CFLAGS) src/Word.cpp -o $@

$(OBJDIR)rand_pattern.o: src/rand_pattern.cpp
	$(CC) $(CFLAGS) src/rand_pattern.cpp -o $@

clean:
	rm -rf $(OBJDIR)*.o bin/Debug/afpd	
	



