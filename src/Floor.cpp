//
//  Floor.cpp
//  Fever
//
//  Created by Arthur Fox on 21/02/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "Floor.h"
#include "Global.h"
#include "SDL_gfxPrimitives.h"
#include <iostream>
#include <fstream>
#include <thread>


// FIRST AND FOREMOST, THIS CODE IS NOT COMPLICATED...
//  SO DO NOT PANIC WHEN YOU COME BACK TO IT!

//TODO: 
// -> Gen level with a real song:
//  - genLevel asks matlab to analyse music
//  - call genfloor using values from analysed music!
//  - Decision needs to be made on whether coins gen'd at runtime or statically
// -> Tidy up code! Especially genfloor code!

float Floor::m_COPYoffsetX;
int Floor::m_updateCalled;

Floor::Floor( const std::string &filename )
{
    m_filename = filename;
    
    m_fromKeyPointI = m_toKeyPointI = 0;
    m_offsetX = 0;
    SetFloorPoints();
    SetFloorVertices();
    
    //NOTE: TEMPORARY
    m_COPYoffsetX = 0;
//    std::thread t( ThreadFunc );
//    t.detach();
	
    //NOTE: Just to avoid errors - remove altogether if Floor doesn't use surfaces at all
	m_pImage = Global::SharedGlobal()->LoadImage( RESOURCE_PLAYER );
    
    //DEBUG
    m_updateCalled = 0;
}

//NOTE: DEBUG - DELETE this later
void Floor::ThreadFunc()
{
    for (int i = 0; i < 216; i++)
    {
        std::cout << i << ": " << m_COPYoffsetX << ", " << m_updateCalled << std::endl;
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}

Floor::~Floor()
{
    SDL_FreeSurface( m_pImage );
    delete m_pFloorPoints;
}

// TODO: needs to read-in the files floor points to m_floorPoints
void Floor::SetFloorPoints()
{
    std::ifstream lvlfile;
    lvlfile.open( m_filename.c_str() );
    std::string songpath;
    float songDuration; 
    int arrSize;
    
    lvlfile >> songpath;
    lvlfile >> m_levelSpeed;
    lvlfile >> songDuration; 
    lvlfile >> arrSize;
    m_pFloorPoints = new Point[arrSize];
    
    for (int i = 0; i < arrSize; i++)
    {
        int x, y;
        lvlfile >> x;
        lvlfile >> y;
        m_pFloorPoints[i] = Point(x,y);
    }
    
    
    lvlfile.close();
    
    //DEBUG - TEMPORARY
//    m_COPYoffsetX = arrSize;
//    std::cout << "Total Points: " << arrSize << std::endl;
}

void Floor::SetFloorVertices()
{    
    Point winSize = Point( SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // Key points define the interval which we want to draw hills for
    while ( m_pFloorPoints[m_fromKeyPointI+1].GetX() < m_offsetX-winSize.GetX()/8 ) {
        m_fromKeyPointI++;
    }
    while ( m_pFloorPoints[m_toKeyPointI].GetX() < m_offsetX+winSize.GetX()*9/8 ) {
        m_toKeyPointI++;
    }
    
    //DEBUG - TEMPORARY
//    if (m_toKeyPointI >= m_COPYoffsetX )
//        std::cout << "Ran out of Points!" << std::endl;
}

//NOTE: CURRENTLY NO USE FOR THIS FUNCTION
float Floor::LerpHeight()
{
    int i = std::max(m_fromKeyPointI, 1);
    
    while ( (m_pFloorPoints[i].GetX() - m_offsetX) < WALL_WIDTH + PLAYER_WIDTH/2)
    {
        i++;
    }
    
    float x0 = m_pFloorPoints[i-1].GetX() - m_offsetX;
    float x1 = m_pFloorPoints[i  ].GetX() - m_offsetX;
    float y0 = m_pFloorPoints[i-1].GetY();
    float y1 = m_pFloorPoints[i  ].GetY();
    
    //We have x = WALL_WIDTH, want to find out the y!
    float y = y0 + (y1 - y0)*(( (WALL_WIDTH + PLAYER_WIDTH/2 ) - x0)/(x1 - x0));
    
    return y;
}

// Returns floor height at either the start of the player + at (at defaults to 0) 
//      or returns the height at the first point a coin would appear at on RHS
float Floor::FindHeight( int at ) const
{
    float ret = 0;
    
    if( at >=0)
    {   
        int i = std::max(m_fromKeyPointI, 1);
        while ( (m_pFloorPoints[i].GetX() - m_offsetX) < WALL_WIDTH + PLAYER_WIDTH/2)
        {
            i++;
        }
        ret = m_pFloorPoints[i + at].GetY();
    }
    else
    {
        int i = std::min(m_toKeyPointI, (MAX_FLOOR_POINTS*MAX_FLOOR_POINTS-1));
        while ( (m_pFloorPoints[i].GetX() - m_offsetX) > SCREEN_WIDTH - COIN_WIDTH/2)
        {
            i--;
        }
        ret = m_pFloorPoints[i].GetY();
    }
    
    return ret;
}

//Updates the floor's state taking time elapsed into account
bool Floor::Update( float deltaTicks )
{	
    //DEBUG
    m_updateCalled++;
    
    //TODO: FIX THIS
    // THE GAME WILL NOT WORK IF THIS IS NOT RELIABLE/PREDICTABLE
    m_offsetX += m_levelSpeed * (deltaTicks/1000.f);
    m_COPYoffsetX = m_offsetX;  //DEBUG
    
    SetFloorVertices();
    
	return true;	
}


void Floor::Render( SDL_Surface* pScreen ) const
{
    for(int i = std::max(m_fromKeyPointI, 1); i <= m_toKeyPointI; ++i) 
    {   
        lineColor( pScreen,m_pFloorPoints[i-1].GetX()-m_offsetX, m_pFloorPoints[i-1].GetY(), m_pFloorPoints[i].GetX()-m_offsetX, m_pFloorPoints[i].GetY(), 0xFFFFFFFF );
    }
    
    //DEBUG!
//    lineColor( pScreen,0, GetHeight(), SCREEN_WIDTH, GetHeight(), 0xFFFF00FF );
}

// NOTE: This function is useless as it stands, should just use Render() instead
void Floor::End( SDL_Surface* pScreen )
{
    for(int i = std::max(m_fromKeyPointI, 1); i <= m_toKeyPointI; ++i) 
    {        
        lineColor( pScreen,m_pFloorPoints[i-1].GetX()-m_offsetX, m_pFloorPoints[i-1].GetY(), m_pFloorPoints[i].GetX()-m_offsetX, m_pFloorPoints[i].GetY(), 0xFFFFFFFF );
    }
    
    // DEBUG
    for(int i = m_fromKeyPointI-20; i <= m_toKeyPointI; ++i) 
    {        
        std::cout << m_pFloorPoints[i-1].GetY() << " ";
    }
    std::cout << std::endl;
    std::cout << "OffsetX: " << m_offsetX << std::endl;
    std::cout << "Updates: " << m_updateCalled << std::endl;
}

//Method which creates a collision box for the floor
const GameObject::CollisionBox Floor::MyCollisionBox( float deltaTicks )
{
	CollisionBox col = CollisionBox();
	col.x1 = 0;	
	col.x2 = SCREEN_WIDTH;
	col.y1 = GetHeight();
	col.y2 = SCREEN_HEIGHT;
	
	return col;
}

//Returns true if the object is on the floor - compares collision boxes which are abstract concepts
bool Floor::OnFloor( GameObject& rObject )
{
	const CollisionBox obj   = rObject.MyCollisionBox();
	const CollisionBox floor = this->MyCollisionBox();
	
	//Check if object box is inside the floor box
	return (floor.y1 <= obj.y2);
}

//Returns the floor height value if the object has just gone through it, -1 otherwise
float Floor::ThroughFloor( GameObject& rObject, float deltaTicks )
{
	const CollisionBox obj   = rObject.MyCollisionBox(deltaTicks);
	const CollisionBox floor = this->MyCollisionBox();
	
    //The floor is considered as a single line, only its top y value is used
    if(floor.y1 <= obj.y2)
    {
        return GetHeight();
    }
    
	return -1;
}

#pragma mark -
#pragma mark LevelGeneration

// TODO: REWRITE THIS FUNCTION TO MAKE IT CLEARER AND MORE CORRECT
bool Floor::GenFloorPoints( std::string lvlpath, std::string songpath )
{
    
    std::ifstream ilvlfile;
    ilvlfile.open( LEVEL_TEMP );
    if ( !ilvlfile.good() )
    {
        printf( "Failed to open file: %s\n", LEVEL_TEMP );
        return false;
    }
    
    //Locals:
    float signature, tempo, duration;
    
    ilvlfile >> signature;
    ilvlfile >> tempo;
    ilvlfile >> duration;
    ilvlfile.close();
    
    float levelSpeed = tempo*LEVEL_SPEED_FACTOR;
    
    int originalPointsSize = 0;
    Point* pOriginalPoints = GenOriginalPoints(&originalPointsSize);

    int smoothPointsSize = 0;
    Point* pSmoothPoints = GenSmoothPoints(pOriginalPoints, originalPointsSize, &smoothPointsSize);
    
    // DEBUG - Write FINAL_OUTPUT_FORMAT using OriginalPoints!
//    std::ofstream olvlfile( lvlpath.c_str(), std::ios::trunc );
//    olvlfile << songpath << std::endl;
//    olvlfile << levelSpeed << " " << duration << " " << originalPointsSize << std::endl;
//    for (int i = 0; i < originalPointsSize; i++)
//    {
//        olvlfile << pOriginalPoints[i].GetX() << " " << pOriginalPoints[i].GetY() << " ";
//    }
//    olvlfile << std::endl;
//    olvlfile.close();
    
    // Write FINAL_OUTPUT_FORMAT into the file!
    std::ofstream olvlfile( lvlpath.c_str(), std::ios::trunc );
    olvlfile << songpath << std::endl;
    olvlfile << levelSpeed << " " << duration << " " << smoothPointsSize << std::endl;
    for (int i = 0; i < smoothPointsSize; i++)
    {
        olvlfile << pSmoothPoints[i].GetX() << " " << pSmoothPoints[i].GetY() << " ";
    }
    olvlfile << std::endl;
    olvlfile.close();
    
    delete pOriginalPoints;
    delete pSmoothPoints;
    
    return true;
}

Point* Floor::GenOriginalPoints(int* pointsSize)
{
    //    PARTIAL_OUTPUT_FORMAT:
    //    Signature
    //    Tempo 
    //    SongDuration(secs)
    //    EnvelopeSampling 
    //    EnvelopeArrSize
    //    [EnvelopeArr]
    
    std::ifstream ilvlfile;
    ilvlfile.open( LEVEL_TEMP );
    if ( !ilvlfile.good() )
    {
        printf( "Failed to open file: %s\n", LEVEL_TEMP );

    }
    
    //Locals:
    float signature, tempo, duration, sampling;
    int envSize;
    
    ilvlfile >> signature;
    ilvlfile >> tempo;
    ilvlfile >> duration;
    ilvlfile >> sampling;
    ilvlfile >> envSize;
    
    float* envelopeArr = new float[envSize];
    
    float maxVal = 0;
    for (int i = 0; i < envSize; i++)
    {
        ilvlfile >> envelopeArr[i];
        maxVal = std::max(maxVal,envelopeArr[i]);
    }
    ilvlfile.close();
    
    float levelSpeed = tempo*LEVEL_SPEED_FACTOR;
    float xTravelledPerSecond = levelSpeed*HEURISTIC_TIME_LOST; 
    *pointsSize = ceil( (duration) * (xTravelledPerSecond/POINT_FREQUENCY) );
    Point* pOriginalPoints = new Point[*pointsSize];
    
    float dataUsedPerPoint = sampling / (xTravelledPerSecond/POINT_FREQUENCY); 
    
    for (int i = 1; i < *pointsSize; i++)
    {
        int x = POINT_FREQUENCY*i + (WALL_WIDTH + PLAYER_WIDTH);
        float y = 0;
        
        // Sums values between given interval
        for ( int j = (i-1)*dataUsedPerPoint; j < i*dataUsedPerPoint; j++ )
            y += (envelopeArr[j]/maxVal);
        
        // Find mean of y
        y /= dataUsedPerPoint;
        
        // Now convert this to be inbetween 0 and SCREEN_HEIGHT and include padding
        // - Values are currently [0,0.5] so y*SCREEN_HEIGHT*2?
        y *= LEVEL_HEIGHT; 
        y += LEVEL_PADDING;
        if (y >= LEVEL_HEIGHT) y = LEVEL_HEIGHT;
        
        pOriginalPoints[i] = Point(x,y);
    }
    pOriginalPoints[0] = Point(0, pOriginalPoints[1].GetY());
    
    // Pad the End of the level so that it continues!
    int extraSeconds =  10; //NOTE: This should Not be completely arbitrary
    int origPointSize = *pointsSize;
    *pointsSize += extraSeconds;
    for (int i = origPointSize; i < *pointsSize; i++)
    {
        int x = pOriginalPoints[i-1].GetX() + xTravelledPerSecond;
        int y = pOriginalPoints[i-1].GetY();
        pOriginalPoints[i] = Point(x,y);
    }
    
    delete envelopeArr;
    
    return pOriginalPoints;
}

Point* Floor::GenSmoothPoints(Point* pOriginalPoints, int originalPointsSize, int* smoothPointsSize)
{
    // CHECK!
    //? What should the exact size be? Does it matter? Is this leading to crashes?
    Point* pSmoothPoints = new Point[originalPointsSize*originalPointsSize];
    
    //    // The following code uses cosine approximations to make the hills smooth
    int idx = 0;
    for ( int i = 0; i < originalPointsSize-1; i++ )
    {
        Point p0 = pOriginalPoints[i];
        Point p1 = pOriginalPoints[i+1];
        int hSegments = floorf( (p1.GetX()-p0.GetX())/SEGMENT_WIDTH );
        float dx = (p1.GetX() - p0.GetX()) / hSegments;
        float da = M_PI / hSegments;
        float ymid = (p0.GetY() + p1.GetY()) / 2;
        float ampl = (p0.GetY() - p1.GetY()) / 2;
        
        Point pt0, pt1;
        pt0 = p0;
        for (int j = 0; j < hSegments+1; ++j)
        {            
            pt1.SetX( p0.GetX() + j*dx );
            pt1.SetY( ymid + ampl * cosf(da*j) );
            
            pSmoothPoints[idx] = pt0;
            idx++;
            
            pt0 = pt1;
        }
    }
    
    *smoothPointsSize = idx;
    
    return pSmoothPoints;
}

//  THIS IS THE ORIGINAL VERSION WHICH WORKED TO MAKE RANDOM LEVELS!
//  KEEPING THIS IN CASE THE ABOVE VERSION F'S UP
//
//float minDX = 100;
//float minDY = 60;
//int rangeDX = 200;
//int rangeDY = 400;
//
//float x = -minDX;
//float y = winSize.GetY()/2-minDY;
//
//float dy, ny;
//float sign = 1; // +1 - going up, -1 - going  down
//float paddingTop = 50;
//float paddingBottom = 100;
//
//// Here we generate floor points pseudorandomly going up and down forming hills
//for (int i=0; i<MAX_FLOOR_POINTS; i++) 
//{
//    pTempPoints[i] = Point(x, y);
//    if ( i == 0 ) 
//    {
//        x = 0;
//        y = winSize.GetY()/2;
//    } 
//    else 
//    {
//        x += rand()%rangeDX+minDX;
//        while ( true ) 
//        {
//            dy = rand()%rangeDY+minDY;
//            ny = y + dy*sign;
//            if ( ny < winSize.GetY()-paddingTop && ny > paddingBottom ) 
//            {
//                break;   
//            }
//        }
//        y = ny;
//    }
//    sign *= -1;
//}
//
//
//// The following code uses cosine approximations to make the hills smooth
//int idx = 0;
//for (int i = 0; i < MAX_FLOOR_POINTS; i++)
//{
//    Point p0 = pTempPoints[i-1];
//    Point p1 = pTempPoints[i];
//    int hSegments = floorf( (p1.GetX()-p0.GetX())/SEGMENT_WIDTH );
//    float dx = (p1.GetX() - p0.GetX()) / hSegments;
//    float da = M_PI / hSegments;
//    float ymid = (p0.GetY() + p1.GetY()) / 2;
//    float ampl = (p0.GetY() - p1.GetY()) / 2;
//    
//    Point pt0, pt1;
//    pt0 = p0;
//    for (int j = 0; j < hSegments+1; ++j) 
//    {            
//        pt1.SetX( p0.GetX() + j*dx );
//        pt1.SetY( ymid + ampl * cosf(da*j) );
//        
//        pFloorPoints[idx] = pt0;
//        idx++;
//        
//        pt0 = pt1;
//    }
//    
//}
//
//// Here I need to write pFloorPoints into the file!
//lvlfile << DEFAULT_LEVEL_SPEED << " " << idx << std::endl;
//for (int i = 0; i < idx; i++)
//{
//    lvlfile << pFloorPoints[i].GetX() << " " << pFloorPoints[i].GetY() << " ";
//}
//lvlfile << std::endl;
//
//return true;
//