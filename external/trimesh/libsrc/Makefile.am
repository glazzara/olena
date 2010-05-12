AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -lGL -lGLU

# FIXME: We should use Libtool instead.
lib_LTLIBRARIES = libtrimesh.la

libtrimesh_la_SOURCES =				\
  TriMesh_bounding.cc				\
  TriMesh_connectivity.cc			\
  TriMesh_curvature.cc				\
  TriMesh_io.cc					\
  TriMesh_grid.cc				\
  TriMesh_normals.cc				\
  TriMesh_pointareas.cc				\
  TriMesh_stats.cc				\
  TriMesh_tstrips.cc				\
  GLCamera.cc					\
  ICP.cc					\
  KDtree.cc					\
  diffuse.cc					\
  edgeflip.cc					\
  faceflip.cc					\
  filter.cc					\
  lmsmooth.cc					\
  remove.cc					\
  reorder_verts.cc				\
  subdiv.cc

# FIXME: Convert this to Automake.
# ifdef DEBUG
#   AM_CPPFLAGS += -DDEBUG
# endif
# debug:
# 	$(MAKE) DEBUG=y
