MAKEGEN = -I ../../lib/cbir2/ -I ../../lib/cb3dr/ -lglut -lCoin -lz -F DLINUX test_graph

ifeq ($(MAKECMDGOALS),debug)
  MODE = debug
else
  MODE = release
endif

MAIN = test_graph
ifeq ($(shell uname -m),i686)
  BITS = 32
else
  BITS = 64
endif
VERSION = 
LDFLAGS = -ldl -lglut -L /home/midi/coin -lCoin -lz

BINDIR = .
LINK = $(MAIN)
BINARY = $(MAIN).$(MODE)$(BITS)$(VERSION)

ifeq ($(MODE),debug)
  CXX = g++ -g -Wall -pipe
  CC  = gcc -g -Wall -pipe
  LD  = g++ -g -Wall -rdynamic
else
  CXX = g++ -g -Wall -pipe -O3 -DNDEBUG -finline --param inline-unit-growth=200 -fomit-frame-pointer
  CC  = gcc -g -Wall -pipe -O3 -DNDEBUG -finline --param inline-unit-growth=200 -fomit-frame-pointer
  LD  = g++ -g -Wall -O3 -rdynamic
endif

ifeq ($(BITS),32)
  CXX += -m32
  CC  += -m32
  LD  += -m32
else
  CXX += -fPIC -m64
  CC  += -fPIC -m64
  LD  += -fPIC -m64
endif

CXX += -DLINUX
CC += -DLINUX

LIBDIR0 = .
HDRDIR0 = $(LIBDIR0)/headers
SRCDIR0 = $(LIBDIR0)/sources
OBJDIR0 = $(LIBDIR0)/$(MODE)$(BITS)
FILES0  = test_graph
HEADERS0 = $(FILES0:%=$(HDRDIR0)/%.h)
OBJECTS0 = $(FILES0:%=$(OBJDIR0)/%.o)

LIBDIR1 = ../../lib/cbir2
HDRDIR1 = $(LIBDIR1)/headers
SRCDIR1 = $(LIBDIR1)/sources
OBJDIR1 = $(LIBDIR1)/$(MODE)$(BITS)
FILES1  =
HEADERS1 = $(FILES1:%=$(HDRDIR1)/%.h)
OBJECTS1 = $(FILES1:%=$(OBJDIR1)/%.o)

LIBDIR2 = ../../lib/cb3dr
HDRDIR2 = $(LIBDIR2)/headers
SRCDIR2 = $(LIBDIR2)/sources
OBJDIR2 = $(LIBDIR2)/$(MODE)$(BITS)
FILES2  = Face WRL OFF Objet3D mesh_filter mesh_shade Vertex Vector MaillageTriangulaire lmsmooth \
          TriMesh_io TriMesh_tstrips TriMesh_normals remove TriMesh_curvature diffuse common Matrix MailleTriangulaire MyRandom \
          calcul TriMesh_connectivity reorder_verts TriMesh_stats TriMesh_pointareas TriMesh_bounding
HEADERS2 = $(FILES2:%=$(HDRDIR2)/%.h)
OBJECTS2 = $(FILES2:%=$(OBJDIR2)/%.o)

LIBDIRS = $(LIBDIR0) $(LIBDIR1) $(LIBDIR2)
OBJDIRS = $(OBJDIR0) $(OBJDIR1) $(OBJDIR2)
HEADERS = $(HEADERS0) $(HEADERS1) $(HEADERS2)
OBJECTS = $(OBJECTS0) $(OBJECTS1) $(OBJECTS2)

all: $(BINDIR)/$(LINK)

debug: $(BINDIR)/$(LINK)

$(BINDIR)/$(LINK) : dirs $(BINDIR)/$(BINARY)
	@echo " * Lien $(BINDIR)/$(LINK) => $(BINDIR)/$(BINARY)"
	@ln -f -s $(BINARY) $(BINDIR)/$(LINK)

$(BINDIR)/$(BINARY): $(OBJECTS)
	@echo " * Edition des liens"
	@$(LD) -o $(BINDIR)/$(BINARY) $(OBJECTS) $(LDFLAGS)
dirs:
	@mkdir -p $(OBJDIRS)

clean:
	@echo "Supprime les binaires"
	@rm -f $(LINK)
	@rm -f $(MAIN).debug32$(VERSION)
	@rm -f $(MAIN).debug64$(VERSION)
	@rm -f $(MAIN).release32$(VERSION)
	@rm -f $(MAIN).release64$(VERSION)
	@echo "Supprime les fichiers temporaires de la librairie $(LIBDIR0)"
	@rm -Rf $(LIBDIR0)/debug32
	@rm -Rf $(LIBDIR0)/debug64
	@rm -Rf $(LIBDIR0)/release32
	@rm -Rf $(LIBDIR0)/release64
	@rm -Rf $(LIBDIR0)/doc
	@rm -f $(LIBDIR0)/sources/*~
	@rm -f $(LIBDIR0)/headers/*~
	@rm -f $(LIBDIR0)/headers/Inventor/*~
	@rm -f $(LIBDIR0)/headers/Inventor/bundles/*~
	@rm -f $(LIBDIR0)/headers/Inventor/VRMLnodes/*~
	@rm -f $(LIBDIR0)/headers/Inventor/system/*~
	@rm -f $(LIBDIR0)/headers/Inventor/details/*~
	@rm -f $(LIBDIR0)/headers/Inventor/actions/*~
	@rm -f $(LIBDIR0)/headers/Inventor/caches/*~
	@rm -f $(LIBDIR0)/headers/Inventor/collision/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/editors/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/widgets/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/engines/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/nodes/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/viewers/*~
	@rm -f $(LIBDIR0)/headers/Inventor/Qt/devices/*~
	@rm -f $(LIBDIR0)/headers/Inventor/projectors/*~
	@rm -f $(LIBDIR0)/headers/Inventor/threads/*~
	@rm -f $(LIBDIR0)/headers/Inventor/engines/*~
	@rm -f $(LIBDIR0)/headers/Inventor/errors/*~
	@rm -f $(LIBDIR0)/headers/Inventor/lists/*~
	@rm -f $(LIBDIR0)/headers/Inventor/c/*~
	@rm -f $(LIBDIR0)/headers/Inventor/c/glue/*~
	@rm -f $(LIBDIR0)/headers/Inventor/c/base/*~
	@rm -f $(LIBDIR0)/headers/Inventor/c/threads/*~
	@rm -f $(LIBDIR0)/headers/Inventor/c/errors/*~
	@rm -f $(LIBDIR0)/headers/Inventor/events/*~
	@rm -f $(LIBDIR0)/headers/Inventor/nodes/*~
	@rm -f $(LIBDIR0)/headers/Inventor/fields/*~
	@rm -f $(LIBDIR0)/headers/Inventor/draggers/*~
	@rm -f $(LIBDIR0)/headers/Inventor/misc/*~
	@rm -f $(LIBDIR0)/headers/Inventor/annex/*~
	@rm -f $(LIBDIR0)/headers/Inventor/annex/HardCopy/*~
	@rm -f $(LIBDIR0)/headers/Inventor/annex/FXViz/*~
	@rm -f $(LIBDIR0)/headers/Inventor/annex/FXViz/nodes/*~
	@rm -f $(LIBDIR0)/headers/Inventor/annex/FXViz/elements/*~
	@rm -f $(LIBDIR0)/headers/Inventor/nodekits/*~
	@rm -f $(LIBDIR0)/headers/Inventor/elements/*~
	@rm -f $(LIBDIR0)/headers/Inventor/sensors/*~
	@rm -f $(LIBDIR0)/headers/Inventor/lock/*~
	@rm -f $(LIBDIR0)/headers/Inventor/manips/*~
	@rm -f $(LIBDIR0)/headers/mln/*~
	@rm -f $(LIBDIR0)/headers/mln/win/*~
	@rm -f $(LIBDIR0)/headers/mln/math/*~
	@rm -f $(LIBDIR0)/headers/mln/subsampling/*~
	@rm -f $(LIBDIR0)/headers/mln/trait/*~
	@rm -f $(LIBDIR0)/headers/mln/trait/image/*~
	@rm -f $(LIBDIR0)/headers/mln/trait/op/*~
	@rm -f $(LIBDIR0)/headers/mln/trait/value/*~
	@rm -f $(LIBDIR0)/headers/mln/set/*~
	@rm -f $(LIBDIR0)/headers/mln/morpho/*~
	@rm -f $(LIBDIR0)/headers/mln/io/*~
	@rm -f $(LIBDIR0)/headers/mln/io/pnm/*~
	@rm -f $(LIBDIR0)/headers/mln/io/fits/*~
	@rm -f $(LIBDIR0)/headers/mln/io/ppm/*~
	@rm -f $(LIBDIR0)/headers/mln/io/pbm/*~
	@rm -f $(LIBDIR0)/headers/mln/io/pfm/*~
	@rm -f $(LIBDIR0)/headers/mln/io/internal/*~
	@rm -f $(LIBDIR0)/headers/mln/io/pgm/*~
	@rm -f $(LIBDIR0)/headers/mln/algebra/*~
	@rm -f $(LIBDIR0)/headers/mln/linear/*~
	@rm -f $(LIBDIR0)/headers/mln/linear/local/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/v2v/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/v2b/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/vv2v/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/internal/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/p2v/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/i2v/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/x2x/*~
	@rm -f $(LIBDIR0)/headers/mln/fun/p2b/*~
	@rm -f $(LIBDIR0)/headers/mln/estim/*~
	@rm -f $(LIBDIR0)/headers/mln/debug/*~
	@rm -f $(LIBDIR0)/headers/mln/tag/*~
	@rm -f $(LIBDIR0)/headers/mln/border/*~
	@rm -f $(LIBDIR0)/headers/mln/test/*~
	@rm -f $(LIBDIR0)/headers/mln/pw/*~
	@rm -f $(LIBDIR0)/headers/mln/trace/*~
	@rm -f $(LIBDIR0)/headers/mln/accu/*~
	@rm -f $(LIBDIR0)/headers/mln/accu/internal/*~
	@rm -f $(LIBDIR0)/headers/mln/literal/*~
	@rm -f $(LIBDIR0)/headers/mln/histo/*~
	@rm -f $(LIBDIR0)/headers/mln/convert/*~
	@rm -f $(LIBDIR0)/headers/mln/neighb/*~
	@rm -f $(LIBDIR0)/headers/mln/metal/*~
	@rm -f $(LIBDIR0)/headers/mln/metal/math/*~
	@rm -f $(LIBDIR0)/headers/mln/metal/make/*~
	@rm -f $(LIBDIR0)/headers/mln/geom/*~
	@rm -f $(LIBDIR0)/headers/mln/level/*~
	@rm -f $(LIBDIR0)/headers/mln/level/approx/*~
	@rm -f $(LIBDIR0)/headers/mln/level/naive/*~
	@rm -f $(LIBDIR0)/headers/mln/core/*~
	@rm -f $(LIBDIR0)/headers/mln/core/trait/*~
	@rm -f $(LIBDIR0)/headers/mln/core/concept/*~
	@rm -f $(LIBDIR0)/headers/mln/core/concept/doc/*~
	@rm -f $(LIBDIR0)/headers/mln/core/internal/*~
	@rm -f $(LIBDIR0)/headers/mln/core/internal/impl/*~
	@rm -f $(LIBDIR0)/headers/mln/core/internal/check/*~
	@rm -f $(LIBDIR0)/headers/mln/logical/*~
	@rm -f $(LIBDIR0)/headers/mln/util/*~
	@rm -f $(LIBDIR0)/headers/mln/util/internal/*~
	@rm -f $(LIBDIR0)/headers/mln/binarization/*~
	@rm -f $(LIBDIR0)/headers/mln/display/*~
	@rm -f $(LIBDIR0)/headers/mln/value/*~
	@rm -f $(LIBDIR0)/headers/mln/value/concept/*~
	@rm -f $(LIBDIR0)/headers/mln/value/internal/*~
	@rm -f $(LIBDIR0)/headers/mln/value/builtin/*~
	@rm -f $(LIBDIR0)/headers/mln/arith/*~
	@rm -f $(LIBDIR0)/headers/mln/draw/*~
	@rm -f $(LIBDIR0)/headers/mln/make/*~
	@rm -f $(LIBDIR0)/headers/mln/canvas/*~
	@rm -f $(LIBDIR0)/headers/mln/canvas/morpho/*~
	@rm -f $(LIBDIR0)/headers/mln/canvas/browsing/*~
	@rm -f $(LIBDIR0)/headers/mln/labeling/*~
	@rm -f $(LIBDIR0)/headers/mln/norm/*~
	@echo "Supprime les fichiers temporaires de la librairie $(LIBDIR1)"
	@rm -Rf $(LIBDIR1)/debug32
	@rm -Rf $(LIBDIR1)/debug64
	@rm -Rf $(LIBDIR1)/release32
	@rm -Rf $(LIBDIR1)/release64
	@rm -Rf $(LIBDIR1)/doc
	@rm -f $(LIBDIR1)/sources/*~
	@rm -f $(LIBDIR1)/sources/filter/*~
	@rm -f $(LIBDIR1)/sources/codecs/*~
	@rm -f $(LIBDIR1)/headers/*~
	@rm -f $(LIBDIR1)/headers/filter/*~
	@rm -f $(LIBDIR1)/headers/codecs/*~
	@echo "Supprime les fichiers temporaires de la librairie $(LIBDIR2)"
	@rm -Rf $(LIBDIR2)/debug32
	@rm -Rf $(LIBDIR2)/debug64
	@rm -Rf $(LIBDIR2)/release32
	@rm -Rf $(LIBDIR2)/release64
	@rm -Rf $(LIBDIR2)/doc
	@rm -f $(LIBDIR2)/sources/*~
	@rm -f $(LIBDIR2)/sources/Pink/*~
	@rm -f $(LIBDIR2)/sources/WRL/*~
	@rm -f $(LIBDIR2)/sources/binvox/*~
	@rm -f $(LIBDIR2)/sources/binvox/math/*~
	@rm -f $(LIBDIR2)/sources/binvox/ui/*~
	@rm -f $(LIBDIR2)/sources/binvox/voxel/*~
	@rm -f $(LIBDIR2)/sources/binvox/geom/*~
	@rm -f $(LIBDIR2)/sources/binvox/time/*~
	@rm -f $(LIBDIR2)/sources/binvox/file/*~
	@rm -f $(LIBDIR2)/sources/binvox/ply/*~
	@rm -f $(LIBDIR2)/sources/histogramme/*~
	@rm -f $(LIBDIR2)/sources/Trimesh/*~
	@rm -f $(LIBDIR2)/sources/matrice/*~
	@rm -f $(LIBDIR2)/headers/*~
	@rm -f $(LIBDIR2)/headers/Pink/*~
	@rm -f $(LIBDIR2)/headers/WRL/*~
	@rm -f $(LIBDIR2)/headers/binvox/*~
	@rm -f $(LIBDIR2)/headers/binvox/math/*~
	@rm -f $(LIBDIR2)/headers/binvox/ui/*~
	@rm -f $(LIBDIR2)/headers/binvox/voxel/*~
	@rm -f $(LIBDIR2)/headers/binvox/geom/*~
	@rm -f $(LIBDIR2)/headers/binvox/time/*~
	@rm -f $(LIBDIR2)/headers/binvox/file/*~
	@rm -f $(LIBDIR2)/headers/binvox/ply/*~
	@rm -f $(LIBDIR2)/headers/histogramme/*~
	@rm -f $(LIBDIR2)/headers/Trimesh/*~
	@rm -f $(LIBDIR2)/headers/matrice/*~

$(OBJDIR0)/test_graph.o:  \
 $(SRCDIR0)/test_graph.cpp  \
 $(HDRDIR2)/binvox/geom/Face.h  \
 $(HDRDIR2)/binvox/geom/Vertex.h  \
 $(HDRDIR2)/binvox/math/Vector.h  \
 $(HDRDIR2)/binvox/math/common.h  \
 $(HDRDIR2)/binvox/math/Matrix.h  \
 $(HDRDIR2)/binvox/geom/geom_defs.h  \
 $(HDRDIR2)/WRL/WRL.h  \
 $(HDRDIR2)/WRL/Coin.h  \
 $(HDRDIR2)/WRL/OFF.h  \
 $(HDRDIR2)/WRL/Objet3D.h  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/mesh_filter.h  \
 $(HDRDIR2)/Trimesh/mesh_shade.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR0)/test_graph.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR0)/test_graph.o $(SRCDIR0)/test_graph.cpp

$(OBJDIR2)/Face.o:  \
 $(SRCDIR2)/binvox/geom/Face.cpp  \
 $(HDRDIR2)/binvox/geom/Face.h  \
 $(HDRDIR2)/binvox/geom/Vertex.h  \
 $(HDRDIR2)/binvox/math/Vector.h  \
 $(HDRDIR2)/binvox/math/common.h  \
 $(HDRDIR2)/binvox/math/Matrix.h  \
 $(HDRDIR2)/binvox/geom/geom_defs.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/binvox/geom/Face.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/Face.o $(SRCDIR2)/binvox/geom/Face.cpp

$(OBJDIR2)/WRL.o:  \
 $(SRCDIR2)/WRL/WRL.cpp  \
 $(HDRDIR2)/WRL/WRL.h  \
 $(HDRDIR2)/WRL/Coin.h  \
 $(HDRDIR2)/WRL/MaillageTriangulaire.h  \
 $(HDRDIR2)/WRL/MailleTriangulaire.h  \
 $(HDRDIR2)/WRL/calcul.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/WRL/WRL.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/WRL.o $(SRCDIR2)/WRL/WRL.cpp

$(OBJDIR2)/OFF.o:  \
 $(SRCDIR2)/WRL/OFF.cpp  \
 $(HDRDIR2)/WRL/OFF.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/WRL/OFF.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/OFF.o $(SRCDIR2)/WRL/OFF.cpp

$(OBJDIR2)/Objet3D.o:  \
 $(SRCDIR2)/WRL/Objet3D.cpp  \
 $(HDRDIR2)/WRL/Objet3D.h  \
 $(HDRDIR2)/WRL/Coin.h  \
 $(HDRDIR2)/WRL/WRL.h  \
 $(HDRDIR2)/WRL/OFF.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/WRL/Objet3D.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/Objet3D.o $(SRCDIR2)/WRL/Objet3D.cpp

$(OBJDIR2)/mesh_filter.o:  \
 $(SRCDIR2)/Trimesh/mesh_filter.cpp  \
 $(HDRDIR2)/Trimesh/lmsmooth.h  \
 $(HDRDIR2)/Trimesh/TriMesh_io.h  \
 $(HDRDIR2)/Trimesh/TriMesh_tstrips.h  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/TriMesh_normals.h  \
 $(HDRDIR2)/Trimesh/remove.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/mesh_filter.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/mesh_filter.o $(SRCDIR2)/Trimesh/mesh_filter.cpp

$(OBJDIR2)/mesh_shade.o:  \
 $(SRCDIR2)/Trimesh/mesh_shade.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/TriMesh_io.h  \
 $(HDRDIR2)/Trimesh/TriMesh_curvature.h  \
 $(HDRDIR2)/Trimesh/diffuse.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/mesh_shade.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/mesh_shade.o $(SRCDIR2)/Trimesh/mesh_shade.cpp

$(OBJDIR2)/Vertex.o:  \
 $(SRCDIR2)/binvox/geom/Vertex.cpp  \
 $(HDRDIR2)/binvox/geom/Vertex.h  \
 $(HDRDIR2)/binvox/math/Vector.h  \
 $(HDRDIR2)/binvox/math/common.h  \
 $(HDRDIR2)/binvox/math/Matrix.h  \
 $(HDRDIR2)/binvox/geom/geom_defs.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/binvox/geom/Vertex.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/Vertex.o $(SRCDIR2)/binvox/geom/Vertex.cpp

$(OBJDIR2)/Vector.o:  \
 $(SRCDIR2)/binvox/math/Vector.cpp  \
 $(HDRDIR2)/binvox/math/Vector.h  \
 $(HDRDIR2)/binvox/math/common.h  \
 $(HDRDIR2)/binvox/math/Matrix.h  \
 $(HDRDIR2)/binvox/math/MyRandom.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/binvox/math/Vector.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/Vector.o $(SRCDIR2)/binvox/math/Vector.cpp

$(OBJDIR2)/MaillageTriangulaire.o:  \
 $(SRCDIR2)/WRL/MaillageTriangulaire.cpp  \
 $(HDRDIR2)/WRL/MaillageTriangulaire.h  \
 $(HDRDIR2)/WRL/Coin.h  \
 $(HDRDIR2)/WRL/MailleTriangulaire.h  \
 $(HDRDIR2)/WRL/calcul.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/WRL/MaillageTriangulaire.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/MaillageTriangulaire.o $(SRCDIR2)/WRL/MaillageTriangulaire.cpp

$(OBJDIR2)/lmsmooth.o:  \
 $(SRCDIR2)/Trimesh/lmsmooth.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/TriMesh_connectivity.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/lmsmooth.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/lmsmooth.o $(SRCDIR2)/Trimesh/lmsmooth.cpp

$(OBJDIR2)/TriMesh_io.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_io.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_io.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_io.o $(SRCDIR2)/Trimesh/TriMesh_io.cpp

$(OBJDIR2)/TriMesh_tstrips.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_tstrips.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_tstrips.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_tstrips.o $(SRCDIR2)/Trimesh/TriMesh_tstrips.cpp

$(OBJDIR2)/TriMesh_normals.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_normals.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_normals.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_normals.o $(SRCDIR2)/Trimesh/TriMesh_normals.cpp

$(OBJDIR2)/remove.o:  \
 $(SRCDIR2)/Trimesh/remove.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/reorder_verts.h  \
 $(HDRDIR2)/Trimesh/TriMesh_stats.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/remove.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/remove.o $(SRCDIR2)/Trimesh/remove.cpp

$(OBJDIR2)/TriMesh_curvature.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_curvature.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_curvature.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_curvature.o $(SRCDIR2)/Trimesh/TriMesh_curvature.cpp

$(OBJDIR2)/diffuse.o:  \
 $(SRCDIR2)/Trimesh/diffuse.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/timestamp.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/diffuse.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/diffuse.o $(SRCDIR2)/Trimesh/diffuse.cpp

$(OBJDIR2)/common.o:  \
 $(SRCDIR2)/binvox/math/common.cpp  \
 $(HDRDIR2)/binvox/math/common.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/binvox/math/common.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/common.o $(SRCDIR2)/binvox/math/common.cpp

$(OBJDIR2)/Matrix.o:  \
 $(SRCDIR2)/binvox/math/Matrix.cpp  \
 $(HDRDIR2)/binvox/math/Matrix.h  \
 $(HDRDIR2)/binvox/math/common.h  \
 $(HDRDIR2)/binvox/math/Vector.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/binvox/math/Matrix.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/Matrix.o $(SRCDIR2)/binvox/math/Matrix.cpp

$(OBJDIR2)/MailleTriangulaire.o:  \
 $(SRCDIR2)/WRL/MailleTriangulaire.cpp  \
 $(HDRDIR2)/WRL/MailleTriangulaire.h  \
 $(HDRDIR2)/WRL/Coin.h  \
 $(HDRDIR2)/WRL/calcul.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/WRL/MailleTriangulaire.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/MailleTriangulaire.o $(SRCDIR2)/WRL/MailleTriangulaire.cpp

$(OBJDIR2)/MyRandom.o:  \
 $(SRCDIR2)/binvox/math/MyRandom.cpp  \
 $(HDRDIR2)/binvox/math/MyRandom.h  \
 $(HDRDIR2)/binvox/math/common.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/binvox/math/MyRandom.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/MyRandom.o $(SRCDIR2)/binvox/math/MyRandom.cpp

$(OBJDIR2)/calcul.o:  \
 $(SRCDIR2)/WRL/calcul.cpp  \
 $(HDRDIR2)/WRL/calcul.h  \
 $(HDRDIR2)/WRL/Coin.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/WRL/calcul.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/calcul.o $(SRCDIR2)/WRL/calcul.cpp

$(OBJDIR2)/TriMesh_connectivity.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_connectivity.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_connectivity.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_connectivity.o $(SRCDIR2)/Trimesh/TriMesh_connectivity.cpp

$(OBJDIR2)/reorder_verts.o:  \
 $(SRCDIR2)/Trimesh/reorder_verts.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR2)/Trimesh/XForm.h  \
 $(HDRDIR2)/Trimesh/lineqn.h  \
 $(HDRDIR2)/Trimesh/TriMesh_pointareas.h  \
 $(HDRDIR2)/Trimesh/TriMesh_bounding.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/reorder_verts.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/reorder_verts.o $(SRCDIR2)/Trimesh/reorder_verts.cpp

$(OBJDIR2)/TriMesh_stats.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_stats.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_stats.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_stats.o $(SRCDIR2)/Trimesh/TriMesh_stats.cpp

$(OBJDIR2)/TriMesh_pointareas.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_pointareas.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_pointareas.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_pointareas.o $(SRCDIR2)/Trimesh/TriMesh_pointareas.cpp

$(OBJDIR2)/TriMesh_bounding.o:  \
 $(SRCDIR2)/Trimesh/TriMesh_bounding.cpp  \
 $(HDRDIR2)/Trimesh/TriMesh.h  \
 $(HDRDIR2)/Trimesh/Vec.h  \
 $(HDRDIR2)/Trimesh/Color.h  \
 $(HDRDIR2)/Trimesh/bsphere.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR2)/Trimesh/TriMesh_bounding.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -I$(HDRDIR2) -o $(OBJDIR2)/TriMesh_bounding.o $(SRCDIR2)/Trimesh/TriMesh_bounding.cpp

