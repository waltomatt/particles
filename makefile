EXE = dist/particles
SOURCES = src/main.cpp src/game.cpp src/particle.cpp src/emitter.cpp
SOURCES += 

CXXFLAGS += -g

CXXFLAGS += -Ilibs/
CXXFLAGS += -lGL -lglfw

LIBS = libs/imgui.o  -lGL -lGLU -lSOIL -lglfw -ldl -lGLEW

default:
	$(CXX) $(CXXFLAGS) -o $(EXE) $(SOURCES) $(LIBS)