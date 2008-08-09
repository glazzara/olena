## Process this file through Automake to produce Makefile.in -*- Makefile -*-

SUBDIRS = python

# Meta-wrappers (templates), not generating a module, but factoring
# common parts.
meta_wrappers =							\
  ch_value.ixx concrete.ixx fill.ixx image2d.ixx intp.ixx	\
  int_u.ixx morpho.ixx pgm.ixx println.ixx

# Wrappers (generating actual modules).
wrappers =							\
 box2d.i dpoint2d.i image2d_int.i image2d_int_u8.i int_u8.i	\
 int_u32.i neighb2d.i point2d.i window2d.i

EXTRA_DIST = $(meta_wrappers) $(wrappers)

check_SCRIPTS = run
