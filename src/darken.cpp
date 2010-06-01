#include <stdexcept>
#include <iostream>
#include "DarkenManager.h"
#include "GL/freeglut.h"


DarkenManager manager;

void render()
{
	manager.render();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	try 
	{
		manager.init(argc, argv);
		glutDisplayFunc(render);
		glutIdleFunc(idle);
		glutMainLoop();
	} catch (std::exception &e)
	{
		std::string err = e.what();
		std::cout << err.c_str() << std::endl;
	}


  return 0;
}
