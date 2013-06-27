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

const int EXTRA_SECONDS = 10;

//TODO: 
//  - Decision needs to be made on whether notes gen'd at runtime or statically
// -> Tidy up code! Especially genfloor code!

Floor::Floor( const std::string &filename )
{
    m_filename = filename;
    
    m_fromKeyPointI = m_toKeyPointI = 0;
    m_pointsSize = 0;
    m_offsetX = 0;
    SetFloorPoints();
    SetFloorVertices();
	
    //NOTE: Just to avoid errors - remove altogether if Floor doesn't use surfaces at all
	m_pImage = Global::SharedGlobal()->LoadImage( RESOURCE_PLAYER );
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
    float signature, songDuration, noteFreq;
    
    lvlfile >> songpath;
    lvlfile >> signature;
    lvlfile >> m_levelSpeed;
    lvlfile >> songDuration;
    lvlfile >> noteFreq; 
    lvlfile >> m_pointsSize;
    m_pFloorPoints = new Point[m_pointsSize];
    
    for (int i = 0; i < m_pointsSize; i++)
    {
        int x, y;
        lvlfile >> x;
        lvlfile >> y;
        m_pFloorPoints[i] = Point(x,y);
    }
    
    lvlfile.close();
}

void Floor::SetFloorVertices()
{    
    Point winSize = Point( SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // Key points define the interval which we want to draw hills for
    while ( m_pFloorPoints[m_fromKeyPointI+1].GetX() < m_offsetX-winSize.GetX()/8 )
    {
        m_fromKeyPointI++;
    }
    while ( m_pFloorPoints[m_toKeyPointI].GetX() < m_offsetX+winSize.GetX()*9/8 )
    {
        m_toKeyPointI++;
    }
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
//      or returns the height at the first point a note would appear at on RHS
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
        int i = std::min(m_toKeyPointI, (int)(m_pointsSize*POINT_FREQUENCY*1.5));
        while ( (m_pFloorPoints[i].GetX() - m_offsetX) > SCREEN_WIDTH - WALL_WIDTH) // NOTE_WIDTH/2)
        {
            i--;
        }
        ret = m_pFloorPoints[i].GetY();
    }
    
    return ret;
}

//Updates the floor's state taking time elapsed into account
bool Floor::Update( float dt )
{
    m_offsetX += m_levelSpeed * (dt/1000.f);
    
    SetFloorVertices();
    
	return true;	
}


void Floor::Render( SDL_Surface* pScreen, Camera& rCamera ) const
{
    for(int i = std::max(m_fromKeyPointI, 1); i <= m_toKeyPointI; ++i) 
    {   
        lineColor( pScreen,(m_pFloorPoints[i-1].GetX()-m_offsetX) - rCamera.GetX(), (m_pFloorPoints[i-1].GetY()) - rCamera.GetY(),
                           (m_pFloorPoints[i].GetX()-m_offsetX)   - rCamera.GetX(), (m_pFloorPoints[i].GetY())   - rCamera.GetY(), 0xFFFFFFFF );
    }
    
    //DEBUG!
//    lineColor( pScreen,0, GetHeight(), SCREEN_WIDTH, GetHeight(), 0xFFFF00FF ); //Floor Height
//    lineColor( pScreen,0, SCREEN_HEIGHT*(1/9.f), SCREEN_WIDTH, SCREEN_HEIGHT*(1/9.f), 0xFFFF00FF ); //Camera Bottom
//    lineColor( pScreen,0, SCREEN_HEIGHT*(2/3.f), SCREEN_WIDTH, SCREEN_HEIGHT*(2/3.f), 0xFFFF00FF ); //Camera Top
}

// NOTE: This function is useless as it stands, should just use Render() instead
void Floor::End( SDL_Surface* pScreen, Camera& rCamera ) const
{
    for(int i = std::max(m_fromKeyPointI, 1); i <= m_toKeyPointI; ++i) 
    {        
        lineColor( pScreen,(m_pFloorPoints[i-1].GetX()-m_offsetX) - rCamera.GetX(), (m_pFloorPoints[i-1].GetY()) - rCamera.GetY(),
                           (m_pFloorPoints[i].GetX()-m_offsetX)   - rCamera.GetX(), (m_pFloorPoints[i].GetY())   - rCamera.GetY(), 0xFFFFFFFF );
    }
    
    // DEBUG
//    for(int i = m_fromKeyPointI-20; i <= m_toKeyPointI; ++i) 
//    {        
//        std::cout << m_pFloorPoints[i-1].GetY() << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "OffsetX: " << m_offsetX << std::endl;
}

//Method which creates a collision box for the floor
const GameObject::CollisionBox Floor::MyCollisionBox( float dt )
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
float Floor::ThroughFloor( GameObject& rObject, float dt ) 
{
	const CollisionBox obj   = rObject.MyCollisionBox(dt);
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

// TODO: MAKE THIS FUNCTION CLEARER
bool Floor::GenFloorPoints( std::string lvlpath, std::string songpath )
{
    float signature, tempo, duration, normaliser;
    int option;
    
    std::ifstream ilvlfile(LEVEL_TEMP);
    if ( !ilvlfile.good() )
    {
        printf( "Failed to open file: %s\n", LEVEL_TEMP );
        return false;
    }
    
    ilvlfile >> signature;
    ilvlfile >> tempo;
    ilvlfile >> duration;
    ilvlfile >> normaliser;
    ilvlfile >> option;
    ilvlfile.close();
    
    float levelSpeed = tempo*LEVEL_SPEED_FACTOR;
    float noteFreq = (levelSpeed*HEURISTIC_TIME_LOST * 60/tempo)*2; // (xtravelled * 60/bpm)*2
    
    int originalPointsSize = 0;
    Point* pOriginalPoints;
    
    if (option == OPTION_RANDOM)
    {
        pOriginalPoints = GenRandomPoints(&originalPointsSize);
    }
    else
    {
        pOriginalPoints = GenOriginalPoints(&originalPointsSize);
    }

    int smoothPointsSize = 0;
    Point* pSmoothPoints = GenSmoothPoints(pOriginalPoints, originalPointsSize, &smoothPointsSize);
    
    // DEBUG - Write FINAL_OUTPUT_FORMAT using OriginalPoints!
//    if (GEN_JAGGED_HILLS_ON)
//    {
//        std::ofstream olvlfile( lvlpath.c_str(), std::ios::trunc );
//        olvlfile << songpath << std::endl;
//        olvlfile << levelSpeed << " " << duration << " " << noteFreq << " " << originalPointsSize << std::endl;
//        for (int i = 0; i < originalPointsSize; i++)
//        {
//            olvlfile << pOriginalPoints[i].GetX() << " " << pOriginalPoints[i].GetY() << " ";
//        }
//        olvlfile << std::endl;
//        olvlfile.close();
//    }
//    else
//    {
    
    // Write FINAL_OUTPUT_FORMAT into the file!
    std::ofstream olvlfile;
    olvlfile.open( lvlpath.c_str(), std::ios::trunc );
    if ( !olvlfile.good() )
    {
        printf( "Failed to open file: %s\n", lvlpath.c_str() );
        return false;
    }
    olvlfile << songpath << std::endl;
    olvlfile << signature << " " << levelSpeed << " " << duration << " " << noteFreq << " " << smoothPointsSize << std::endl;
    for (int i = 0; i < smoothPointsSize; i++)
    {
        olvlfile << pSmoothPoints[i].GetX() << " " << pSmoothPoints[i].GetY() << " ";
    }
    olvlfile << std::endl;
    
    delete pOriginalPoints;
    delete pSmoothPoints;
    
    olvlfile.close();
    
    return true;
}

Point* Floor::GenRandomPoints(int* pointsSize)
{
    float signature, tempo, duration;
    
    std::ifstream ilvlfile;
    ilvlfile.open( LEVEL_TEMP );
    if ( !ilvlfile.good() )
    {
        printf( "Failed to open file: %s\n", LEVEL_TEMP );
        return false;
    }
    
    ilvlfile >> signature;
    ilvlfile >> tempo;
    ilvlfile >> duration;
    ilvlfile.close(); 
    
    //DEBUG
    std::cout << "DEBUG: Option: " << OPTION_RANDOM << ", Max: N/A, Normaliser: N/A " << std::endl;
    
    float levelSpeed = tempo*LEVEL_SPEED_FACTOR;
    //    float noteFreq = (levelSpeed*HEURISTIC_TIME_LOST * 60/tempo)*2; // (xtravelled * 60/bpm)*2
    float xTravelledPerSecond = levelSpeed*HEURISTIC_TIME_LOST;
    float xMaxDistance = duration * xTravelledPerSecond;
    *pointsSize = ceil( (duration) * (xTravelledPerSecond/POINT_FREQUENCY) );
    Point* pRandomPoints = new Point[*pointsSize];
    
    float minDX = 100;
    float minDY = 60;
    int rangeDX = 200;
    int rangeDY = 400;
    
    float x = -minDX;
    float y = SCREEN_HEIGHT/2-minDY;
    
    float dy, ny;
    float sign = 1; // +1 - going up, -1 - going  down
    float paddingTop = 50;
    float paddingBottom = 100;
    
    // Here we generate floor points pseudorandomly going up and down forming hills
    srand(signature);
    int i;
    for (i=0; i<*pointsSize; i++)
    {
        pRandomPoints[i] = Point(x, y);
        if ( i == 0 )
        {
            x = 0;
            y = SCREEN_HEIGHT/2;
        }
        else
        {
            x += rand()%rangeDX+minDX;
            while ( true )
            {
                dy = rand()%rangeDY+minDY;
                ny = y + dy*sign;
                if ( ny < SCREEN_HEIGHT-paddingTop && ny > paddingBottom )
                {
                    break;
                }
            }
            y = ny;
        }
        sign *= -1;
        
        if (x >= xMaxDistance)
            break;
    }
    *pointsSize = i;
    
    // Pad the End of the level so that it continues!
    int origPointSize = *pointsSize;
    *pointsSize += EXTRA_SECONDS;
    for (int i = origPointSize; i < *pointsSize; i++)
    {
        int x = pRandomPoints[i-1].GetX() + xTravelledPerSecond;
        int y = pRandomPoints[i-1].GetY();
        pRandomPoints[i] = Point(x,y);
    }
    
    return pRandomPoints;
}

Point* Floor::GenOriginalPoints(int* pointsSize)
{
    std::ifstream ilvlfile(LEVEL_TEMP);
    if ( !ilvlfile.good() )
    {
        printf( "Failed to open file: %s\n", LEVEL_TEMP );

    }
    
    //Locals:
    float signature, tempo, duration, normaliser, sampling;
    int option, envSize;
    
    ilvlfile >> signature;
    ilvlfile >> tempo;
    ilvlfile >> duration;
    ilvlfile >> normaliser;
    ilvlfile >> option;
    ilvlfile >> sampling;
    ilvlfile >> envSize;
    
    float* envelopeArr = new float[envSize];
    float maxVal = 0;
    for (int i = 0; i < envSize; i++)
    {
        ilvlfile >> envelopeArr[i];
        maxVal = std::max(maxVal, envelopeArr[i]);
    }
    ilvlfile.close();


    //DEBUG
    std::cout << "DEBUG: Option: " << option << ", Max: " << maxVal << ", Normaliser: " << normaliser << std::endl;
    
    if (option != OPTION_AMPLITUDE)
    {
        for (int i = 0; i < envSize; i++)
            envelopeArr[i] *= (normaliser/maxVal);
    }
    
    
    // Adjust Normaliser: Normaliser = (LargestVal/MaxNorm * Range) + MinNorm!
    // OR just Increase by a small amount - with larger numbers the increase is less noticeable hence similar effect
//    normaliser = (normaliser/MAX_FLOOR_NORMALISER) * (MAX_FLOOR_NORMALISER - MIN_FLOOR_NORMALISER) + MIN_FLOOR_NORMALISER;
    normaliser += INCREASE_FLOOR_NORMALISER;
    
    float levelSpeed = tempo*LEVEL_SPEED_FACTOR;
    float xTravelledPerSecond = levelSpeed*HEURISTIC_TIME_LOST;
    float dataUsedPerPoint = sampling / (xTravelledPerSecond/POINT_FREQUENCY);
    
    int origPointSize = ceil( (duration) * (xTravelledPerSecond/POINT_FREQUENCY) );
    *pointsSize = origPointSize + EXTRA_SECONDS; //Take padding into account
    Point* pOriginalPoints = new Point[*pointsSize];
    
    for (int i = 1; i < origPointSize; i++)
    {
        int x = POINT_FREQUENCY*i + (WALL_WIDTH + PLAYER_WIDTH);
        float y = 0;
        
        // Sums values between given interval
        for ( int j = (i-1)*dataUsedPerPoint; j < i*dataUsedPerPoint; j++ )
            y += envelopeArr[j];
        
        // Find mean of y
        y /= (dataUsedPerPoint * normaliser);
        
        // Now convert this to be inbetween 0 and LEVEL_HEIGHT
        y *= LEVEL_HEIGHT; 
//        y += LEVEL_PADDING;
//        if (y >= LEVEL_HEIGHT)
//            y = LEVEL_HEIGHT;
        
        pOriginalPoints[i] = Point(x,y);
    }
    pOriginalPoints[0] = Point(0, pOriginalPoints[1].GetY());
    
    // Pad the End of the level so that it continues!
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
    // CHECK: What should the exact size be?
    Point* pSmoothPoints = new Point[originalPointsSize*100];
    
    // The following code uses cosine approximations to make the hills smooth
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