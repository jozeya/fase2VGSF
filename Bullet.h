#ifndef BULLET_H
#define BULLET_H
#pragma warning (disable:4786)
//ctime.h


#include <iostream>
#include "Vector2D.h"
#include "geometry.h"
#include "Transformations.h"
#include "EntityMove.h"
#include <vector>

# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include <glm/glm.hpp> 

using namespace std;

class Bullet : public EntityMove{
public:

	double timeElapsed;
	int m_iTypeBullet;

	vector<Vector2D> vecVehicleBuff;
	vector<Vector2D> vectBulletBuff;
	double m_dRaduisBullet;
	bool m_bStatus_bullet;

	Bullet(Vector2D position, Vector2D hg, Vector2D vel, double _mass, double mForce, double mSpeed, double mTurnRate, double scale, int typeB);
	~Bullet();
	void Render();
	void Update(double);
	void initializeBuffer();
	void ClosedShape(const std::vector<Vector2D> &);
	void Circle(float px,float py,float r);
	bool CheckCollision(EntityMove*);
	float Clamp(float, float, float);
	void BulletOff();
	bool getStatusBullet();

};

	Bullet::Bullet(Vector2D position, Vector2D hg, Vector2D vel, double _mass, double mForce, double mSpeed, double mTurnRate, double scale, int typeB): EntityMove(
																																			position, 
																																			scale, 
																																			vel, 
																																			mSpeed, 
																																			hg, 
																																			_mass, 
																																			Vector2D(scale, scale), 
																																			mTurnRate, 
																																			mForce),
																																			timeElapsed(0.0),
																																			m_iTypeBullet(typeB),
																																			m_bStatus_bullet(true)
	{
		initializeBuffer();
		m_dRaduisBullet = 13;
	}

	Bullet::~Bullet(){}

	void Bullet::BulletOff(){
		m_bStatus_bullet = false;
		setMaxSpeed(0.0);
		setPos(Vector2D(0,0));	
	}

	bool Bullet::getStatusBullet(){
		return m_bStatus_bullet;
	}

	void Bullet::Render(){

		if (m_bStatus_bullet){
			static std::vector<Vector2D>  m_vecVehicleVBTrans;
			m_vecVehicleVBTrans = WorldTransform( vectBulletBuff, getPos(), getHeading(), getSide(), getScale() );
			ClosedShape(m_vecVehicleVBTrans);
		}
	}

	void Bullet::Update(double time_elapsed){
		timeElapsed = time_elapsed;

		Vector2D OldPos = getPos();


		Vector2D SteeringForce;

		SteeringForce = getHeading();
	
		//calculate the combined force from each steering behavior in the 
		//vehicle's list
		//SteeringForce = m_pSteering -> Calculate();
		    
		//Acceleration = Force/Mass
		Vector2D acceleration = SteeringForce / mass;

		//update velocity
		velocity += acceleration * time_elapsed; 

		//make sure vehicle does not exceed maximum velocity
		velocity.Truncate(maxSpeed);

		//update the position
		entityPos += velocity * time_elapsed;

		//update the heading if the vehicle has a non zero velocity
		if (velocity.LengthSq() > 0.00000001){    
			heading = Vec2DNormalize(velocity);

		    side = heading.Perp();
		}

		if ( !(getPos().x > 0 && getPos().x < 800 && getPos().y > 0 || getPos().y < 600) )
			BulletOff();
		//EnforceNonPenetrationConstraint(this, World()->Agents());

		//treat the screen as a toroid
		//WrapAround(entityPos, m_pWorld -> getWidth(), m_pWorld -> getHeight());
	}

	void Bullet::initializeBuffer(){
		const int numVehicleVerts = 4;

		//	Vector2D vehicle[numVehicleVerts] = { Vector2D(-1.0f, 0.6f), Vector2D(1.0f, 0.0f), Vector2D(-1.0f, -0.6f) };
		int x = 1;
		//Vector2D vehicle[numVehicleVerts] = { Vector2D(-1.0f, 0.6f), Vector2D(1.0f, 0.0f), Vector2D(-1.0f, -0.6f) };
		Vector2D vehicle[numVehicleVerts] = { Vector2D(-1.0f*x, 0.5f*x), Vector2D(1.0f*x, 0.5f*x), Vector2D(1.0f*x, -0.5f*x), Vector2D(-1.0f*x, -0.5f*x) };

		for (int i = 0; i < numVehicleVerts; ++i){
			vectBulletBuff.push_back(vehicle[i]);
		}
	}

	void Bullet::ClosedShape(const std::vector<Vector2D> &points){
	
		glPushMatrix();
			if (m_iTypeBullet == 1){
				glColor3f(0, 0, 1);

				glBegin(GL_POLYGON);
					glVertex3d(points[0].x, points[0].y, 1); 

				    //for (unsigned int p=1; p<points.size(); ++p){
					for (unsigned int p=points.size()-1; p>=1; --p){
			    		glVertex3d(points[p].x, points[p].y, 1);
		    		}
		    		//glVertex3d(points[0].x, points[0].y, 0); 
		  	 	glEnd();

			}else{
				glColor3f(1, 0, 0);
				Circle(getPos().x, getPos().y, m_dRaduisBullet);

		    	//glTranslatef(getPos().x, getPos().y, 0);
				//glutWireSphere(10.0, 50, 50);
		  	}
		glPopMatrix();
	}

	void Bullet::Circle(float px,float py,float r){
		glBegin(GL_POLYGON);
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

	bool Bullet::CheckCollision(EntityMove* two){

		if (m_iTypeBullet == 0){

			// Get center point circle first 
		    glm::vec2 center(getPos().x, getPos().y);

		    // Calculate AABB info (center, half-extents)
		    glm::vec2 aabb_half_extents;
		    if (two -> getTypeMove() == 1)
		    	aabb_half_extents = glm::vec2(16  / 2, 8 / 2);
		   	else
		   		aabb_half_extents = glm::vec2(30  / 2, 18 / 2);

		    glm::vec2 aabb_center(two -> getPos().x, two -> getPos().y);
		    // Get difference vector between both centers
		    glm::vec2 difference = center - aabb_center;
		    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
		    // Add clamped value to AABB_center and we get the value of box closest to circle
		    glm::vec2 closest = aabb_center + clamped;
		    // Retrieve vector between center circle and closest point AABB and check if length <= radius
		    difference = closest - center;
		    return glm::length(difference) < m_dRaduisBullet;
		}else{
			// Get center point circle first 
		    glm::vec2 center(two -> getPos().x, two -> getPos().y);

		    // Calculate AABB info (center, half-extents)
		    glm::vec2 aabb_half_extents;
		    if (getTypeMove() == 1)
		    	aabb_half_extents = glm::vec2(16  / 2, 8 / 2);
		   	else
		   		aabb_half_extents = glm::vec2(30  / 2, 18 / 2);

		    glm::vec2 aabb_center(getPos().x, getPos().y);
		    // Get difference vector between both centers
		    glm::vec2 difference = center - aabb_center;
		    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
		    // Add clamped value to AABB_center and we get the value of box closest to circle
		    glm::vec2 closest = aabb_center + clamped;
		    // Retrieve vector between center circle and closest point AABB and check if length <= radius
		    difference = closest - center;
		    return glm::length(difference) < two -> getScale().x;
		}

		return 0;
	}

	float Bullet::Clamp(float value, float min, float max) {
    	return std::max(min, std::min(max, value));
	} 


//coodernadas polares
#endif 