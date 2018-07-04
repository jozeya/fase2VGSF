#define GLUT_DISABLE_ATEXIT_HACK	
#ifndef OPENGL_H
#define OPENGL_H	

#include <GL/glut.h>
#include <iostream>
#include "GameWorld.h"

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

using namespace std;

class OpenGL{
public:
	char* appName;
	int width;
	int height;
	int ini_window_x;
	int ini_window_y;

	float x_move_mouse, tmp_x_move;
	float y_move_mouse, tmp_y_move;


	GameWorld* p_mGame;

	OpenGL(char*, int, int, int, int);
	OpenGL();
	~OpenGL();

	GLvoid config_Window();
	GLvoid Run(int, char**);
	GLvoid initGL();
	void window_display(void);
	GLvoid window_idle();
	GLvoid window_keys_pressed(unsigned char, int, int);
	GLvoid window_redraw(GLsizei, GLsizei);
	GLvoid window_keyUP(unsigned char, int, int);
	GLvoid callback_special(int, int, int);
	GLvoid callback_motion(int, int);
	GLvoid callback_motion_press(int, int);
	GLvoid callback_mouse(int, int, int, int);
	static OpenGL *Init(char*, int, int, int, int);

private:
	static OpenGL* Instance;
	static void wd_Callback(void);
	static GLvoid wk_Callback(unsigned char, int , int);
	static GLvoid wkUP_Callback(unsigned char, int , int);
	static GLvoid wi_Callback();
	static GLvoid wr_Callback(GLsizei,GLsizei);
	static GLvoid cs_Callback(int, int, int);
	static GLvoid mm_callback(int, int);
	static GLvoid mm_callback_press(int, int);
	static GLvoid mm_callback_mouse(int, int, int, int);
	static GLvoid run_Game();
};

#endif //OPENGL_H