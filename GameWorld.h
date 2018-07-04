#ifndef GameWorld_H
#define GameWorld_H

#include "Vehicle.h"
#include "Wall2D.h"

#define AGENTS 15
#define KEY_A   	65
#define KEY_D   	68
#define KEY_W   	87
#define KEY_S   	83

class GameWorld {
public:
	int width;
	int height;
	bool keyStates[256];

	Vehicle* m_Vehicles[AGENTS];

	Vector2D m_vCrosshair;

	std::vector<Wall2D> m_Walls;

public:

	GameWorld(int, int);
	GameWorld();
	~GameWorld();

	void Render();
	void RenderCrosshair(Vector2D ch);
	void Circle(float px,float py,float r);
	void captureInput(int);


	void Update(double);

	bool AllDead();

	void ActivateKey(int key);

	void DeactivateKey(int key);
	
	Vector2D Crosshair()const{
		return m_vCrosshair;
	}

	void setCrosshair(Vector2D target){
		m_vCrosshair = target;
	}

	int getWidth(){
		return width;
	}

	int getHeight(){
		return height;
	}

	const std::vector<Wall2D>& Walls(){
		return m_Walls;
	} 

	void CreateWalls();
};

#endif