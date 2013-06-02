//
//  GameObject.h
//  Fever
//
//  Created by Arthur Fox on 14/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_GameObject_h
#define Fever_GameObject_h

#include "SDL.h"
#include "Camera.h"

// GameObject is the superclass for all INTERACTABLE objects in the game 
// eg. player, notes, floor
//NOTE: All subclasses must initialise EVERY protected variable in their constructor

class GameObject
{
public:
	
    //Virtual destructor to avoid memory leaks when classes inheriting from GameObject are destroyed
    virtual ~GameObject(){}
    
	class CollisionBox
	{
	public:
		float x1, x2, y1, y2; //NOTE: This is an Abstract concept - the box is not necessarily the outline of the image
		CollisionBox();
	};
	
	//Returns the object's collision line - uses dt to calculate y1 and MUST be implemented by object
	const CollisionBox virtual MyCollisionBox( float dt = 0 ) = 0;
	
	//Moves the object, returns true on success - MUST be implemented by actual object 
	bool virtual Update( float dt ) = 0;
	
	void virtual Render( SDL_Surface* pScreen, Camera& rCamera) const;
	inline float GetX()      const {return m_x;   }
	inline float GetY()      const {return m_y;   }
	inline float GetXVel()   const {return m_xVel;}
	inline float GetYVel()   const {return m_yVel;}
	inline int   GetHeight() const {return m_pImage!=NULL ? m_pImage->h : -1 ;} //NOTE: Height and Width return -1 if the image failed to load
	inline int   GetWidth()  const {return m_pImage!=NULL ? m_pImage->w : -1 ;}
    
protected:
    
	float m_x, m_y;
	float m_xVel, m_yVel;
	
	//Image of the object
	SDL_Surface* m_pImage;
};

#endif