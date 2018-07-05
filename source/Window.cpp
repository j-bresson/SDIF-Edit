/*
 *  Window.cpp
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

//#define GLUTCHECKLOOP

#include "Window.h"

std::map<int, Window *> Window::windowsMap;
bool Window::STOP_EXE;
int Window::closeWinID;


Window::Window(){
}

int Window::getID(){
    return ID;
}

void Window::createWindow(){
    // Initialisation et creation de la fenetre GLUT
    glutInitWindowSize(width, height);
    ID = glutCreateWindow(windowTitle);
    // Callbacks pour la fenetre courante
    glutDisplayFunc(&Window::displayCallback);
    glutKeyboardFunc(&Window::keyboardCallback);
    glutSpecialFunc(&Window::SpecialCallback);
    glutMouseFunc(&Window::MouseCallback);
    glutMotionFunc(&Window::MouseMotionCallback);
    glutIdleFunc(&Window::IdleCallback);
    glutReshapeFunc(&Window::reshapeCallback);
    // Fonction de fermeture (si elle existe dans la liubrairie GLUT utilisee)
#if defined(GLUTWMCLOSEFUNC)
    glutWMCloseFunc(&Window::closeCallback);
#endif
    windowsMap[ID] = this;
}    

void Window::start(int *argc, char **argv){
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    // Initialisation SDIF
    //SdifGenInit("");
    
    STOP_EXE = false;
    closeWinID = -1;
}

void Window::closeWindow(int Id){
	std::cout << "close window" << std::endl;
    Window *win = getWindow(Id);
    delete win;   
    closeWinID = -1;
}

int Window::closeMe(int id){
	std::cout << "close me" << std::endl;
    if(closeWinID == -1){
		closeWinID = id;
		return 0;
    } else { 
		return -1;
    }
}


void Window::show(int Id){
     Window *win = getWindow(Id);
     win->showWindow();
}


int Window::mainLoop(){

#ifndef GLUTCHECKLOOP
    glutMainLoop();       
#else   
    for(;;){
	// fermer l'eventuelle fenêtre en attente de fermeture
    	if(closeWinID != -1) closeWindow(closeWinID);
	// tester fin d'execution
	if (!STOP_EXE) 
	    glutCheckLoop();
	else break;
    }
#endif
	std::cout << "Leaving event loop.." << std::endl;
    return 1;
}


void Window::showWindow(){
    glutSetWindow(ID);
    glutShowWindow();
    visible = true;
    glutPostRedisplay();
}

void Window::hideWindow(){
    glutSetWindow(ID);
    glutHideWindow();
    visible = false;
}

bool Window::isVisible(){
    return visible;
}

void Window::orthoProj(){
    // reset matrix
    glLoadIdentity();
    // set a 2D orthographic projection
    gluOrtho2D(0, width, 0, height);
    // invert the y axis, down is positive
    glScalef(1, -1, 1);
    // mover the origin from the bottom left corner
    // to the upper left corner
    glTranslatef(0, -height, 0);
}

/* Recuperation de la fenetre courante pour application des fonctions appel√©es */
Window *Window::getCurrentWindow() {
    int id = glutGetWindow();
    std::map<int, Window *>::iterator it = windowsMap.find(id);
    if(it == windowsMap.end())
	return 0;
    return it->second;
}

Window *Window::getWindow(int Id) {
    std::map<int, Window *>::iterator it = windowsMap.find(Id);
    if(it == windowsMap.end())
	return 0;
    return it->second;
}

/* Fonction d'affichage */
void Window::displayCallback() {
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // affichage
    w->draw();
}

/* Fonction clavier */
void Window::keyboardCallback(unsigned char key, int x, int y) {
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel de la fonction associee
    w->keyPressed(key, x, y);
}

/* Fonction touches speciales du clavier */
void Window::SpecialCallback(int key, int x, int y) {
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel de la fonction associee
    w->specialPressed(key, x, y);
}

/* Fonction souris */
void Window::MouseCallback(int mouse, int state, int x, int y) {
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel de la fonction mousePressed
    w->mousePressed(mouse, state, x, y);
    
}

/* Fonction mouvement de souris */
void Window::MouseMotionCallback(int x, int y) {
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel de la fonction associee
    //if(glutGetModifiers()==GLUT_ACTIVE_CTRL)
    w->mouseMoved(x, y);
}

/* Fonction dedimensionnement de la fenetre */
void Window::reshapeCallback(int width, int height){
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel a la fonction associee
    w->reshape(width,height);
}

/* Fonction idle de la fenetre */
void Window::IdleCallback(){
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel a la fonction associee
    w->idleFunc();
}

/* Fonction fermeture de la fenetre */
void Window::closeCallback(){
    Window *w = getCurrentWindow();
    if(!w)
	return;
    // appel a la fonction associee
    w->close();
}

void Window::quit(){
    // exit(0);
    STOP_EXE = true;
	std::cout << "QUIT" << std::endl;
}
