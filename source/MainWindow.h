/*
 *  Class MainWindow
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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "include.h"

#include "EventManager.h"
#include "Window.h"
#include "SDIFDescriptor.h"
#include "GUI.h"

/**
 * Startup widow (SDIF stream selection)
 */
class MainWindow : public Window {
    
private:
    
    SDIFDescriptor* sdd;	// file structure infoirmation
    char* fileName;		    // file name 
    char* audioFileName;	// associated audio file
    
    Window *editWin;		// Visualization window
    int editorID;		    // Visualization window ID
    
    int nbMatrices;		
    int selectedMatrixNum;	
    int nbStr;			
    int errFlag;		
    char* errMsg;		
    
    // Visualisation
    void* font;			
    int space;			// space between lines (pixels)
	
    // Composants de l'interface
    GUIComponent* fullPanel;
    GUIComponent* fileLabel;
    GUIComponent* selectButton;
    GUIComponent* partialsButton;
    GUIComponent** streamLabels;
    GUIComponent** matrixButtons;
    GUIComponent* tmpLabel1;
    GUIComponent* tmpLabel2;
    GUIComponent* closeButton;
    
    
    // Lecture du fichier SDIF et extraction des informations.
    // description envoye sur standard output.
    // return 1 si fichier lu correctement
    // ou 0 si erreur de lecture
    int readSdifFile();
    
    // Selection d'un matrice
    void selectMatrix(int matNum);
    
    // initialisation des composants graphiques
    void setComponents();
    // initialisation du contexte OpenGL
    void initGL();
    // affichage du menu de selection
    void drawGUI();
    
    
    public :
	
    // Constructeur
    MainWindow(const char *_fileName, int width = 500, int height = 300);
    // Desctructeur
    ~MainWindow(); 
    
    // accesseurs sur l'editeur par son ID
    void setEditorID(int i);
    int getEditorID();
    
	// selection d'une matrice (clic ou key event)
	void newSelection(int num);
	
    // affectation d'un fichier audio
    void setAudioFileName(char *name);
    
    // Definition des callbacks d'evenements glut
    void reshape(int w, int h);
    void keyPressed( unsigned char key, int x, int y );
    void specialPressed( int a_keys, int x, int y );
    void mousePressed(int mouse, int state, int x, int y);
    void mouseMoved(int x, int y);
    void draw();
    void close();
};

#endif
