CC = g++ -fopenmp
IDIR = ./include
CFLAGS = -c -Wall -std=c++11 -I $(IDIR)
OBJDIR = ./obj/

objects = $(addprefix $(OBJDIR), main.o Sequence.o Word.o misc.o calc_matches.o parser.o pattern.o patternset.o rasbcomp.o rasbhari.o rasbimp.o rasbopt.o sensmem.o speedsens.o parameters.o sw_parser.o)

Debug: all
all: protfswm

protfswm: $(objects)
	mkdir -p bin/Debug
	$(CC) -Wall -o bin/Debug/protfswm $(objects)
	mkdir -p ~/bin
	cp bin/Debug/protfswm ~/bin/protfswm

$(OBJDIR)main.o: main.cpp
	mkdir -p obj
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

$(OBJDIR)pattern.o: src/pattern.cpp
	$(CC) $(CFLAGS) src/pattern.cpp -o $@

$(OBJDIR)patternset.o: src/patternset.cpp
	$(CC) $(CFLAGS) src/patternset.cpp -o $@

$(OBJDIR)rasbcomp.o: src/rasbcomp.cpp
	$(CC) $(CFLAGS) src/rasbcomp.cpp -o $@

$(OBJDIR)rasbhari.o: src/rasbhari.cpp
	$(CC) $(CFLAGS) src/rasbhari.cpp -o $@

$(OBJDIR)rasbimp.o: src/rasbimp.cpp
	$(CC) $(CFLAGS) src/rasbimp.cpp -o $@

$(OBJDIR)rasbopt.o: src/rasbopt.cpp
	$(CC) $(CFLAGS) src/rasbopt.cpp -o $@

$(OBJDIR)sensmem.o: src/sensmem.cpp
	$(CC) $(CFLAGS) src/sensmem.cpp -o $@

$(OBJDIR)speedsens.o: src/speedsens.cpp
	$(CC) $(CFLAGS) src/speedsens.cpp -o $@

$(OBJDIR)parameters.o: src/parameters.cpp
	$(CC) $(CFLAGS) src/parameters.cpp -o $@

$(OBJDIR)sw_parser.o: src/sw_parser.cpp
	$(CC) $(CFLAGS) src/sw_parser.cpp -o $@

clean:
	rm -rf $(OBJDIR)*.o bin/Debug/protfswm	~/bin/protfswm




