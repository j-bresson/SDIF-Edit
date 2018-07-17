
# SDIF-Edit

<img src="https://raw.githubusercontent.com/wiki/j-bresson/SDIF-Edit/images/sdif-edit-1.png" width="500pt" align="right">

SDIF-Edit is a tool for the visalization SDIF sound description data.

The [SDIF](http://sdif.sourceforge.net/ "SDIF source repository") format (Sound Description Interchange Format) aims at standardizing and optimizing the codification and storage of sound analyse and synthesis data in order to facilitate their transfer and interchange between applications. The format contains some standard specifications for the most commonly used sound descriptions (spectral and additive descriptions, FFT, filters, transfer functions, resonances, fundamental frequency estimate, voiced/non-voiced decision, temporal markers, etc.), and also allows the users to extend these types (i.e. to create new description fields) or to create new ones. 
SDIF is used as export format by various sound analysis/synthesis and computer music software, such as [AudioSculpt](http://forumnet.ircam.fr/fr/produit/audiosculpt/), [Spear](http://www.klingbeil.com/spear/), or [OpenMusic](http://repmus.ircam.fr/openmusic/).

SDIF-Edit proposes a generic graphical representation of SDIF data, i.e. independant from the type of data contained in the SDIF file. It allows to browse the file structure (streams, matrices), and then to represent and edit graphically the data contained in these structures. 


--------
### License and copyright

This project is released under the GPLv3 license.
(c) Jean Bresson — Ircam 2003-2004.

---------
### Compilation

SDIF-Edit is written in C++. All GUI and graphical interaction is in OpenGL/GLUT (see below).

Just use Makefile in your favorite platform !

---------
### Run

Form a command-line Terminal, just type:

```
./SDIF-Edit /<path-to-file>/<filename>.sdif
```
--------
### Dependencies

SDIF-Edit uses the following libraries :
  * [SDIF](http://sourceforge.net/projects/sdif/files/sdif/): linked statically (does not need to be installed on your system in order to run the executables, but required to compile the project)
  * [OpenGL](http://www.opengl.org/)/[GLUT](http://www.opengl.org/resources/libraries/glut.html): linked dynamically (needs to be previously installed on your system)

For an optimal use, a GLUT release including the glutCheckLoop et glutWMCloseFunc features should be preferred (see [Some modifcations to the standard glut](http://www-users.york.ac.uk/~rpf1/glut.html). These modifications are generally included in the recent GLUT releases (at least on MacOSX and Windows). However, the standard GLUT should work as well...

---------
<img src="https://raw.githubusercontent.com/wiki/j-bresson/SDIF-Edit/images/sdifedit-om.jpg" width="300pt" align="right">

### OpenMusic interface


SDIF-Edit can be used as an editor for the SDIFFILE object in [OpenMusic](http://repmus.ircam.fr/openmusic/). This feature was integrated in OM 5 and 6. For OM 6.4 and higher versions, the use of SDIF-Edit requires preliminary loading of the library [SDIF-Edit4OM](https://forge.ircam.fr/p/omlibraries/downloads/173/). 

---------
### Publication

For additional info, or to cite this work:

  * J. Bresson and C. Agon: SDIF Sound Description Data Representation and Manipulation in Computer Assisted Composition. Proceedings of the International Computer Music Conference (ICMC'04), University of Miami, USA, 2004. [[PDF]](https://hal.archives-ouvertes.fr/hal-01161261)
  
---------

### Documentation

See the project's [wiki pages](https://github.com/j-bresson/SDIF-Edit/wiki).





