## Linking with gluit and GLUT/OpenGL.		-*- Makefile-Automake -*-

## ------------------------------------------------------------------ ##
## Before loading this file, you must define `gluit_builddir' first.  ##
## ------------------------------------------------------------------ ##

GLFLAGS = -L$(gluit_builddir)
GLLIBS  = -lgluit

# FIXME: Handle other OSes as well (Win32, SunOS, etc.)
if DARWIN
  GLFLAGS += -L$(gluit_builddir) -framework GLUT -framework OpenGL
else !DARWIN
  GLFLAGS += -L$(gluit_builddir)
  GLLIBS  += -lGL -lGLU -lX11 -lXmu -lXi
endif !DARWIN
