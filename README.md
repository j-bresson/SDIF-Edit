# SDIF-Edit

SDIF-Edit is a tool for the visalization SDIF sound description data.

The [SDIF](http://sdif.sourceforge.net/ "SDIF source repository") format (Sound Description Interchange Format) aims at standardizing and optimizing the codification and storage of sound analyse and synthesis data in order to facilitate their transfer and interchange between applications. The format contains some standard specifications for the most commonly used sound descriptions (spectral and additive descriptions, FFT, filters, transfer functions, resonances, fundamental frequency estimate, voiced/non-voiced decision, temporal markers, etc.), and also allows the users to extend these types (i.e. to create new description fields) or to create new ones. 
SDIF is used as export format by various sound analysis/synthesis and computer music software, such as [AudioSculpt](http://forumnet.ircam.fr/fr/produit/audiosculpt/), [Spear](http://www.klingbeil.com/spear/), or [OpenMusic](http://repmus.ircam.fr/openmusic/).

SDIF-Edit proposes a generic graphical representation of SDIF data, i.e. independant from the type of data contained in the SDIF file. It allows to browse the file structure (streams, matrices), and then to represent and edit graphically the data contained in these structures. 


Dependencies

SDIF-Edit uses the following libraries :
  * [SDIF](http://sourceforge.net/projects/sdif/files/sdif/): linked statically (does not need to be installed on your system in order to run the executables, but required to compile the project)
  * [OpenGL](http://www.opengl.org/)/[GLUT](http://www.opengl.org/resources/libraries/glut.html): linked dynamically (needs to be previously installed on your system)

For an optimal use, a GLUT release including the glutCheckLoop et glutWMCloseFunc features should be preferred (see [Some modifcations to the standard glut](http://www-users.york.ac.uk/~rpf1/glut.html). These modifications are generally included in the recent GLUT releases (at least on MacOSX and Windows). However, the standard GLUT should work as well...
