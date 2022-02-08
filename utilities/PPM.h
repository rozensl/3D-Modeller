#ifndef PPM_H_
#define PPM_H_

#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>

GLubyte * LoadPPM(
    const char * file,
    int * width,
    int * height
);

#endif
