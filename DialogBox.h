//
//  DialogBox.h
//  Fever
//
//  Created by Arthur Fox on 08/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

//#import <Foundation/Foundation.h>
//
//@interface DialogBox : NSObject
//
//@end


#ifndef Climber_DialogBox_h
#define Climber_DialogBox_h

#include <string>

class DialogBox 
{
public:
    
    static std::string OpenFileBrowser(const std::string filetypes = "NULL");
};


#endif
