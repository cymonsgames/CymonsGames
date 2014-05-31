CPPFLAGS = -Wall -pipe
LIBS = -lncurses
OBJDIR = build
OBJECTS = main.o map.o crate.o
OBJECTS := $(addprefix ${OBJDIR}/,${OBJECTS})

tetrisrl: $(OBJECTS)
	g++ $(CPPFLAGS) $(LIBS) $(OBJECTS) -o tetrisrl

${OBJDIR}/%.o : %.cpp
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi #create directory if it doesn't exist
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) tetrisrl
