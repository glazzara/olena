## Process this file through Automake to create Makefile.in.

## This Makefile makes use of GNU Make extensions (%-patterns,
## patsubst).  We might want to turn it into something more portable
## when Olena is released.

meshdir = $(pkgdatadir)/images
dist_mesh_DATA =

## -------- ##
## Meshes.  ##
## -------- ##

# The `$(srcdir)/' prefixes are mandatory, to force `$(pinv_curv)' and
# `$(max_curv)' to reference files living in the *source* directory,
# instead of the current (build) directory.  See their definitions
# below, using substitution references on `$(meshes)'.
meshes =					\
  $(srcdir)/bunny-holefilled.off		\
  $(srcdir)/cone.off				\
  $(srcdir)/cube.off				\
  $(srcdir)/socket.off				\
  $(srcdir)/teapot.off				\
  $(srcdir)/tetrahedron.off
dist_mesh_DATA += $(meshes)

## ------------------------------------ ##
## Pseudo-inverse curvature of meshes.  ##
## ------------------------------------ ##

mesh_pinv_curv = $(top_builddir)/milena/apps/statues/mesh-pinv-curv
mesh_pinv_curv: $(top_srcdir)/milena/apps/statues/mesh-pinv-curv.cc
	cd $(top_builddir)/milena/apps/statues/mesh-pinv-curv && \
	  $(MAKE) $(AM_MAKEFLAGS) mesh_pinv_curv

# FIXME: *-pinv-curv.off files should not be regenerated
# automatically, because it really takes time.  Find a way to prevent
# Make to regen them without disabling this rule.
## $(srcdir)/%-pinv-curv.off: %.off $(mesh_pinv_curv)
##	$(mesh_pinv_curv) $< $@

pinv_curv = $(meshes:.off=-pinv-curv.off)
dist_mesh_DATA += $(pinv_curv)

## ----------------------------- ##
## Maximum curvature of meshes.  ##
## ----------------------------- ##

mesh_max_curv = $(top_builddir)/milena/apps/statues/mesh-max-curv
mesh_max_curv: $(top_srcdir)/milena/apps/statues/mesh-max-curv.cc
	cd $(top_builddir)/milena/apps/statues/mesh-max-curv && \
	  $(MAKE) $(AM_MAKEFLAGS) mesh_max_curv

# FIXME: *-max-curv.off files should not be regenerated
# automatically, because it really takes time.  Find a way to prevent
# Make to regen them without disabling this rule.
## $(srcdir)/%-max-curv.off: %.off $(mesh_max_curv)
## 	$(mesh_max_curv) $< $@

max_curv = $(meshes:.off=-max-curv.off)
dist_mesh_DATA += $(max_curv)

.PHONY: debug
debug:
	@echo $(pinv_curv)
	@echo $(max_curv)
