//
//  Floor.h
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_Floor_h
#define Fever_Floor_h

#include <string>
#include "GameObject.h"
#include "Point.h"

const int SEGMENT_WIDTH = 5;
const int POINT_FREQUENCY = 50; // Every 50x calculate a point
const float HEURISTIC_TIME_LOST = 0.995;

const float MAX_FLOOR_NORMALISER = 0.5;
const float MIN_FLOOR_NORMALISER = 0.05; //0.1;
const float INCREASE_FLOOR_NORMALISER = 0.03;

// Floor draws itself and tells player where it is for collision detection
class Floor : public GameObject
{	
public:
    
	Floor(const std::string &filename);
    ~Floor();
    
    inline int GetHeight() const {return FindHeight(0);}
    inline int GetNextHeight() const {return FindHeight(1);}
    inline int GetLastHeight() const {return FindHeight(-1);}
    
    void SetFloorPoints();
    void SetFloorVertices();
    
    float LerpHeight(); // CURRENTLY NO USE FOR THIS FUNC
    
	bool virtual Update(float dt); 
    void virtual Render(SDL_Surface* pScreen, Camera& rCamera) const;
    void End(SDL_Surface* pScreen, Camera& rCamera) const;
    
    bool OnFloor(GameObject& rObject);
    float ThroughFloor(GameObject& rObject, float dt);
	
	const CollisionBox virtual MyCollisionBox(float dt = 0);
    
    static bool GenFloorPoints(std::string lvlfile, std::string songpath);
    
private:
    
    std::string m_filename;
    
    float m_levelSpeed;
    int m_pointsSize;
    int m_fromKeyPointI, m_toKeyPointI;
    float m_offsetX;
    Point* m_pFloorPoints;  // MAKE THIS INTO A VECTOR INSTEAD
    
    
    static Point* GenRandomPoints(int* pointsSize);
    static Point* GenOriginalPoints(int* pointsSize);
    static Point* GenSmoothPoints(Point* pOriginalPoints, int originalPointsSize, int* smoothPointsSize);
    
    float FindHeight(int at = 0) const;
};

#endif
