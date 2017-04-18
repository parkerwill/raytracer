CC=g++
CFLAGS=-c -Wall -pedantic
LDFLAGS=
SOURCES=main.cpp Geometry/Ellipsoid.cpp Geometry/LightSource.cpp Geometry/Plane.cpp Geometry/Ray.cpp RGBColour.cpp Scene.cpp Geometry/Shape.cpp Geometry/Sphere.cpp Geometry/Vector.cpp Viewport.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ray-tracer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@









