all: *.cpp ../include/*.h
	g++ -std=c++0x -L/usr/local/lib -lGLEW -lglut -lOpenCL -lSDL -lSDL_image \
	-I/usr/local/include/SDL -I/usr/include/SDL -I../include \
	-DSHADER_LOC=\"glsl/\" -DCL_LOC=\"OpenCL/\" -g *.cpp -o ../cl