/*  Window.h
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

 
 /**********************************************************
  * This class is the superclass for all application windows
  * and handles static management of the execution loop
  * Uses GLUT for Mac OSX 10.7 (GLUT-3.2)
  ******************************************************** */

#ifndef WINDOW_H
#define WINDOW_H



#include "include.h"
#include <map>

class Window {

protected:
    
    // attributs d'une fenetre
    int width; 
    int height;
    char* windowTitle;
    int ID;
    bool visible;
    
    // variable de controle de l'execution
    static bool STOP_EXE;	// signal de sortie de la boucle d'évenements
    static int closeWinID;	// identifiant de la fenêtre à fermer (ou -1 si aucune)
    
public:

    Window();
    virtual ~Window(){};
    
    // Returns window ID
    int getID();
    // Creates a new GLUT window
    void createWindow();
    // returns visibility of the window
    bool isVisible();
    // makes window visible
    void showWindow();
    // makes window hidden
    void hideWindow();
    // makes projection matrix of the current GL context
    // as an orthographic projection (used for drawing 2D graphics
    // such as user interfcace,...
    void orthoProj();    
    
    // Acces statique aux fonctions appelees par GLUT
    static void displayCallback();
    static void keyboardCallback(unsigned char key, int x, int y);
    static void SpecialCallback(int key, int x, int y);
    static void MouseCallback(int mouse, int state, int x, int y);
    static void MouseMotionCallback(int x, int y);
    static void reshapeCallback(int w, int h);
    static void IdleCallback();
    static void closeCallback();
    
    
    // Ces methodes doivent être redéfinies par les classes heritant de Window 
    virtual void reshape(int w, int h){};
    virtual void keyPressed( unsigned char key, int x, int y ){};
    virtual void specialPressed( int a_keys, int x, int y ){};
    virtual void mousePressed(int mouse, int state, int x, int y){};
    virtual void mouseMoved(int x, int y){};
    virtual void draw(){};
    virtual void idleFunc(){};
    virtual void close(){};


    // GESTION STATIQUE DE L4EXECUTION
    // Demarrage
    static void start(int *argc, char **argv);
    // entrée dans la boucle d'évenemennts
    static int mainLoop();
    // sortie de l'application
    static void quit();
    // affichage d'une fenetre d'identifiant Id
    static void show(int Id);
    // fermeture de la fenetre Id
    static void closeWindow(int Id);
    
    // appel d'une fenêtre à la classe window pour être fermée
    // return 0 si la fermeture est acceptée, -1 sinon (autre fenetre déjà
    // en attente de fermeture)
    static int closeMe(int Id);

    
    // MULTIPLE WINDOW MANAGING
    // windows table
    static std::map<int, Window *> windowsMap;
    // returns current window
    static Window *getCurrentWindow();
    // returns a window with ID
    static Window *getWindow(int Id);
    
};




#endif


