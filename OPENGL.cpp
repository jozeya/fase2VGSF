#include "OPENGL.h"

OpenGL* OpenGL::Instance = 0;

OpenGL::OpenGL(char* _appName, int _width, int _height, int posx_ini, int posy_ini):
																					appName(_appName), 
																					width(_width), 
																					height(_height), 
																					ini_window_x(posx_ini), 
																					ini_window_y(posy_ini)
{
	p_mGame = 0;
}

OpenGL::OpenGL(){
	p_mGame = 0;
}

OpenGL::~OpenGL(){}

GLvoid OpenGL::config_Window(){

	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (width, height);
	glutInitWindowPosition (width/2,height/2);
	glutCreateWindow(appName);

}

GLvoid OpenGL::initGL(){

	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightPos0[] = { -10.0f, -100.0f, -10.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	GLfloat lightColor0_diffuse[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0_diffuse);
	
	GLfloat lightColor0_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0_specular);

	GLfloat lightColor0_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0_ambient);
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_NORMALIZE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);
	
	glClearColor(0.4f, 0.4f, 0.4f, 0.2f); 

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat roofAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roofAmbient);

	GLfloat roofAmbient1[] = {0.8f, 0.8f, 0.8f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roofAmbient1);

	GLfloat roofAmbient2[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roofAmbient2);

	GLfloat roofAmbient3[] = {128};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, roofAmbient3);
}

GLvoid OpenGL::Run(int argc, char** argv){

	glutInit(&argc, argv);
	config_Window();
	initGL();

	Init(appName, width, height, ini_window_x, ini_window_y);

	glutDisplayFunc(OpenGL::wd_Callback);
	glutReshapeFunc(OpenGL::wr_Callback);
	glutKeyboardFunc(OpenGL::wk_Callback);
	glutKeyboardUpFunc(OpenGL::wkUP_Callback);

	glutSpecialFunc(OpenGL::cs_Callback);
	glutIdleFunc(OpenGL::wi_Callback);
	//glutPassiveMotionFunc(&mm_callback);
	glutMouseFunc(&mm_callback_mouse);
	glutMotionFunc(&mm_callback_press);
	
	glClearColor(0.22, 0.20, 0.2, 1.f);
	glutMainLoop();
}

void OpenGL::window_display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();

	gluPerspective(80.0f, 1.0f, 0.01f, 1000.0f);


	glRotatef(x_move_mouse, 0.0, 1.0, 0.0);
	glRotatef(y_move_mouse, 1.0, 0.0, 0.0);

	Vector2D pos = p_mGame -> m_Vehicles[0] -> getPos();
	int x_pos = pos.x;
	int y_pos = pos.y;

	gluLookAt(x_pos, y_pos, 300-0.8, x_pos, y_pos, 300, 1.0, 0.0, 0.0);

	run_Game();

	glutSwapBuffers();

	glFlush();

}

GLvoid OpenGL::window_redraw(GLsizei width, GLsizei height) {

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

}

GLvoid OpenGL::window_keys_pressed(unsigned char key, int x, int y){
	switch(key){
		case 27: // escape
			exit(1); 
			break;
		case 32: //barra espaciadora
			Instance -> p_mGame -> ActivateKey(32);
			//cout << "espaciadora presionado" << endl;
			break;
		case 87: // tecla w
			Instance -> p_mGame -> ActivateKey(87);
			//cout << "W presionado" << endl;
			break;
		case 65: // tecla a
			Instance -> p_mGame -> ActivateKey(65);
			//cout << "A presionado" << endl;
			break;
		case 68: // tecla d
			Instance -> p_mGame -> ActivateKey(68);
			//cout << "D presionado" << endl;
			break;
		case 83: // tecla s
			Instance -> p_mGame -> ActivateKey(83);
			//cout << "D presionado" << endl;
			break;
		default:
			break;
	}
}

GLvoid OpenGL::window_keyUP(unsigned char key, int x, int y){
	switch(key){
		case 27: // escape
			exit(1); 
			break;
		case 32: //barra espaciadora
			Instance -> p_mGame -> DeactivateKey(32);
			//cout << "espaciadora soltado" << endl;
			break;
		case 87: // tecla w
			Instance -> p_mGame -> DeactivateKey(87);
			//cout << "W soltado" << endl;
			break;
		case 65: // tecla a
			Instance -> p_mGame -> DeactivateKey(65);
			//cout << "A soltado" << endl;
			break;
		case 68: // tecla d
			Instance -> p_mGame -> DeactivateKey(68);
			//cout << "D soltado" << endl;
			break;
		case 83: // tecla s
			Instance -> p_mGame -> DeactivateKey(83);
			//cout << "D presionado" << endl;
			break;
		default:
			break;
	}
}

GLvoid OpenGL::window_idle(){
	glutPostRedisplay();
}

GLvoid OpenGL::callback_special(int key, int x, int y){
	if (key == GLUT_KEY_DOWN){
		glutPostRedisplay();			// et on demande le réaffichage.
	}
}

GLvoid OpenGL::callback_motion(int x, int y){
	Vector2D cPointerMouse;

	cPointerMouse = Vector2D(x, height - y);
	p_mGame -> setCrosshair (Vector2D(cPointerMouse));

	glutPostRedisplay();
}

GLvoid OpenGL::callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
		tmp_x_move = x;
		tmp_y_move = y;
	}
}

GLvoid OpenGL::callback_motion_press(int x, int y){
	cout << x << "," << y << endl;

	x_move_mouse += x - tmp_x_move;
	tmp_x_move = x;
	
	y_move_mouse += y - tmp_y_move;
	tmp_y_move = y;
	
	glutPostRedisplay();						
}


OpenGL* OpenGL::Init(char* _appName, int _width, int _height, int posx_ini, int posy_ini){

	if (!Instance)
		Instance = new OpenGL(_appName, _width, _height, posx_ini, posy_ini);

	Instance -> p_mGame = new GameWorld(Instance -> width, Instance -> height);

	return Instance;
}

void OpenGL::wd_Callback(void){
	Instance -> window_display();
}

GLvoid OpenGL::wk_Callback(unsigned char key, int x, int y){
	Instance -> window_keys_pressed(key, x, y);
}

GLvoid OpenGL::wi_Callback(){
	Instance -> window_idle();
}

GLvoid OpenGL::wr_Callback(GLsizei width, GLsizei height){
	Instance -> window_redraw(width, height);
}

GLvoid OpenGL::wkUP_Callback(unsigned char key, int x, int y){
	Instance -> window_keyUP(key,x,y);
}

GLvoid OpenGL::mm_callback(int x, int y){
	Instance -> callback_motion(x,y);
}

GLvoid OpenGL::run_Game(){
	Instance -> p_mGame -> Render();
	Instance -> p_mGame -> Update(0.04);
}

GLvoid OpenGL::cs_Callback(int key, int x, int y){
	Instance -> callback_special(key, x, y);
}

GLvoid OpenGL::mm_callback_press(int x, int y){
	Instance -> callback_motion_press(x, y);	
}

GLvoid OpenGL::mm_callback_mouse(int button, int state, int x, int y){
	Instance -> callback_mouse(button, state, x, y);
}
