//
//  CoinManager.h
//  Fever
//
//  Created by Arthur Fox on 02/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#ifndef Fever_CoinManager_h
#define Fever_CoinManager_h

#include <vector>
#include "GameObject.h"
#include "Note.h"
#include "Floor.h"

class NoteManager
{
public:
    
	NoteManager( float speed, float freq, int width, int height, Floor* pFloor );
	
	inline int GetSpeed() const {return m_speed;}
	inline int GetTouched() const {return m_numTouched;}
    void SetSpeed( int s );
    
	void Update( float delta );
	void Render( SDL_Surface* pScreen, Camera& rCamera);
    
    //Adds notes to notes vector, returns true if note added, false otherwise
	bool AddNote( int initialX = -1, int initialY = -1 );
    int TouchNote( GameObject& rObject );
    Note Remove( int num );
	void ClearInvisible();
    
    inline bool IsCreatingNotes() const {return m_creatingNotes;}
    inline void SetCreatingNotes(bool flag) {m_creatingNotes = flag;}
    
private:	
	//Vector of notes
	std::vector<Note> notes;
    
    //Iterator for notes
    std::vector<Note>::iterator m_it;
	
    //Reference to the level's floor
    Floor* m_pFloor;
    
	//Coins' attributes
	int m_width, m_height, m_numTouched;
    
    //Speed and Frequency of notes - corresponds to bpm
    float m_speed, m_noteFreq;
    
    //Flag saying whether notes are being created or not
    bool m_creatingNotes;
	
	//Creates new note and adds to notes vector
	void NewNote( int initialX = -1, int initialY = -1 );
};

#endif
