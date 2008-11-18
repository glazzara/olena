## Process this file through Automake to create Makefile.in -*- Makefile -*-

## This Makefile makes use of GNU Make extensions (%-patterns,
## patsubst).  We might want to turn it into something more portable
## when Olena is released.

meshdir = $(pkgdatadir)/images
dist_mesh_DATA =

## -------- ##
## Meshes.  ##
## -------- ##

meshes =					\
  bunny-holefilled.off				\
  cone.off					\
  cube.off					\
  socket.off					\
  teapot.off					\
  tetrahedron.off
dist_mesh_DATA += $(meshes)

## ------------------------------------ ##
## Pseudo-inverse curvature of meshes.  ##
## ------------------------------------ ##

mesh_pinv_curv = $(top_builddir)/milena/apps/statues/mesh-pinv-curv
mesh_pinv_curv: $(top_srcdir)/milena/apps/statues/mesh-pinv-curv.cc
	cd $(top_builddir)/milena/apps/statues/mesh-pinv-curv && \
	  $(MAKE) $(AM_MAKEFLAGS) mesh_pinv_curv

$(srcdir)/%-pinv-curv.off: %.off $(mesh_pinv_curv)
	$(mesh_pinv_curv) $< $@

# pinv_curv = $(meshes:.off=-pinv-curv.off)
pinv_curv = $(patsubst %.off,$(srcdir)/%-pinv-curv.off,$(meshes))
dist_mesh_DATA += $(pinv_curv)

## ----------------------------- ##
## Maximum curvature of meshes.  ##
## ----------------------------- ##

mesh_max_curv = $(top_builddir)/milena/apps/statues/mesh-max-curv
mesh_max_curv: $(top_srcdir)/milena/apps/statues/mesh-max-curv.cc
	cd $(top_builddir)/milena/apps/statues/mesh-max-curv && \
	  $(MAKE) $(AM_MAKEFLAGS) mesh_max_curv

$(srcdir)/%-max-curv.off: %.off $(mesh_max_curv)
	$(mesh_max_curv) $< $@

#max_curv = $(meshes:.off=-max-curv.off)
max_curv = $(patsubst %.off,$(srcdir)/%-max-curv.off,$(meshes))
dist_mesh_DATA += $(max_curv)

.PHONY: debug
debug:
	@echo $(pinv_curv)
	@echo $(max_curv)
