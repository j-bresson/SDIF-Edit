/*
 *  EventManager.cpp
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

#include "EventManager.h"
#include "MainWindow.h"

EventManager::EventManager(){}
EventManager::~EventManager(){}



void EventManager::SendCloseEvent(int winID){}


void EventManager::setFrontProcess(){}


int EventManager::openSdifFile(char *fileName){
    MainWindow* win = new MainWindow(fileName);
	setFrontProcess();
	return win->getID();
}

int EventManager::openSdifFileWithAudio(char *fileName, char *audioFile){
    MainWindow* win = new MainWindow(fileName);
    win->setAudioFileName(audioFile);
    
    setFrontProcess();
    
    return win->getID();
}

void EventManager::quitApp(){
    Window::quit();
}

void EventManager::showWindow(int winId){
    
    setFrontProcess();
    
    MainWindow* w = (MainWindow*) Window::getWindow(winId);
    if(w->getEditorID() == -1) 
	Window::show(winId);
    else 
	Window::show(w->getEditorID());
    
}



