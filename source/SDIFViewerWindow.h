/*
 *  SdifViewer.h
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

#ifndef SDIF_VIEWER_WINDOW_H
#define SDIF_VIEWER_WINDOW_H

#include "include.h"


#include "GUI.h"
#include "EventManager.h"
#include "Window.h"	
#include "SDIFDescriptor.h"
#include "SplineSurface.h"
#include "SplineEditorWindow.h"
#include "ColorChooserWindow.h"
#include "SliceViewWindow.h"
#include "ViewPointsWindow.h"
#include "SonagramWindow.h"
#include "Vector.h"

#include "MainWindow.h"

//#include "AudioPlayer.h"
//#include "AudioWindow.h"

// types de commandes
#define MODE_VIEW 1
#define MODE_EDIT 2

#define CLICK_NONE  0
#define CLICK_GRID 1
#define CLICK_WIDGET 2
#define EDIT_GRID 3

// type de vues
#define TIME_VIEW 1
#define ELEM_VIEW 2

#define PARAM_SPEED 0
#define PARAM_SCALEV 1
#define PARAM_SCALET 2
#define PARAM_SCALEE 3
#define PARAM_EDITP 4
#define PARAM_EAREA_FROM 5
#define PARAM_EAREA_TO 6
#define PARAM_EAREA_MIN 7
#define PARAM_EAREA_MAX 8
#define PARAM_XPARAM 9
#define PARAM_NONE -1

#define TIME_PARAM_BEGIN 0
#define TIME_PARAM_DUR 1

#define UP 1
#define DOWN -1

#define CLASSIC_DATA 0
#define PARTIAL_DATA 1

#define FLT_FORMAT 0
#define MC_FORMAT 1

#define REF_EDIT 0
#define SPLINE_EDIT 1
#define FLAT_EDIT 2

//class MainWindow;


/**
 * Visualisateur: 
 * Gere la representation des donnees SDIF 
 */
class SDIFViewerWindow : public Window {
    
    // Classes annexes
    friend class SliceViewWindow;
    friend class ViewPointsWindow;
    friend class SonagramWindow;
    //friend class AudioPlayer;
    //friend class AudioWindow;
    friend class ColorChooserWindow;
	friend class MainWindow;

private:
    
    SDIFDescriptor* sdd;	// Informations sur la structure du fichier
    const char* fileName;	// Nom du fichier en cours de lecture
    int tps, element, field;	// Dimensions dans le tableau de stockage des donnees SDIF
    char ** fieldNames;		// Noms des differents champs
    matrixValuesT* data;	// Donnees
    SVStream* selectedStream;	// Flux selectionne
    SVMatrix* selectedMatrix;	// Matrice selectionnee
    int selectedMatrixNum;	// numero de la matrice selectionnee
    int nbMatrices;		// Nombre de matrices
    int selectedT;		// Frame courante
    int selectedElem;		// Element courant
    int selectedField;		// Champ visualise
    float min, max;		// Valueurs min et max des donnees visualisees
    int xParam;			// Champ des abscisses
    float xmin,xmax;		// Valeurs min et max du champ des abscisses
    int colorParam;		// Champ de couleur
    float cmax,cmin;		// Valeurs min et max du champ de couleurs
    Vector<viewpoint> viewpoints; 	// Points de vue préselectionnés
    bool dataModified; 		// Controle de modification pour la sauvegarde
    int beginT, endT;		// limites de temporelles de la portion des données représentée
    int maxElt;			// Nombre max d'elements de matrice affichés
	
	
    int *freqFields;		// gestion des champs dont les valeurs doivent être converties en midicents
    int nbFreqFields;
    
    bool autoSynth;		// Activation de la synthèse a chaque mouvement du point selectionné
    bool regularGridSize;	// controle de la régularité des dimensions (grille rectangulaire) 
    bool timeLines;		// Affichage des lignes dans la direction temporelle
    
    
    int SDIF_DATA;		// type de données SDIF (utilisé pour la gestion du cas particulier des partiels - matrice EASM)
    int timeParam;		// Parametre selectionnée dans la selection temporelle
    int shortCutParam; 		// Le parametre reglable par la commande +/- (F3/F4)
    int mouseX, mouseY;		// controle de la position de la souris
    int click;			// controle des clics souris
    
	bool settings_on;
	bool edition_on;
	
    int mode;			// Mode d'action
    int view;			// Mode de visaualisation
    
    void* font;			// Police d'ecriture
    int space;			// espace entre deux lignes (pixels)	
    GLint vp[4];		// Viewport d'affichage 3D
    float posX, posZ;		// position de la camera
    float *posY;		// hauteur (une valeur pour chacun des champs)
    float aX, aY;		// orientation
    float step;			// Vitesse de deplacement
    
    float scaleT, scaleE; 	// Echelles dans les trois dimensions: Time = X, Value = Y, Element = Z.  
    float *scaleV;		// Echelle des valeurs (Y) propre pour chacun de schamps
    float scaleReference; 	// Dimension a laquelle sera ramenee la grille de donnees
    
    float editStep; 		// Pas d'edition des valeurs
    int editPrecision;		// degré de précision
    int editWidthL, editWidthR, editWidthF, editWidthB;		// dimension de la zone d'édition de part et d'autres d'un point
    bool showEditZone;		// affichage de la zone d'édition
    int editMode;		// mode d'edition (courbe spline, plat, modèle)
    float zpt;		// eloignement du point cible d'édition
    
    color bgColor;		// couleur de fond de la representation 3D
    color minColor;		// couleur des valeurs min
    color maxColor;		// couleurs des valeurs max
    float lowTh, highTh;	// seuils min et max pour le dégradé de couleurs ( 0 <= lowTh < highTh <=1)
    
	int clickedWidget;
							
    // Composants de l'interfcae
    GUIComponent* panelN;
    GUIComponent* panelS;
    GUIComponent* panelW;
    GUIComponent* panelE;
    
    GUIComponent* fileLabel;
    GUIComponent** fieldButtons;
    GUIComponent* beginTimeButton;
    GUIComponent* endTimeLabel;
    GUIComponent* durationButton;
    GUIComponent* nbFrameLabel;
    GUIComponent* matrixButton;
    GUIComponent* timePreview;
    GUIComponent* xParamButton;
    GUIComponent* xParamLabel;
    GUIComponent* eltPreview;
    GUIComponent* movingModeButton;
    GUIComponent* movingModeLabel;
    GUIComponent* pointLabel;
    GUIComponent* tLabel;
    GUIComponent* eltLabel;
    GUIComponent* settingsLabel;
    GUIComponent* settingsButton;
    GUIComponent* speedLabel;
    GUIComponent* scaleELabel;
    GUIComponent* scaleTLabel;
    GUIComponent* scaleVLabel;
    GUIComponent* speedWidget;
    GUIComponent* scaleEWidget;
    GUIComponent* scaleTWidget;
    GUIComponent* scaleVWidget;
    GUIComponent* editStepLabel;
    GUIComponent* editStepPButton;
    GUIComponent* editStepMButton;
    GUIComponent* initButton;
    GUIComponent* linesButton;
    GUIComponent* saveButton;
    GUIComponent* closeButton;
    GUIComponent* editLabel;
    GUIComponent* editButton;
    GUIComponent* editAreaButton;
    GUIComponent* editTimeFromButton;
    GUIComponent* editTimeFromPButton;
    GUIComponent* editTimeFromMButton;
    GUIComponent* editTimeToButton;
    GUIComponent* editTimeToPButton;
    GUIComponent* editTimeToMButton;
    GUIComponent* editElemFromButton;
    GUIComponent* editElemFromPButton;
    GUIComponent* editElemFromMButton;
    GUIComponent* editElemToButton;
    GUIComponent* editElemToPButton;
    GUIComponent* editElemToMButton;
    GUIComponent* editCurveButton;
    GUIComponent* flatButton;
    GUIComponent *colorButton;
    GUIComponent *sliceButton;
    GUIComponent *sonagramButton;
    GUIComponent *userViewsButton;
    GUIComponent *audioButton;
    GUIComponent *editModeButton;
    GUIComponent *editModeLabel;
    
    float **refSurface;
    SplineSurface *surf;	// Calcul de surface spline pour l'edition des donnees
    SplineEditorWindow *se;		// Editeur de surface spline
    ColorChooserWindow *cc;		// Gestion des couleurs
    SliceViewWindow *svw;	// Vues en coupe
    ViewPointsWindow *vpw;	// Gestions des points de vues personnalisés
    SonagramWindow *sona;		// Sonagramme
    //AudioPlayer *ap;		// Fonctions audio
    //AudioWindow *aw;		// Fenetre audio
    
    MainWindow* mainWin;	// Fenetre principale associée
    

    
    // OPERATIONS DE DEPART
    // Initialisation du contexte OpenGL
    void initViewer();
    
    // Selection d'une matrice
    void selectMatrix(int matNum);
    
    // Remplit le tableau de donnees
    // returns 0 si OK, -1 sinon
    int setData();
    
    // Initialisation des attributs
    void initViewerAttributes();

    // Calcul des attributs et valeurs de visualisation
    void reset();
    
    // initialisation des hauteurs pour chaque champ
    void initYInitValues();
    // Calcule min, max et edit step selon le champ selectionné
    void processGridData();
    // Calcule xmax, xmin, echelle et positionnement en fonction du parametre choisi en x-param
    void processXData();
    // Calcule le pas d'edition des donnees en fonction de l'echelle de valeurs
    void calculateEditStep();
    // Renvoie la valeur minimale pour un champ donne
    float getMinValue(int field);
    // Renvoie la valeur maximale pour un champ donne
    float getMaxValue(int field);
    // Renvoie le temps min 
    float minTime();
    // Renvoire le temps max
    float maxTime();
    
    // Initialisation du point de vue
    void initView();

    // Renvoie le type de données SDIF (distiction matrices EASM-PartialSet des  autres)
    int checkData(SVMatrix *m);

    // Ecrit la matrice courante dans le fichier sdif
    // et les eventuels viewpoints enregistrés dans un fichier .vpt
    void save();
    
    // Calcul des dimensions du viewport 3D
    void setViewport();
    // Calcul des positions des composants de l'interface
    void setComponents();
    
    // ajoute ou supprime un champ des champs de frequence
    void updateFrequencyFields(int newfield);
    // Check le format d'affichage de données (midicent,...)
    int fieldPrintFormat(int fieldNum);

    
    // COULEURS
    void setMinColor(color c);
    void setMaxColor(color c);
    void setBgColor(color c);
    color getBgColor();
    color getMinColor();
    color getMaxColor();
    // Change le champ de couleur et met a jour cmin, cmax, et le color chooser
    void setColorParam(int cp);
    // Calcule la couleur d'une valeur
    color calculateColor(float value);
    
    // EDITION
    // visibilite de la zone d'edition
    void setEditZoneVisible();
    // Etendue de la zone d'edition		
    void changeEditWidthL(int v);
    void changeEditWidthR(int v);
    void changeEditWidthF(int v);
    void changeEditWidthB(int v);
    // Edition
    void editUp();
    void editDown();
    void flatEditZone();
    void changeEditMode();
    void mouseEdit(int dy);
	
	// Surface de reference
    void updateRefSurface();
	
	
    
    
    // FENETRES
    void openSplineEditor();
    void openViewPointsWindow();
    void openColorChooser();
    void openSliceWindow();
    void openSonagram();
    void openAudioWindow();    
	
    // Sous-fonctions d'affichage: textes et infos et graphiques 3D
    void drawGUI();		// Interface
    void drawSdifData();	// Grille SDIF
    void drawPts();		// Points pour la selection
    void drawAxes();		// Repère
    void drawPartials();	// Données SDIF sous foprme de partiels
    
    // Selection d'un point avec la souris
    void selectPt(int x, int y);
    // Traite le buffer de selection pour determiner l'objet selectionn√à
    void processHits(GLint hits, GLuint buffer[]);

    // Selection d'une action en fonction de la position sur la fenetre
    void selectAction(int x, int y);
    void selectShortCutParam(int x, int y); 
	void widgetMouseMoved(int dx, int dy);

    // Retour à la fenetre principale de selection des matrices
    void backToMatrixSelection();
    
    // TIME SELECTION
    void setTimeParam(int param);
    void changeBeginTime(int signe, int step);
    void changeDuration(int signe, int step);
    
    // VISUALISATION
    void changeXParam(int v);
    void changeElemRange(int signe, int step);
    void changeViewMode();
    void selectField(int field);
    void changeEditP(int v);
    void changeScaleV(int v);
    void changeScaleT(int v);
    void changeScaleE(int v);
    void setLinesDrawing(bool ln);
    
    // POINT SELECTIONNE
    void selectedPtL();
    void selectedPtR();
    void selectedPtFwd();
    void selectedPtBwd();
    
    // DEPLACEMENTS
    void speedUp();
    void speedDown();
    void cameraLeft();
    void cameraRight();
    void cameraFwd();
    void cameraBwd();
    void cameraUp();
    void cameraDown();
    void moveToSelectedPt();
    void moveAwayFromSelectedPt();
    void turnLAroundSelectedPt();
    void turnRAroundSelectedPt();
    
    // VIEWPOINTS
    // write wiewpoints file using SdifDescriptor
    void saveViewPoints();
    // load viewpoints from SdifDescriptor
    void loadViewPoints();
    
    // Appel a l'AudioPlayer pour la synthese 
    void sound();
    
    
    public :
	
    // Constructeur
    SDIFViewerWindow(SDIFDescriptor* sdifD, int numMatrix, MainWindow* parent, char *audioFileName = NULL, int width = 800, int height = 600);
    // Desctructeur
    ~SDIFViewerWindow(); 
    // attachement d'un fichier audio à l'AudioPlayer
    void setAudioFile(char *fileName);
    
    // Recuperations des callbacks glut
    void reshape(int w, int h);
    void keyPressed( unsigned char key, int x, int y );
    void specialPressed( int a_keys, int x, int y );
    void mousePressed(int mouse, int state, int x, int y);
    void mouseMoved(int x, int y);
    void draw();
    
    // Fermeture
    void close();
    
    static int FreqToMidicents(float frequency);

};

#endif
