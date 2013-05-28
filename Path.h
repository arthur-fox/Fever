//
//  Path.h
//  Climber
//
//  Created by Arthur Fox on 25/07/2012.
//  Copyright (c) 2012 OMA. All rights reserved.
//

#ifndef Climber_Path_h
#define Climber_Path_h

#include <string>

class Path 
{
public:
    
    static std::string PathForFile(const std::string& filename);
    static std::string NameFromPath(const std::string& path);
};


#endif
