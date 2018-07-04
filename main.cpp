#include "OPENGL.h"


int main(int argc, char **argv){

	OpenGL sys("Steering Behavior", 900, 700, 50, 50);
	sys.Run(argc, argv);

	return 0;
}