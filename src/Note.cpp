//
//  Coin.cpp
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include <iostream>
#include "Note.h"
#include "Global.h"

SDL_Surface *Note::ms_pNoteImg1(0);
SDL_Surface *Note::ms_pNoteImg2(0);
SDL_Surface *Note::ms_pNoteImg3(0);
SDL_Surface *Note::ms_pClefImg(0);

const int NOTE_TYPES = 3;// Ignoring Clef
const float NOTE_DANCE_FACTOR = 2.5;

#pragma mark -
#pragma mark CoinFunctions

//Initialise platform variables
Note::Note( int vel, int initialX, int initialY, Floor* pFloor)
{	
    if ( ms_pNoteImg1 == 0 || ms_pNoteImg2 == 0 || ms_pNoteImg3 == 0 || ms_pClefImg == 0 )
        InitNoteImages();
    
    //Gives Note a random image,
	int imgSize = (rand() % NOTE_TYPES)+1;
	switch (imgSize)
	{
        case 4:
			m_pImage = ms_pClefImg;
			break;
            
		case 3:
			m_pImage = ms_pNoteImg3;
			break;
		case 2:
			m_pImage = ms_pNoteImg2;
			break;
		case 1:
			m_pImage = ms_pNoteImg1;
			break;
	}
    
    m_x = initialX - GetWidth();
    m_y = initialY - GetHeight();
    
    m_pFloor = pFloor;
    
	m_yVel = 0;
	m_xVel = vel;
}

Note::~Note()
{
    //SDL_FreeSurface(m_pImage);
}

//Initialise all the static surfaces for the different types of platforms
void Note::InitNoteImages()
{
    if (COINS_ON)
    {
        ms_pNoteImg1 = Global::SharedGlobal()->LoadImage( RESOURCE_COIN );
        ms_pNoteImg2 = Global::SharedGlobal()->LoadImage( RESOURCE_COIN );
        ms_pNoteImg3 = Global::SharedGlobal()->LoadImage( RESOURCE_COIN );
        ms_pClefImg  = Global::SharedGlobal()->LoadImage( RESOURCE_COIN );
    }
    else
    {
        ms_pNoteImg1 = Global::SharedGlobal()->LoadImage( RESOURCE_NOTE1 );
        ms_pNoteImg2 = Global::SharedGlobal()->LoadImage( RESOURCE_NOTE2 );
        ms_pNoteImg3 = Global::SharedGlobal()->LoadImage( RESOURCE_NOTE3 );
        ms_pClefImg  = Global::SharedGlobal()->LoadImage( RESOURCE_CLEF  );
    }
    
    if ( ms_pNoteImg1 == 0 || ms_pNoteImg2 == 0 || ms_pNoteImg3 == 0 || ms_pClefImg == 0 )
    {
		printf( "Could not load note images correctly: %s\n", SDL_GetError() );
		SDL_Quit();
		exit(1);
	}
}

//Updates the platforms position
bool Note::Update( float dt )
{
	m_x -= m_xVel * ( dt / 1000.f );
    
    float heightDiff = m_pFloor->GetNextHeight() - m_pFloor->GetHeight();
    m_y += heightDiff/NOTE_DANCE_FACTOR; // * ( dt / 1000.f );
    
	return true;
}

//Returns true if Coin is within screen dimensions
bool Note::IsVisible()
{
	return ( m_x >=0 && m_x <=SCREEN_WIDTH); // && m_y >= 0 && m_y <= SCREEN_HEIGHT );
}

#pragma mark -
#pragma mark CollisionDetection

//Method which creates a collision box for the note
const GameObject::CollisionBox Note::MyCollisionBox( float dt )
{
	CollisionBox col = CollisionBox();
	col.x1 = m_x;                       
	col.x2 = m_x + this->GetWidth();	
	col.y1 = m_y;						
	col.y2 = m_y + this->GetHeight();   
	
	return col;
}

//Returns true if the object is on this platform - very abstract, as it compares collision boxes which are abstract concepts
bool Note::TouchCoin( GameObject& rObject )
{
	CollisionBox objCol = rObject.MyCollisionBox();
	CollisionBox noteCol = this->MyCollisionBox();
	
	//Check if object box is inside the note box
    //THIS SEEMS LIKE TOO MANY CHECKS, CAN I SIMPLIFY?
	return ( ((noteCol.y2 >= objCol.y1 && noteCol.y1 <= objCol.y1) ||
              (noteCol.y2 <= objCol.y2 && noteCol.y1 >= objCol.y1)||
              (noteCol.y1 <= objCol.y2 && noteCol.y2 >= objCol.y2)) &&
             ((noteCol.x2 >= objCol.x1 && noteCol.x1 <= objCol.x1) ||
              (noteCol.x2 <= objCol.x2 && noteCol.x1 >= objCol.x1)||
              (noteCol.x1 <= objCol.x2 && noteCol.x2 >= objCol.x2)) );
}
