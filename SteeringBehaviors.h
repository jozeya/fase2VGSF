#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H
#pragma warning (disable:4786)

#include "Vector2D.h"
#include "Wall2D.h"

class Vehicle;
//class Wall2D;

class SteeringBehaviors{
	public:
		enum summing_method{weighted_average, prioritized, dithered};

    float dir;
    float direction;

	private:
		enum behavior_type{
                none               = 0x00000,
                seek               = 0x00002,
                flee               = 0x00004,
                arrive             = 0x00008,
                wander             = 0x00010,
                cohesion           = 0x00020,
                separation         = 0x00040,
                allignment         = 0x00080,
                obstacle_avoidance = 0x00100,
                wall_avoidance     = 0x00200,
                follow_path        = 0x00400,
                pursuit            = 0x00800,
                evade              = 0x01000,
                interpose          = 0x02000,
                hide               = 0x04000,
                flock              = 0x08000,
                offset_pursuit     = 0x10000,
                manual_wander      = 0x08000,
      };

    	Vehicle* m_pVehicle;
    	Vector2D m_vSteeringForce;
    	Vehicle* m_pTargetAgent1;
  		Vehicle* m_pTargetAgent2;
  		Vector2D m_vTarget;

      double m_dWeightSeek;
      double m_dWeightFlee;
      double m_dWeightPursuit;
      double m_dWeightEvade;

      double m_dWanderJitter;
      double m_dWanderRadius;
      double m_dWanderDistance;
      double m_dWeightWander;

      double m_dWeightWallAvoidance;
      double m_dWallDetectionFeelerLength;
      std::vector<Vector2D> m_Feelers;

      Vector2D m_vWanderTarget;
      
      int m_iFlags;
      enum Deceleration{slow = 3, normal = 2, fast = 1};

      Deceleration m_Deceleration;

      bool On(behavior_type bt){
        return (m_iFlags & bt) == bt;
      }

  		Vector2D Seek(Vector2D TargetPos);

  		Vector2D Flee(Vector2D TargetPos);

      Vector2D Pursuit(const Vehicle*);

      Vector2D Evade(const Vehicle*);

      Vector2D Wander(); 
      Vector2D ManualWander();

      bool AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);

      Vector2D CalculatePrioritized();

      Vector2D WallAvoidance(const std::vector<Wall2D> &walls);
      void CreateFeelers();

  	public:

  		SteeringBehaviors(Vehicle* agent);
  		~SteeringBehaviors();
		
  		Vector2D Calculate();

      void SeekOn(){
        m_iFlags |= seek;
      }

      void FleeOn(){
        m_iFlags |= flee; 
      }

      void PursuitOn(Vehicle* v){
        m_iFlags |= pursuit; 
        m_pTargetAgent1 = v;
      }

      void EvadeOn(Vehicle* v){
        m_iFlags |= evade; 
        m_pTargetAgent1 = v;
      }

      void WanderOn(){
        m_iFlags |= wander;
      }

      void ManualWanderOn(float keyDirection){
        m_iFlags |= manual_wander;
        direction = keyDirection;
      }

      void ManualWanderOff(){
        if(On(manual_wander)) 
          m_iFlags ^= manual_wander;
      }

      void DrawRadiusWander();

      void Circle(float, float, float);

      void WallAvoidanceOn(){
        m_iFlags |= wall_avoidance;
      }



};

#endif