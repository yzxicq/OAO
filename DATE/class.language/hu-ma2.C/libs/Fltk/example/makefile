


CXX      = $(shell fltk-config --cxx)
DEBUG    = -g
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I.
LDFLAGS  = $(shell fltk-config --use-gl --use-images --ldflags )
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK     = $(CXX)
TARGET = cube
OBJS = e1.o
SRCS = e1.cxx
.SUFFIXES: .o .cxx
%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $<
all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)
$(TARGET): $(OBJS)
e1.o: e1.cxx
clean: $(TARGET) $(OBJS)
	rm -f *.o 2> /dev/null
	rm -f $(TARGET) 2> /dev/null

