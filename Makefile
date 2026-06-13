all: findPath

findPath.o: findPath.cpp
	g++ -O2 -DMACOS -c findPath.cpp -I/usr/X11/include -I./cal3d-0.11.0/src

Basic.o: Basic.cpp
	g++ -O2 -DMACOS -c Basic.cpp -I/usr/X11/include -I./cal3d-0.11.0/src

Environment.o: Environment.cpp
	g++ -O2 -DMACOS -c Environment.cpp -I/usr/X11/include -I./cal3d-0.11.0/src

findPath: findPath.o Environment.o Basic.o
	g++ findPath.o Basic.o Environment.o -o findPath -framework OpenGL -framework GLUT
 

clean:
	rm *.o findPath
