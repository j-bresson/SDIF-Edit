MAKE = make
SRCDIR = ../source
CPP = g++
CPPFLAGS = -g -O2 -I../include -D __MACOSX_CORE__
LDFLAGS = -L./lib -lsdif -framework OpenGL -framework GLUT

SRC = ${SRCDIR}/main.cpp

SRC += ${SRCDIR}/SVMatrix.cpp 
SRC += ${SRCDIR}/SVStream.cpp 
SRC += ${SRCDIR}/SDIFDescriptor.cpp 
SRC += ${SRCDIR}/EventManager.cpp 
SRC += ${SRCDIR}/GUI.cpp 
SRC += ${SRCDIR}/GUIComponent.cpp 
SRC += ${SRCDIR}/Window.cpp 
SRC += ${SRCDIR}/MainWindow.cpp 
SRC += ${SRCDIR}/SDIFViewerWindow.cpp 
SRC += ${SRCDIR}/SDIFViewerWindow_Draw.cpp 
SRC += ${SRCDIR}/SonagramWindow.cpp 
SRC += ${SRCDIR}/ColorChooserWindow.cpp 
SRC += ${SRCDIR}/SliceViewWindow.cpp 
SRC += ${SRCDIR}/ViewPointsWindow.cpp 
SRC += ${SRCDIR}/SplineSurface.cpp 
SRC += ${SRCDIR}/SplineEditorWindow.cpp 
SRC += ${SRCDIR}/Preferences.cpp 

OBJECTS=$(SRC:.cpp=.o)
EXECUTABLE= SDIF-Edit

# -Wall

all: SDIF-Edit

SDIF-Edit: $(OBJECTS) 
	@echo "\nLinking $(TARGET) ... "
	$(CPP) $(CPPFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	@echo "\n\n\n==================Compiling file: $<"
	$(CPP) $(CPPFLAGS) -c $< -o $@
	@echo "\n================== compilation DONE : $<"

clean:
	rm -f ${SRCDIR}/*.o core *~ SDIFEdit
