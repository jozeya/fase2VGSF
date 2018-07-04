#include "GameWorld.h"
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>

# include "SteeringBehaviors.h" 
//# include "Bullet.h"


GameWorld::GameWorld(int w, int h): width(w), height(h) {

	Vector2D SpawnPos;
	
	//Vector2D SpawnPos2 = Vector2D(w/2.0, h/2.0);

	//vehicle seek

	SpawnPos = Vector2D(40, height/2.0 - 50);
	//p_mVehicle = new Vehicle(this, SpawnPos, RandFloat()*TwoPi, Vector2D(0.0, 0.0), 1.0, 200.0 * 2.0, 150.0, Pi, 15.0);
	m_Vehicles[0] = new Vehicle(this, SpawnPos, RandFloat()*TwoPi, Vector2D(0.0, 0.0), 0.5, 40.0 * 2.0 , 50.0, TwoPi, 15.0);
	m_Vehicles[0] -> setType(1);
	//m_Vehicles[0] -> Steering() -> ManualWanderOn(0);
	m_Vehicles[0] -> Steering() -> WallAvoidanceOn();

	for (int w = 1; w < AGENTS; w++){
		SpawnPos = Vector2D(width/2.0+RandomClamped()*width/2.0, height/2.0+RandomClamped()*height/2.0);
		m_Vehicles[w] = new Vehicle(this, SpawnPos, RandFloat()*TwoPi, Vector2D(0.0, 0.0), 1.0, 40.0 * 2.0, 50.0, Pi, 15.0);
	}

	/* m_vCrosshair = Vector2D(width/2, height/2);*/

	CreateWalls();


	for (int w = 1; w < AGENTS; w++){
		m_Vehicles[w] -> Steering() -> WanderOn();
		m_Vehicles[w] -> Steering() -> WallAvoidanceOn();
	}
}

GameWorld::GameWorld(){}


GameWorld::~GameWorld(){

}

void GameWorld::ActivateKey(int key){
	m_Vehicles[0] -> KeyPressed(key);

	switch (key){
		case KEY_A:
			m_Vehicles[0] -> Steering() -> ManualWanderOn(20);
			break;
		case KEY_W:
			m_Vehicles[0] -> Steering() -> ManualWanderOn(0);
			break;
		case KEY_D:
			m_Vehicles[0] -> Steering() -> ManualWanderOn(-20);
			break;
		case KEY_S:
			m_Vehicles[0] -> Steering() -> ManualWanderOn(59.7);
			break;
		}
		
}

void GameWorld::DeactivateKey(int key){
	m_Vehicles[0] -> keyUp(key);

	switch (key){
		case KEY_A:
			m_Vehicles[0] -> Steering() -> ManualWanderOff();
			break;
		case KEY_W:
			m_Vehicles[0] -> Steering() -> ManualWanderOff();
			break;
		case KEY_D:
			m_Vehicles[0] -> Steering() -> ManualWanderOff();
			break;
		case KEY_S:
			m_Vehicles[0] -> Steering() -> ManualWanderOff();
			break;
		}
}

void GameWorld::Render(){
	//render walls
	

  	int i = 10;
  	int size = 3, ii = 0;
	double bordersize = 20.0;

  		glPushMatrix();
		glColor3f(0.42, 0.42, 0.42);
		glBegin(GL_QUADS);
			
			glNormal3f(0,0,1);
			glVertex3d(bordersize, height/2 - i * size * 4, 0), // 1)
			glVertex3d(bordersize, height/2 + i * size, 0); // 34
		   	glVertex3d(bordersize, height/2 + i * size, -1000); // 34
		   	glVertex3d(bordersize, height/2 - i * size * 4, -1000), // 1)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize, height/2 - i * size * 4, 0), // 1)
			glVertex3d(bordersize, height/2 - i * size * 4, -1000), // 1)
			glVertex3d(bordersize * i, height/2 - i * size * 4, -1000); // 2
		   	glVertex3d(bordersize * i, height/2 - i * size * 4, 0); // 2
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize, height/2 - i * size * 4, 0), // 1)
			glVertex3d(bordersize, height/2 - i * size * 4, -1000), // 1)
			glVertex3d(bordersize * i, height/2 - i * size * 4, -1000); // 2
		   	glVertex3d(bordersize * i, height/2 - i * size * 4, 0); // 2
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i, height/2 - i * size * 2.4, 0); // 3)
			glVertex3d(bordersize * i, height/2 - i * size * 4, 0); // 2
			glVertex3d(bordersize * i, height/2 - i * size * 4, -1000); // 2
			glVertex3d(bordersize * i, height/2 - i * size * 2.4, -1000); // 3)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 - i * size * 2.4, 0); // 4)
			glVertex3d(bordersize * i, height/2 - i * size * 2.4, 0); // 3)
			glVertex3d(bordersize * i, height/2 - i * size * 2.4, -1000); // 3)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 2.4, -1000); // 4)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 - i * size * 2.4, 0); // 4)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 2.4, -1000); // 4)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 4, -1000); // 5)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 4, 0); // 5)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 - i * size * 4, 0); // 5)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 4, -1000); // 5)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 4, -1000); // 6)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 4, 0); // 6)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3, height/2 - i * size * 4, 0); // 6)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 4, -1000); // 6)
			glVertex3d(bordersize * i * 3, height/2 + i * size, -1000); // 7)
			glVertex3d(bordersize * i * 3, height/2 + i * size, 0); // 7)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3, height/2 + i * size, 0); // 7)
			glVertex3d(bordersize * i * 3, height/2 + i * size, -1000); // 7)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size, -1000); // 8)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size, 0); // 8)		
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2.7, height/2 + i * size, 0); // 8)		
			glVertex3d(bordersize * i * 2.7, height/2 + i * size, -1000); // 8)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, -1000); // 9)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, 0); // 9)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, 0); // 9)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, -1000); // 9)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, -1000); // 10)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, 0); // 10)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, 0); // 9)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, -1000); // 9)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, -1000); // 10)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, 0); // 10)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, 0); // 9)
			glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, -1000); // 9)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, -1000); // 10)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, 0); // 10)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, 0); // 10)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, -1000); // 10)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size, -1000); // 11)
			glVertex3d(bordersize * i * 3.8, height/2 + i * size, 0); // 11)	
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3.8, height/2 + i * size, 0), // 11)	
			glVertex3d(bordersize * i * 3.8, height/2 + i * size, -1000); // 11)
			glVertex3d(bordersize * i * 3.5, height/2 + i * size, -1000); // 12
			glVertex3d(bordersize * i * 3.5, height/2 + i * size, 0); // 12
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3.5, height/2 + i * size, 0); // 12
			glVertex3d(bordersize * i * 3.5, height/2 + i * size, -1000); // 12
			glVertex3d(bordersize * i * 3.5, height/2 - i * size * 4, -1000); // 13)
			glVertex3d(bordersize * i * 3.5, height/2 - i * size * 4, 0); // 13)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3.5, height/2 - i * size * 4, 0); // 13)
			glVertex3d(bordersize * i * 3.5, height/2 - i * size * 4, -1000); // 13)
			glVertex3d(bordersize * i * 4, height/2 - i * size * 4, -1000); // 14)
			glVertex3d(bordersize * i * 4, height/2 - i * size * 4, 0); // 14)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4, height/2 - i * size * 4, 0); // 14)
			glVertex3d(bordersize * i * 4, height/2 - i * size * 4, -1000); // 14)
			glVertex3d(bordersize * i * 4, height/2 - i * size * 6, -1000); // 15)
			glVertex3d(bordersize * i * 4, height/2 - i * size * 6, 0); // 15)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4, height/2 - i * size * 6, 0); // 15)
			glVertex3d(bordersize * i * 4, height/2 - i * size * 6, -1000); // 15)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 6, -1000); // 16)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 6, 0); // 16)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3, height/2 - i * size * 6, 0); // 16)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 6, -1000); // 16)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 5, -1000); // 17)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 5, 0); // 17)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3, height/2 - i * size * 5, 0); // 17)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 5, -1000); // 17)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 5, -1000); // 18
			glVertex3d(bordersize * i * 2, height/2 - i * size * 5, 0); // 18
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 - i * size * 5, 0); // 18
			glVertex3d(bordersize * i * 2, height/2 - i * size * 5, -1000); // 18
			glVertex3d(bordersize * i * 2, height/2 - i * size * 10, -1000); // 19)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 10, 0); // 19)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 - i * size * 10, 0); // 19)
			glVertex3d(bordersize * i * 2, height/2 - i * size * 10, -1000); // 19)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 10, -1000); // 20)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 10, 0); // 20)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3, height/2 - i * size * 10, 0); // 20)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 10, -1000); // 20)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 8, -1000); // 21)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 8, 0); // 21)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 3, height/2 - i * size * 8, 0); // 21)
			glVertex3d(bordersize * i * 3, height/2 - i * size * 8, -1000); // 21)
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 8, -1000); // 22)
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 8, 0); // 22)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 8, 0); // 22)
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 8, -1000); // 22)
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 4, -1000); // 23)
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 4, 0); // 23)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 4, 0); // 23)
			glVertex3d(bordersize * i * 4.3, height/2 - i * size * 4, -1000); // 23)
			glVertex3d(bordersize * i * 4.5, height/2 - i * size * 4, -1000); // 24)
			glVertex3d(bordersize * i * 4.5, height/2 - i * size * 4, 0); // 24)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4.5, height/2 - i * size * 4, 0); // 24)
			glVertex3d(bordersize * i * 4.5, height/2 - i * size * 4, -1000); // 24)
			glVertex3d(bordersize * i * 4.5, height/2 + i * size, -1000); // 25)
			glVertex3d(bordersize * i * 4.5, height/2 + i * size, 0); // 25)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4.5, height/2 + i * size, 0); // 25)
			glVertex3d(bordersize * i * 4.5, height/2 + i * size, -1000); // 25)
			glVertex3d(bordersize * i * 4.2, height/2 + i * size, -1000); // 26)
			glVertex3d(bordersize * i * 4.2, height/2 + i * size, 0); // 26)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4.2, height/2 + i * size, 0); // 26)
			glVertex3d(bordersize * i * 4.2, height/2 + i * size, -1000); // 26)
			glVertex3d(bordersize * i * 4.2, height/2 + i * size * 6, -1000); // 27
			glVertex3d(bordersize * i * 4.2, height/2 + i * size * 6, 0); // 27
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 4.2, height/2 + i * size * 6, 0); // 27
			glVertex3d(bordersize * i * 4.2, height/2 + i * size * 6, -1000); // 27
			glVertex3d(bordersize * i * 2.3, height/2 + i * size * 6, -1000); // 28)
			glVertex3d(bordersize * i * 2.3, height/2 + i * size * 6, 0); // 28)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2.3, height/2 + i * size * 6, 0); // 28)
			glVertex3d(bordersize * i * 2.3, height/2 + i * size * 6, -1000); // 28)
			glVertex3d(bordersize * i * 2.3, height/2 + i * size, -1000); // 29)
			glVertex3d(bordersize * i * 2.3, height/2 + i * size, 0); // 29)	
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2.3, height/2 + i * size, 0); // 29)	
			glVertex3d(bordersize * i * 2.3, height/2 + i * size, -1000); // 29)
			glVertex3d(bordersize * i * 2, height/2 + i * size, -1000); // 30)
			glVertex3d(bordersize * i * 2, height/2 + i * size, 0); // 30)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 + i * size, 0); // 30)
			glVertex3d(bordersize * i * 2, height/2 + i * size, -1000); // 30)
			glVertex3d(bordersize * i * 2, height/2 - i * size, -1000); // 31)
			glVertex3d(bordersize * i * 2, height/2 - i * size, 0); // 31)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i * 2, height/2 - i * size, 0); // 31)
			glVertex3d(bordersize * i * 2, height/2 - i * size, -1000); // 31)
			glVertex3d(bordersize * i, height/2 - i * size, -1000); // 32)
			glVertex3d(bordersize * i, height/2 - i * size, 0); // 32)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i, height/2 - i * size, 0); // 32)
			glVertex3d(bordersize * i, height/2 - i * size, -1000); // 32)
			glVertex3d(bordersize * i, height/2 + i * size, -1000); // 33)
			glVertex3d(bordersize * i, height/2 + i * size, 0); // 33)
		glEnd();

		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glVertex3d(bordersize * i, height/2 + i * size, 0); // 33)
			glVertex3d(bordersize * i, height/2 + i * size, -1000); // 33)
			glVertex3d(bordersize, height/2 + i * size, -1000); // 34)
			glVertex3d(bordersize, height/2 + i * size, 0); // 34)
		glEnd();

	glPopMatrix();

	glPushMatrix();
  		glColor3f(0.81, 0.81, 0.81);
        glBegin(GL_QUADS);
          glVertex3d(bordersize, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i, height/2 + i * size, ii);
          glVertex3d(bordersize, height/2 + i * size, ii);
		glEnd();

		glBegin(GL_QUADS);
          glVertex3d(bordersize * i, height/2 - i * size * 2.4, ii);
          glVertex3d(bordersize * i * 2, height/2 - i * size * 2.4, ii);
          glVertex3d(bordersize * i * 2, height/2 - i * size, ii);
          glVertex3d(bordersize * i, height/2 - i * size, ii);
        glEnd();

        glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 2, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i * 3, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i * 3, height/2 + i * size, ii);
          glVertex3d(bordersize * i * 2, height/2 + i * size, ii);
        glEnd();

		glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 2.3, height/2 + i * size, ii);
          glVertex3d(bordersize * i * 2.7, height/2 + i * size, ii);
          glVertex3d(bordersize * i * 2.7, height/2 + i * size * 4, ii);
          glVertex3d(bordersize * i * 2.3, height/2 + i * size * 4, ii);
        glEnd();

        glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 2.3, height/2 + i * size * 4, ii);
          glVertex3d(bordersize * i * 4.2, height/2 + i * size * 4, ii);
          glVertex3d(bordersize * i * 4.2, height/2 + i * size * 6, ii);
          glVertex3d(bordersize * i * 2.3, height/2 + i * size * 6, ii);
        glEnd();

        glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 3.8, height/2 + i * size, ii);
          glVertex3d(bordersize * i * 4.2, height/2 + i * size, ii);
          glVertex3d(bordersize * i * 4.2, height/2 + i * size * 4, ii);
          glVertex3d(bordersize * i * 3.8, height/2 + i * size * 4, ii);
		glEnd();

		glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 3.5, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i * 4.5, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i * 4.5, height/2 + i * size, ii);
          glVertex3d(bordersize * i * 3.5, height/2 + i * size, ii);
        glEnd();

        glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 4, height/2 - i * size * 6, ii);
          glVertex3d(bordersize * i * 4.3, height/2 - i * size * 6, ii);
          glVertex3d(bordersize * i * 4.3, height/2 - i * size * 4, ii);
          glVertex3d(bordersize * i * 4, height/2 - i * size * 4, ii);
        glEnd();

        glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 3, height/2 - i * size * 8, ii);
          glVertex3d(bordersize * i * 4.3, height/2 - i * size * 8, ii);
          glVertex3d(bordersize * i * 4.3, height/2 - i * size * 6, ii);
          glVertex3d(bordersize * i * 3, height/2 - i * size * 6, ii);
        glEnd();

        glBegin(GL_QUADS);
          glVertex3d(bordersize * i * 2, height/2 - i * size * 10, ii);
          glVertex3d(bordersize * i * 3, height/2 - i * size * 10, ii);
          glVertex3d(bordersize * i * 3, height/2 - i * size * 5, ii);
          glVertex3d(bordersize * i * 2, height/2 - i * size * 5, ii);
        glEnd();

        glPopMatrix();

  		//render vehicles
  	for (int w = 0; w < AGENTS; ++w)
  		m_Vehicles[w] -> Render();
	    
	//RenderCrosshair(m_vCrosshair);

	/*bordersize, height/2 - i * size * 4), // 1
	bordersize * i, height/2 - i * size * 4), // 2
	bordersize * i, height/2 - i * size * 2.4), // 3
	bordersize * i * 2, height/2 - i * size * 2.4), // 4
	bordersize * i * 2, height/2 - i * size * 4), // 5
	bordersize * i * 3, height/2 - i * size * 4), // 6
	bordersize * i * 3, height/2 + i * size), // 7
	bordersize * i * 2.7, height/2 + i * size), // 8
	bordersize * i * 2.7, height/2 + i * size * 4), // 9
	bordersize * i * 3.8, height/2 + i * size * 4), // 10
	bordersize * i * 3.8, height/2 + i * size), // 11
	bordersize * i * 3.5, height/2 + i * size), // 12
	bordersize * i * 3.5, height/2 - i * size * 4), // 13
	bordersize * i * 4, height/2 - i * size * 4), // 14
	bordersize * i * 4, height/2 - i * size * 6), // 15
	bordersize * i * 3, height/2 - i * size * 6), // 16
	bordersize * i * 3, height/2 - i * size * 5), // 17
	bordersize * i * 2, height/2 - i * size * 5), // 18
	bordersize * i * 2, height/2 - i * size * 10), // 19
	bordersize * i * 3, height/2 - i * size * 10), // 20
	bordersize * i * 3, height/2 - i * size * 8), // 21
	bordersize * i * 4.3, height/2 - i * size * 8), // 22
	bordersize * i * 4.3, height/2 - i * size * 4), // 23
	bordersize * i * 4.5, height/2 - i * size * 4), // 24
	bordersize * i * 4.5, height/2 + i * size), // 25
	bordersize * i * 4.2, height/2 + i * size), // 26
	bordersize * i * 4.2, height/2 + i * size * 6), // 27
	bordersize * i * 2.3, height/2 + i * size * 6), // 28
	bordersize * i * 2.3, height/2 + i * size), // 29
	bordersize * i * 2, height/2 + i * size), // 30
	bordersize * i * 2, height/2 - i * size), // 31
	bordersize * i, height/2 - i * size), // 32
	bordersize * i, height/2 + i * size), // 33
	bordersize, height/2 + i * size) // 34
*/




}

void GameWorld::RenderCrosshair(Vector2D ch){

	glPushMatrix();
	glColor3f(1,0,0);
	Circle((float)ch.x, (float)ch.y, 10);
	glBegin(GL_LINES);
		glVertex2d(ch.x - 18, ch.y); glVertex2d(ch.x + 18, ch.y);
  		glVertex2d(ch.x, ch.y - 18); glVertex2d(ch.x, ch.y + 18);
  	glEnd();	
  	glPopMatrix();
}

void GameWorld::Circle(float px,float py,float r){
	glBegin(GL_LINE_LOOP);
	//glColor3d(255,0,0);

	// 0 < t < 2 PI
	float t = 0.0, x = 0.0, y = 0.0;
	for (int i = 0 ; i < 50 ; i++){
		t = float(i) * 2.0 * Pi;
		x = r * cosf(t/50);
		y = r * sinf(t/50);
		glVertex2d(px + x, py + y);
	}

	glEnd();
}


void GameWorld::Update(double time_elapsed){

	if (m_Vehicles[0] -> getLives() > 0 && !AllDead() ){
		m_Vehicles[0] -> Update(time_elapsed);

		for (int w = 1; w < AGENTS; w++){
			m_Vehicles[w] -> Update(time_elapsed);
			m_Vehicles[w] -> CheckCollisions(m_Vehicles[0] -> getVecBullets());
			m_Vehicles[w] -> CheckCollisions(m_Vehicles[0]);
			m_Vehicles[0] -> CheckCollisions(m_Vehicles[w]);
		}
	}else{
		if (AllDead()){
			cout << "GANASTE !!!" << endl;
		}else{
			cout << "PERDISTE !!!" << endl;
		}
	}

	//for (int w = 1; w < AGENTS; w++)
	//	CheckCollision(m_Vehicles[0], m_Vehicles[w]);
}

bool GameWorld::AllDead(){
	for (int w = 1; w < AGENTS; w++){
		if ( !(m_Vehicles[w] -> getLives() <= 0) )
			return false;
	}
	return true;
}

void GameWorld::CreateWalls(){

  //create the walls  
  int m_cyClient = height; // 600
  int m_cxClient = width;  // 800

  double bordersize = 20.0;
  double CornerSize = 0.1;
  double vDist = m_cyClient - 2 * bordersize; // 560
  double hDist = m_cxClient - 2 * bordersize; // 760
  
  const int NumWallVerts = 34;

  /*cout << "walls : " << endl;
  cout << "wall 1: " << hDist * CornerSize + bordersize << ", " <<  bordersize << endl;
  cout << "wall 2: " << m_cxClient - bordersize - hDist * CornerSize << ", " << bordersize << endl;
  cout << "wall 3: " << m_cxClient - bordersize << ", " << bordersize + vDist*CornerSize << endl;
  cout << "wall 4: " << m_cxClient - bordersize << ", " << m_cyClient - bordersize - vDist * CornerSize << endl;
  cout << "wall 5: " << m_cxClient - bordersize - hDist * CornerSize << ", " << m_cyClient - bordersize << endl;
  cout << "wall 6: " << hDist * CornerSize + bordersize << ", " << m_cyClient - bordersize << endl;
  cout << "wall 7: " << bordersize << ", " << m_cyClient - bordersize - vDist * CornerSize << endl;
  cout << "wall 8: " << bordersize << ", " << bordersize + vDist * CornerSize << endl;*/

  int i = 10;
  int size = 3;
  Vector2D walls[NumWallVerts] = { 	Vector2D(bordersize, height/2 - i * size * 4), // 1
  									Vector2D(bordersize * i, height/2 - i * size * 4), // 2
  									Vector2D(bordersize * i, height/2 - i * size * 2.4), // 3
  									Vector2D(bordersize * i * 2, height/2 - i * size * 2.4), // 4
  									Vector2D(bordersize * i * 2, height/2 - i * size * 4), // 5
  									Vector2D(bordersize * i * 3, height/2 - i * size * 4), // 6
  									Vector2D(bordersize * i * 3, height/2 + i * size), // 7
  									Vector2D(bordersize * i * 2.7, height/2 + i * size), // 8
  									Vector2D(bordersize * i * 2.7, height/2 + i * size * 4), // 9
  									Vector2D(bordersize * i * 3.8, height/2 + i * size * 4), // 10
  									Vector2D(bordersize * i * 3.8, height/2 + i * size), // 11
  									Vector2D(bordersize * i * 3.5, height/2 + i * size), // 12
  									Vector2D(bordersize * i * 3.5, height/2 - i * size * 4), // 13
  									Vector2D(bordersize * i * 4, height/2 - i * size * 4), // 14
  									Vector2D(bordersize * i * 4, height/2 - i * size * 6), // 15
  									Vector2D(bordersize * i * 3, height/2 - i * size * 6), // 16
  									Vector2D(bordersize * i * 3, height/2 - i * size * 5), // 17
  									Vector2D(bordersize * i * 2, height/2 - i * size * 5), // 18
  									Vector2D(bordersize * i * 2, height/2 - i * size * 10), // 19
  									Vector2D(bordersize * i * 3, height/2 - i * size * 10), // 20
  									Vector2D(bordersize * i * 3, height/2 - i * size * 8), // 21
  									Vector2D(bordersize * i * 4.3, height/2 - i * size * 8), // 22
  									Vector2D(bordersize * i * 4.3, height/2 - i * size * 4), // 23
  									Vector2D(bordersize * i * 4.5, height/2 - i * size * 4), // 24
  									Vector2D(bordersize * i * 4.5, height/2 + i * size), // 25
  									Vector2D(bordersize * i * 4.2, height/2 + i * size), // 26
  									Vector2D(bordersize * i * 4.2, height/2 + i * size * 6), // 27
  									Vector2D(bordersize * i * 2.3, height/2 + i * size * 6), // 28
  									Vector2D(bordersize * i * 2.3, height/2 + i * size), // 29
  									Vector2D(bordersize * i * 2, height/2 + i * size), // 30
  									Vector2D(bordersize * i * 2, height/2 - i * size), // 31
  									Vector2D(bordersize * i, height/2 - i * size), // 32
  									Vector2D(bordersize * i, height/2 + i * size), // 33
  									Vector2D(bordersize, height/2 + i * size) // 34
  								 };

  //Vector2D walls[NumWallVerts] = { Vector2D(hDist * CornerSize + bordersize, bordersize),
  //                                 Vector2D(m_cxClient - bordersize - hDist * CornerSize, bordersize),
  //                                 Vector2D(m_cxClient - bordersize, bordersize + vDist*CornerSize)
                                   //Vector2D(m_cxClient - bordersize, m_cyClient - bordersize - vDist * CornerSize),
                                   //Vector2D(m_cxClient - bordersize - hDist * CornerSize, m_cyClient - bordersize),
                                   //Vector2D(hDist * CornerSize + bordersize, m_cyClient - bordersize),
                                   //Vector2D(bordersize, m_cyClient - bordersize - vDist * CornerSize),
                                   //Vector2D(bordersize, bordersize + vDist * CornerSize)
  //                             };
  
  
  for (int w=0; w<NumWallVerts-1; ++w)
  {
    m_Walls.push_back(Wall2D(walls[w], walls[w+1]));
  }

  m_Walls.push_back(Wall2D(walls[NumWallVerts-1], walls[0]));
}

void GameWorld::captureInput(int key){
	switch(key){
		case 32:
			break;
	}
}