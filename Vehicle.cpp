# include "Vehicle.h"
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include "SteeringBehaviors.h"
# include "GameWorld.h"
# include "Bullet.h"

Vehicle::Vehicle(GameWorld* world, Vector2D position, double rotation, Vector2D vel, double _mass, double mForce, double mSpeed, double mTurnRate, double scale) :
					EntityMove( position, scale, vel, mSpeed, Vector2D(sin(rotation), -cos(rotation)), _mass, Vector2D(scale, scale), mTurnRate, mForce),
					timeElapsed(0.0), m_Feelers(3), m_pWorld(world), m_iLives(5), m_bStatus(true)
{

	initializeBuffer();

	time_ = 0;
	timebase = 0;
	anim = 0;
}

Vehicle::~Vehicle(){

}

void Vehicle::Update(double time_elapsed){
	//update the time elapsed
	timeElapsed = time_elapsed;

	//keep a record of its old position so we can update its cell later	
	//in this method
	Vector2D OldPos = getPos();


	Vector2D SteeringForce;
	
	//calculate the combined force from each steering behavior in the 
	//vehicle's list
	SteeringForce = m_pSteering -> Calculate();
	    
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

	//shoot ----------------------------------------------

	for (unsigned int w = 0; w < m_Bullets.size(); ++w){
    	m_Bullets[w] -> Render();
    	m_Bullets[w] -> Update(time_elapsed);
  	}

	time_ = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	int dt = time_ - timebase;// delta time
	timebase = time_;
	anim += dt;//duracion de la animacion entre dos cambios de Sprite
	
	if (anim / 1000.0 > 3.0 && getType() == 0 && m_iLives > 0)// si el tiempo de animacion dura mas 0.15s cambiamos de sprite
	{
		Bullet* tmpBullet;
		float m_dWanderDistance = 2.0;

		Vector2D m_vTCC = PointToWorldSpace(Vector2D(m_dWanderDistance * getRadius(), 0), getHeading(), getSide(), getPos());

		tmpBullet = new Bullet(m_vTCC, getHeading(), getHeading() * 50, 1.0, 40.0 * 2.0, 50.0, Pi, 10.0, getType());
		tmpBullet -> setTypeMove(1);
		m_Bullets.push_back(tmpBullet);

		anim = 0.0;
	}else{
		if (keyStates[KEY_SPACE] && getType() == 1 && (float(anim /1000) > 0.2) && m_iLives > 0){
			Bullet* tmpBullet;
			float m_dWanderDistance = 2.0;

			Vector2D m_vTCC = PointToWorldSpace(Vector2D(m_dWanderDistance * getRadius(), 0), getHeading(), getSide(), getPos());

			tmpBullet = new Bullet(m_vTCC, getHeading(), getHeading() * 200, 1.0, 40.0 * 2.0, 50.0, Pi, 8.0, getType());
			tmpBullet -> setTypeMove(1);
			m_Bullets.push_back(tmpBullet);
			anim = 0.0;
		}
	}

	//treat the screen as a toroid
	WrapAround(entityPos, m_pWorld -> getWidth(), m_pWorld -> getHeight()); 
}

bool Vehicle::CheckCollisions(std::vector<Bullet*> v_bullets){
	if (v_bullets.size() > 0){
		for (unsigned int w = 0; w < m_Bullets.size(); ++w){
			for (unsigned int y = 0; y < v_bullets.size(); ++y){
				if (m_Bullets[w] -> CheckCollision(v_bullets[y]) && v_bullets[y] -> getStatusBullet() && m_Bullets[w] -> getStatusBullet() ){
						v_bullets[y] -> BulletOff();
						m_Bullets[w] -> BulletOff();
						return 1;
	  				}
			}
	  	}
	}

  	return 0;
}

bool Vehicle::CheckCollisions(Vehicle* veh){
	if (m_Bullets.size() > 0){
		for (unsigned int w = 0; w < m_Bullets.size(); ++w){
			if (m_Bullets[w] -> CheckCollision(veh)){
				m_Bullets[w] -> BulletOff();
				veh -> setImpact();
				cout << "Vehiculo " << veh -> getID() << " Te quedan : " << veh -> getLives() << " vidas" << endl; 
			}
		}
	}
}

void Vehicle::setImpact(){
	m_iLives--;
	if (m_iLives == 0){
		setMaxSpeed(0.0);	
	}
}

void Vehicle::Render(){

	if (m_bStatus){
		static std::vector<Vector2D>  m_vecVehicleVBTrans;
		m_vecVehicleVBTrans = WorldTransform( vecVehicleBuff, getPos(), getHeading(), getSide(), getScale() );
		ClosedShape(m_vecVehicleVBTrans);
	}
}

void Vehicle::ClosedShape(const std::vector<Vector2D> &points){

	glPushMatrix();
		if (getType() == 0)
			glColor3f(0.22, 0.20, 0.20);
		else
			glColor3f(0.30, 0.20, 0.60);

		glBegin(GL_POLYGON);
			glVertex3d(points[0].x, points[0].y, 1); 

		    //for (unsigned int p=1; p<points.size(); ++p){
			for (unsigned int p=points.size()-1; p>=1; --p){
	    		glVertex3d(points[p].x, points[p].y, 1);
	    	}
	    	//glVertex3d(points[0].x, points[0].y, 0); 

	    glEnd();
    glPopMatrix();

    glPushMatrix();
		glColor3f(1, 1, 1);			
		glBegin(GL_POINTS);
			glVertex3d(getPos().x, getPos().y,1);
		glEnd();
	glPopMatrix();
}

void Vehicle::initializeBuffer(){
	const int numVehicleVerts = 3;

//	Vector2D vehicle[numVehicleVerts] = { Vector2D(-1.0f, 0.6f), Vector2D(1.0f, 0.0f), Vector2D(-1.0f, -0.6f) };
	int x = 1;
	Vector2D vehicle[numVehicleVerts] = { Vector2D(-1.0f*x, 0.6f*x), Vector2D(1.0f*x, 0.0f*x), Vector2D(-1.0f*x, -0.6f*x) };

	for (int i = 0; i < numVehicleVerts; ++i){
		vecVehicleBuff.push_back(vehicle[i]);
	}
}

