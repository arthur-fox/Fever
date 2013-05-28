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

// TODO: FINISH UP AND TIDY UP BOTH HEADER AND CLASS FILES!


//TODO: THESE VALUES SHOULD PROBABLY BE GOT FROM THE FILE?
const int MAX_FLOOR_POINTS = 100; 
const int SEGMENT_WIDTH = 5;
const int POINT_FREQUENCY = 50; // Every 50x calculate a point
const float HEURISTIC_TIME_LOST = 0.995;

// Floor draws itself and tells player where it is for collision detection
class Floor : public GameObject
{	
public:
    
	Floor(const std::string &filename);
    ~Floor();
    
    inline int GetHeight() const {return FindHeight(0);}
    inline int GetLastHeight() const {return FindHeight(-1);}
    
    void SetFloorPoints();
    void SetFloorVertices();
    
    float LerpHeight(); // CURRENTLY NO USE FOR THIS FUNC
    
	bool virtual Update(float deltaTicks); 
    void Render(SDL_Surface* pScreen) const;
    void End(SDL_Surface* pScreen);
    
    bool OnFloor(GameObject& rObject);
    float ThroughFloor(GameObject& rObject, float deltaTicks);
	
	const CollisionBox virtual MyCollisionBox(float deltaTicks = 0);
    
    static bool GenFloorPoints(std::string lvlfile, std::string songpath);
    
private:

    //DEBUG
    static int m_updateCalled;
    
    std::string m_filename;
    
    float m_levelSpeed;
    int m_fromKeyPointI, m_toKeyPointI;
    float m_offsetX;
    Point* m_pFloorPoints;  // MAKE THIS INTO A VECTOR INSTEAD
    
    
    static Point* GenOriginalPoints(int* pointsSize);
    static Point* GenSmoothPoints(Point* pOriginalPoints, int originalPointsSize, int* smoothPointsSize);
    
    float FindHeight(int at = 0) const;
    
    //NOTE: DELETE this later
    //      can be used to find x-travelled!
    static float m_COPYoffsetX; //TEMPORARY
    static void ThreadFunc(); // TEMPORARY
};

#endif
