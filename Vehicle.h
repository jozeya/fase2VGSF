#ifndef VEHICLE_H
#define VEHICLE_H
#pragma warning (disable:4786)
//ctime.h

#include "EntityMove.h"
#include <vector>
#include <iostream>
#include "Transformations.h"

#include "geometry.h"

class GameWorld;
class SteeringBehaviors;
class Bullet;

#define KEY_SPACE   32
#define KEY_A   	65
#define KEY_D   	68
#define KEY_W   	87

using namespace std;

class Vehicle : public EntityMove{
	private:
		double timeElapsed;
		vector<Vector2D> vecVehicleBuff; // buffer para la forma del vehiculo

		SteeringBehaviors* m_pSteering;

		GameWorld* m_pWorld;

		Vector2D SteeringForce;

		void initializeBuffer();

		bool keyStates[256];

		int m_iLives;
		bool m_bStatus;

		// vehicle bullet 
		double time_;
		double timebase;
		double anim;

		vector <Bullet*> m_Bullets;

  		int m_iFlags;

	public:
		Vehicle(GameWorld* world, Vector2D position, double rotation, Vector2D vel, double _mass, double mForce, double mSpeed, double mTurnRate, double scale);

		~Vehicle();

		void Update(double);
		void Render();
		void ClosedShape(const std::vector<Vector2D> &points);

		double getTimeElapsed(){
			return timeElapsed;
		}

		void KeyPressed(int key){
			keyStates[key] = true;
		}

		void keyUp(int key){
			keyStates[key] = false;
		}

		bool CheckCollisions(std::vector<Bullet*>);
		bool CheckCollisions(Vehicle*);

		vector<Bullet*> getVecBullets(){
			return m_Bullets;
		}

		void setImpact();

		int getLives(){
			return m_iLives;
		}

		//*+++++++++++++++++++++++++++++++++++++++++++

		GameWorld*const World()const{
			return m_pWorld;
		} 

		SteeringBehaviors*const Steering()const{
			return m_pSteering;
		}

};

#endif