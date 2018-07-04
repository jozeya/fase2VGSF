#include "SteeringBehaviors.h"
# include <GL/glu.h>
# include <GL/glut.h>
#include "Vehicle.h"
#include "GameWorld.h"
//#include "Wall2D.h"

SteeringBehaviors::SteeringBehaviors(Vehicle* agent): 
														m_pVehicle(agent),
														m_iFlags(0),
														m_dWeightSeek(1.0),
														m_dWeightFlee(1.0),
														m_dWeightPursuit(1.0),
														m_dWeightEvade(1.0),
														//wander
														m_dWanderJitter(20.0),
														m_dWanderRadius(1.2),
														m_dWanderDistance(2.0),
														m_dWeightWander(8.0),
														//walls
														m_dWeightWallAvoidance(10.0),
														m_dWallDetectionFeelerLength(40.0),
														m_Feelers(3)
{

	//stuff for the wander behavior
  	double theta = RandFloat() * TwoPi;
    theta = 1;
  	//create a vector to a target position on the wander circle
  	m_vWanderTarget = Vector2D(m_dWanderRadius * cos(theta), m_dWanderRadius * sin(theta));
    
    dir = 0;

}

Vector2D SteeringBehaviors::Seek(Vector2D TargetPos){

	Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle -> getPos()) * m_pVehicle -> getMaxSpeed();

	return (DesiredVelocity - m_pVehicle -> getVelocity());
}

Vector2D SteeringBehaviors::Flee(Vector2D TargetPos){
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	const double PanicDistanceSq = 100.0f * 100.0;

	if (Vec2DDistanceSq(m_pVehicle -> getPos(), TargetPos) > PanicDistanceSq){
 	    return Vector2D(0,0);
	}
	

	Vector2D DesiredVelocity = Vec2DNormalize(m_pVehicle -> getPos() - TargetPos) * m_pVehicle -> getMaxSpeed();

	return (DesiredVelocity - m_pVehicle -> getVelocity());
}

Vector2D SteeringBehaviors::Pursuit(const Vehicle* evader){
  	//if the evader is ahead and facing the agent then we can just seek
  	//for the evader's current position.
  	Vector2D ToEvader = evader -> getPos() - m_pVehicle -> getPos();

  	double RelativeHeading = m_pVehicle -> getHeading().Dot(evader -> getHeading());

  	if ( (ToEvader.Dot(m_pVehicle -> getHeading()) > 0) &&  (RelativeHeading < -0.95)){  //acos(0.95)=18 degs
    	return Seek(evader -> getPos());
  	}

  	//Not considered ahead so we predict where the evader will be.
 
  	//the lookahead time is propotional to the distance between the evader
  	//and the pursuer; and is inversely proportional to the sum of the
  	//agent's velocities
  	double LookAheadTime = ToEvader.Length() / (m_pVehicle -> getMaxSpeed() + evader -> getSpeed());
  
  	//now seek to the predicted future position of the evader
  	return Seek(evader -> getPos() + evader -> getVelocity() * LookAheadTime);
}

Vector2D SteeringBehaviors::Evade(const Vehicle* pursuer){
  
  	/* Not necessary to include the check for facing direction this time */

  	Vector2D ToPursuer = pursuer -> getPos() - m_pVehicle -> getPos();

  	//uncomment the following two lines to have Evade only consider pursuers 
  	//within a 'threat range'
  	const double ThreatRange = 100.0;
  	if (ToPursuer.LengthSq() > ThreatRange * ThreatRange) 
  		return Vector2D();
 
  	//the lookahead time is propotional to the distance between the pursuer
  	//and the pursuer; and is inversely proportional to the sum of the
  	//agents' velocities
  	double LookAheadTime = ToPursuer.Length() / (m_pVehicle -> getMaxSpeed() + pursuer -> getSpeed());
  
  	//now flee away from predicted future position of the pursuer
  	return Flee(pursuer -> getPos() + pursuer -> getVelocity() * LookAheadTime);
}

Vector2D SteeringBehaviors::Wander(){ 
	
	//this behavior is dependent on the update rate, so this line must
 	//be included when using time independent framerate.
  double JitterThisTimeSlice = m_dWanderJitter * m_pVehicle -> getTimeElapsed();

 	//first, add a small random vector to the target's position
 	m_vWanderTarget += Vector2D(RandomClamped() * JitterThisTimeSlice, RandomClamped() * JitterThisTimeSlice);
	
	//reproject this new vector back on to a unit circle
 	m_vWanderTarget.Normalize();

 	//increase the length of the vector to the same as the radius
	//of the wander circle
	m_vWanderTarget *= m_dWanderRadius;

	//move the target into a position WanderDist in front of the agent
	Vector2D target = m_vWanderTarget + Vector2D(m_dWanderDistance, 0);

	//project the target into world space
	Vector2D Target = PointToWorldSpace(target, m_pVehicle -> getHeading(), m_pVehicle -> getSide(), m_pVehicle -> getPos());

	//and steer towards it
	//DrawRadiusWander();
	return Target - m_pVehicle -> getPos(); 
}

Vector2D SteeringBehaviors::ManualWander(){ 
  
  //this behavior is dependent on the update rate, so this line must
  //be included when using time independent framerate.
  double JitterThisTimeSlice = m_dWanderJitter * m_pVehicle -> getTimeElapsed();
  //first, add a small random vector to the target's position
  //m_vWanderTarget += Vector2D(RandomClamped() * JitterThisTimeSlice, RandomClamped() * JitterThisTimeSlice);

  float theta = direction;
  //create a vector to a target position on the wander circle
  m_dWanderRadius = 5.0;
  m_dWanderDistance = 0.0;
  m_vWanderTarget = Vector2D(m_dWanderRadius * cos(theta), m_dWanderRadius * sin(theta));

  dir = 0;

  m_vWanderTarget += Vector2D(dir * JitterThisTimeSlice, dir * JitterThisTimeSlice);

  //reproject this new vector back on to a unit circle
  m_vWanderTarget.Normalize();

  //increase the length of the vector to the same as the radius
  //of the wander circle
  m_vWanderTarget *= m_dWanderRadius;

  //move the target into a position WanderDist in front of the agent
  Vector2D target = m_vWanderTarget + Vector2D(m_dWanderDistance, 0);
  //Vector2D target = m_vWanderTarget + Vector2D(0, 0);

  //project the target into world space
  Vector2D Target = PointToWorldSpace(target, m_pVehicle -> getHeading(), m_pVehicle -> getSide(), m_pVehicle -> getPos());

  //and steer towards it
  //DrawRadiusWander();
  return Target - m_pVehicle -> getPos(); 
}

bool SteeringBehaviors::AccumulateForce(Vector2D &RunningTot, Vector2D ForceToAdd){
  
  //calculate how much steering force the vehicle has used so far
  double MagnitudeSoFar = RunningTot.Length();

  //calculate how much steering force remains to be used by this vehicle
  double MagnitudeRemaining = m_pVehicle -> getMaxForce() - MagnitudeSoFar;

  //return false if there is no more force left to use
  if (MagnitudeRemaining <= 0.0) return false;

  //calculate the magnitude of the force we want to add
  double MagnitudeToAdd = ForceToAdd.Length();
  
  //if the magnitude of the sum of ForceToAdd and the running total
  //does not exceed the maximum force available to this vehicle, just
  //add together. Otherwise add as much of the ForceToAdd vector is
  //possible without going over the max.
  if (MagnitudeToAdd < MagnitudeRemaining){
    RunningTot += ForceToAdd;
  }else{
    //add it to the steering force
    RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining); 
  }

  return true;
}

Vector2D SteeringBehaviors::CalculatePrioritized(){       
  
  	Vector2D force;

  	if (On(wall_avoidance)){
    	force = WallAvoidance(m_pVehicle->World() -> Walls()) * m_dWeightWallAvoidance;

    	if (!AccumulateForce(m_vSteeringForce, force)) 
    		return m_vSteeringForce;
  	}
  
  	if (On(seek)){
  		force = Seek(m_pVehicle -> World() -> Crosshair()) * m_dWeightSeek;

    	if (!AccumulateForce(m_vSteeringForce, force)) 
    		return m_vSteeringForce;
  	}

  	if (On(flee)){
    	force = Flee(m_pVehicle -> World() -> Crosshair()) * m_dWeightFlee;

	    if (!AccumulateForce(m_vSteeringForce, force)) 
    		return m_vSteeringForce;
  	}
	
	if (On(pursuit)){
    	assert(m_pTargetAgent1 && "pursuit target not assigned");

    	force = Pursuit(m_pTargetAgent1) * m_dWeightPursuit;

    	if (!AccumulateForce(m_vSteeringForce, force)) 
    		return m_vSteeringForce;
  	}

  	if (On(wander)){
    	force = Wander() * m_dWeightWander;

    	if (!AccumulateForce(m_vSteeringForce, force)) 
    		return m_vSteeringForce;
  	}

    if (On(manual_wander)){
      force = ManualWander() * m_dWeightWander;

      if (!AccumulateForce(m_vSteeringForce, force)) 
        return m_vSteeringForce;
    }

  	 if (On(evade)){
    	assert(m_pTargetAgent1 && "Evade target not assigned");
    
    	force = Evade(m_pTargetAgent1) * m_dWeightEvade;

    	if (!AccumulateForce(m_vSteeringForce, force)) 
    		return m_vSteeringForce;
  	}


  	return m_vSteeringForce;
  }

Vector2D SteeringBehaviors::Calculate(){ 
  	
  	//reset the steering force
 	m_vSteeringForce.Zero();

	m_vSteeringForce = CalculatePrioritized(); 

	return m_vSteeringForce;
}

void SteeringBehaviors::Circle(float px,float py,float r){
	glBegin(GL_LINE_LOOP);
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


void SteeringBehaviors::DrawRadiusWander(){

	Vector2D m_vTCC = PointToWorldSpace(Vector2D(m_dWanderDistance * m_pVehicle->getRadius(), 0),
                                         m_pVehicle->getHeading(),
                                         m_pVehicle->getSide(),
                                         m_pVehicle->getPos());
	glPushMatrix();
		glColor3f(1,0,0);
		Circle(m_vTCC.x, m_vTCC.y, m_dWanderRadius * m_pVehicle->getRadius()); 
	glPopMatrix();


	m_vTCC = PointToWorldSpace(( m_vWanderTarget + Vector2D(m_dWanderDistance,0)) * m_pVehicle->getRadius(),
                                  m_pVehicle->getHeading(),
                                  m_pVehicle->getSide(),
                                  m_pVehicle->getPos());

	glPushMatrix();
		glColor3f(0,0,0);
		Circle(m_vTCC.x, m_vTCC.y, 3);
	glPopMatrix();
}

Vector2D SteeringBehaviors::WallAvoidance(const std::vector<Wall2D>& walls){
  
  //the feelers are contained in a std::vector, m_Feelers
  CreateFeelers();
  
  double DistToThisIP    = 0.0;
  double DistToClosestIP = MaxDouble;

  //this will hold an index into the vector of walls
  int ClosestWall = -1;

  Vector2D SteeringForce,
            point,         //used for storing temporary info
            ClosestPoint;  //holds the closest intersection point

  //examine each feeler in turn
  for (unsigned int flr = 0; flr < m_Feelers.size(); ++flr){
    //run through each wall checking for any intersection points
    for (unsigned int w = 0; w < walls.size(); ++w){
      if (LineIntersection2D(m_pVehicle -> getPos(), m_Feelers[flr], walls[w].From(), walls[w].To(), DistToThisIP, point)){
        //is this the closest found so far? If so keep a record
        if (DistToThisIP < DistToClosestIP){
          DistToClosestIP = DistToThisIP;

          ClosestWall = w;

          ClosestPoint = point;
        }
      }
    }//next wall

  
    //if an intersection point has been detected, calculate a force  
    //that will direct the agent away
    if (ClosestWall >= 0)
    {
      //calculate by what distance the projected position of the agent
      //will overshoot the wall
      Vector2D OverShoot = m_Feelers[flr] - ClosestPoint;

      //create a force in the direction of the wall normal, with a 
      //magnitude of the overshoot
      SteeringForce = walls[ClosestWall].Normal() * OverShoot.Length();
    }

  }//next feeler

  return SteeringForce;
}

void SteeringBehaviors::CreateFeelers(){
  
  //feeler pointing straight in front
  m_Feelers[0] = m_pVehicle -> getPos() + m_dWallDetectionFeelerLength * m_pVehicle -> getHeading();

  //feeler to left
  Vector2D temp = m_pVehicle -> getHeading();
  Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
  m_Feelers[1] = m_pVehicle -> getPos() + m_dWallDetectionFeelerLength/2.0f * temp;

  //feeler to right
  temp = m_pVehicle -> getHeading();
  Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
  m_Feelers[2] = m_pVehicle -> getPos() + m_dWallDetectionFeelerLength/2.0f * temp;
}