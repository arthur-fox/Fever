//
//  DialogBox.m
//  Fever
//
//  Created by Arthur Fox on 08/03/2013.
//  Copyright (c) 2013 OMA. All rights reserved.
//

#include "DialogBox.h"

#include <Foundation/Foundation.h>
#include <ApplicationServices/ApplicationServices.h>

std::string DialogBox::OpenFileBrowser(const std::string filetypes) 
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    NSString *selectedFileName = [NSString stringWithCString:"NULL" encoding:NSUTF8StringEncoding];

    NSOpenPanel *panel = [[NSOpenPanel alloc] init];
    panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:NO];
    
    if (filetypes.compare("NULL") != 0)
    {
        NSString* types = [NSString stringWithCString:filetypes.c_str() encoding:NSUTF8StringEncoding];
        NSArray* fileTypes = [types componentsSeparatedByString:@","];
        [panel setAllowedFileTypes:fileTypes];
    }
    
    if ([panel runModal] == NSOKButton)
    {
        selectedFileName = [[[panel URLs] objectAtIndex:0] path];
    }
    
    std::string selectedFile = [selectedFileName cStringUsingEncoding:NSUTF8StringEncoding];
    
    [pool release];
    return selectedFile;
}