CFLAGS = -Wextra
FRAMEWORKS = OpenGL
LIBS = -lglfw3 -lGLEW
INCLUDE = -I../include
TARGET = ../bin/test
OBJDIR = ./obj
SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

$(TARGET) : $(OBJECTS) 
	g++ $(INCLUDE) -o $@ $^ -framework $(FRAMEWORKS) $(LIBS) 

$(OBJDIR)/%.o : %.cpp
	g++ $(CFLAGS) $(INCLUDE) -framework $(FRAMEWORKS) $(LIBS) -o $@ -c $^

all : clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
