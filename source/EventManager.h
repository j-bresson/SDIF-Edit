/*
 *  EventManager.h
 *
 *  SDIF-Edit: 3D vizualizer for SDIF files
 *  Copyright (C) 2003 Jean Bresson, IRCAM
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING. If not, write to the
 *  Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

/*
#if defined(__MACOSX_CORE__)
#include <ApplicationServices/ApplicationServices.h>
#include "MoreProcesses.h"
#endif
*/

#include "include.h"
#include "Window.h"

class EventManager {

private :

    static void setFrontProcess();
    
public :

    EventManager();
    ~EventManager();

/*
#if defined(__MACOSX_CORE__)
    static OSErr AEOpenSdifFile(const AppleEvent *messagein,
					      AppleEvent *reply,
					      long refIn);
    static OSErr AEOpenSdifFileWithAudio(const AppleEvent *messagein,
					      AppleEvent *reply,
					      long refIn);
    static OSErr AEQuitApplication(const AppleEvent *messagein,
						 AppleEvent *reply,
						 long refIn);
    static OSErr AEShowWindow(const AppleEvent *messagein,
						 AppleEvent *reply,
						 long refIn);
#endif
*/
						 
    static void SendCloseEvent(int winID);

    static int openSdifFile(char *fileName);
    static int openSdifFileWithAudio(char *fileName, char *audioFile);
    static void quitApp();
    static void showWindow(int winId);

};

#endif

