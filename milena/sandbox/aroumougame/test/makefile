MAKEGEN = -I ../retin/ -lglut -lCoin -lz -F DLINUX test_graph

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
LDFLAGS = -ldl -lglut -L $(LSA)/lib -lCoin -lz

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

CXX += -DLINUX -I ../../../
CC += -DLINUX

LIBDIR0 = .
HDRDIR0 = $(LIBDIR0)/headers
SRCDIR0 = $(LIBDIR0)/sources
OBJDIR0 = $(LIBDIR0)/$(MODE)$(BITS)
FILES0  = test_graph
HEADERS0 = $(FILES0:%=$(HDRDIR0)/%.h)
OBJECTS0 = $(FILES0:%=$(OBJDIR0)/%.o)

LIBDIR1 = ../retin
HDRDIR1 = $(LIBDIR1)/headers
SRCDIR1 = $(LIBDIR1)/sources
OBJDIR1 = $(LIBDIR1)/$(MODE)$(BITS)
FILES1  = Face WRL OFF Objet3D mesh_filter mesh_shade Vertex Vector MaillageTriangulaire lmsmooth \
          TriMesh_io TriMesh_tstrips TriMesh_normals remove TriMesh_curvature diffuse common Matrix MailleTriangulaire MyRandom \
          calcul TriMesh_connectivity reorder_verts TriMesh_stats TriMesh_pointareas TriMesh_bounding
HEADERS1 = $(FILES1:%=$(HDRDIR1)/%.h)
OBJECTS1 = $(FILES1:%=$(OBJDIR1)/%.o)

LIBDIRS = $(LIBDIR0) $(LIBDIR1)
OBJDIRS = $(OBJDIR0) $(OBJDIR1)
HEADERS = $(HEADERS0) $(HEADERS1)
OBJECTS = $(OBJECTS0) $(OBJECTS1)

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
	@echo "Supprime les fichiers temporaires de la librairie $(LIBDIR1)"
	@rm -Rf $(LIBDIR1)/debug32
	@rm -Rf $(LIBDIR1)/debug64
	@rm -Rf $(LIBDIR1)/release32
	@rm -Rf $(LIBDIR1)/release64
	@rm -Rf $(LIBDIR1)/doc
	@rm -f $(LIBDIR1)/sources/*~
	@rm -f $(LIBDIR1)/sources/WRL/*~
	@rm -f $(LIBDIR1)/sources/binvox/*~
	@rm -f $(LIBDIR1)/sources/binvox/math/*~
	@rm -f $(LIBDIR1)/sources/binvox/ui/*~
	@rm -f $(LIBDIR1)/sources/binvox/voxel/*~
	@rm -f $(LIBDIR1)/sources/binvox/geom/*~
	@rm -f $(LIBDIR1)/sources/binvox/time/*~
	@rm -f $(LIBDIR1)/sources/binvox/file/*~
	@rm -f $(LIBDIR1)/sources/binvox/ply/*~
	@rm -f $(LIBDIR1)/sources/histogramme/*~
	@rm -f $(LIBDIR1)/sources/Trimesh/*~
	@rm -f $(LIBDIR1)/sources/matrice/*~
	@rm -f $(LIBDIR1)/headers/*~
	@rm -f $(LIBDIR1)/headers/WRL/*~
	@rm -f $(LIBDIR1)/headers/binvox/*~
	@rm -f $(LIBDIR1)/headers/binvox/math/*~
	@rm -f $(LIBDIR1)/headers/binvox/ui/*~
	@rm -f $(LIBDIR1)/headers/binvox/voxel/*~
	@rm -f $(LIBDIR1)/headers/binvox/geom/*~
	@rm -f $(LIBDIR1)/headers/binvox/time/*~
	@rm -f $(LIBDIR1)/headers/binvox/file/*~
	@rm -f $(LIBDIR1)/headers/binvox/ply/*~
	@rm -f $(LIBDIR1)/headers/histogramme/*~
	@rm -f $(LIBDIR1)/headers/Trimesh/*~
	@rm -f $(LIBDIR1)/headers/matrice/*~

$(OBJDIR0)/test_graph.o:  \
 $(SRCDIR0)/test_graph.cpp  \
 $(HDRDIR1)/binvox/geom/Face.h  \
 $(HDRDIR1)/binvox/geom/Vertex.h  \
 $(HDRDIR1)/binvox/math/Vector.h  \
 $(HDRDIR1)/binvox/math/common.h  \
 $(HDRDIR1)/binvox/math/Matrix.h  \
 $(HDRDIR1)/binvox/geom/geom_defs.h  \
 $(HDRDIR1)/WRL/WRL.h  \
 $(HDRDIR1)/WRL/Coin.h  \
 $(HDRDIR1)/WRL/OFF.h  \
 $(HDRDIR1)/WRL/Objet3D.h  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/mesh_filter.h  \
 $(HDRDIR1)/Trimesh/mesh_shade.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR0)/test_graph.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR0)/test_graph.o $(SRCDIR0)/test_graph.cpp

$(OBJDIR1)/Face.o:  \
 $(SRCDIR1)/binvox/geom/Face.cpp  \
 $(HDRDIR1)/binvox/geom/Face.h  \
 $(HDRDIR1)/binvox/geom/Vertex.h  \
 $(HDRDIR1)/binvox/math/Vector.h  \
 $(HDRDIR1)/binvox/math/common.h  \
 $(HDRDIR1)/binvox/math/Matrix.h  \
 $(HDRDIR1)/binvox/geom/geom_defs.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/binvox/geom/Face.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/Face.o $(SRCDIR1)/binvox/geom/Face.cpp

$(OBJDIR1)/WRL.o:  \
 $(SRCDIR1)/WRL/WRL.cpp  \
 $(HDRDIR1)/WRL/WRL.h  \
 $(HDRDIR1)/WRL/Coin.h  \
 $(HDRDIR1)/WRL/MaillageTriangulaire.h  \
 $(HDRDIR1)/WRL/MailleTriangulaire.h  \
 $(HDRDIR1)/WRL/calcul.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/WRL/WRL.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/WRL.o $(SRCDIR1)/WRL/WRL.cpp

$(OBJDIR1)/OFF.o:  \
 $(SRCDIR1)/WRL/OFF.cpp  \
 $(HDRDIR1)/WRL/OFF.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/WRL/OFF.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/OFF.o $(SRCDIR1)/WRL/OFF.cpp

$(OBJDIR1)/Objet3D.o:  \
 $(SRCDIR1)/WRL/Objet3D.cpp  \
 $(HDRDIR1)/WRL/Objet3D.h  \
 $(HDRDIR1)/WRL/Coin.h  \
 $(HDRDIR1)/WRL/WRL.h  \
 $(HDRDIR1)/WRL/OFF.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/WRL/Objet3D.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/Objet3D.o $(SRCDIR1)/WRL/Objet3D.cpp

$(OBJDIR1)/mesh_filter.o:  \
 $(SRCDIR1)/Trimesh/mesh_filter.cpp  \
 $(HDRDIR1)/Trimesh/lmsmooth.h  \
 $(HDRDIR1)/Trimesh/TriMesh_io.h  \
 $(HDRDIR1)/Trimesh/TriMesh_tstrips.h  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/TriMesh_normals.h  \
 $(HDRDIR1)/Trimesh/remove.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/mesh_filter.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/mesh_filter.o $(SRCDIR1)/Trimesh/mesh_filter.cpp

$(OBJDIR1)/mesh_shade.o:  \
 $(SRCDIR1)/Trimesh/mesh_shade.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/TriMesh_io.h  \
 $(HDRDIR1)/Trimesh/TriMesh_curvature.h  \
 $(HDRDIR1)/Trimesh/diffuse.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/mesh_shade.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/mesh_shade.o $(SRCDIR1)/Trimesh/mesh_shade.cpp

$(OBJDIR1)/Vertex.o:  \
 $(SRCDIR1)/binvox/geom/Vertex.cpp  \
 $(HDRDIR1)/binvox/geom/Vertex.h  \
 $(HDRDIR1)/binvox/math/Vector.h  \
 $(HDRDIR1)/binvox/math/common.h  \
 $(HDRDIR1)/binvox/math/Matrix.h  \
 $(HDRDIR1)/binvox/geom/geom_defs.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/binvox/geom/Vertex.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/Vertex.o $(SRCDIR1)/binvox/geom/Vertex.cpp

$(OBJDIR1)/Vector.o:  \
 $(SRCDIR1)/binvox/math/Vector.cpp  \
 $(HDRDIR1)/binvox/math/Vector.h  \
 $(HDRDIR1)/binvox/math/common.h  \
 $(HDRDIR1)/binvox/math/Matrix.h  \
 $(HDRDIR1)/binvox/math/MyRandom.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/binvox/math/Vector.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/Vector.o $(SRCDIR1)/binvox/math/Vector.cpp

$(OBJDIR1)/MaillageTriangulaire.o:  \
 $(SRCDIR1)/WRL/MaillageTriangulaire.cpp  \
 $(HDRDIR1)/WRL/MaillageTriangulaire.h  \
 $(HDRDIR1)/WRL/Coin.h  \
 $(HDRDIR1)/WRL/MailleTriangulaire.h  \
 $(HDRDIR1)/WRL/calcul.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/WRL/MaillageTriangulaire.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/MaillageTriangulaire.o $(SRCDIR1)/WRL/MaillageTriangulaire.cpp

$(OBJDIR1)/lmsmooth.o:  \
 $(SRCDIR1)/Trimesh/lmsmooth.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/TriMesh_connectivity.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/lmsmooth.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/lmsmooth.o $(SRCDIR1)/Trimesh/lmsmooth.cpp

$(OBJDIR1)/TriMesh_io.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_io.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_io.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_io.o $(SRCDIR1)/Trimesh/TriMesh_io.cpp

$(OBJDIR1)/TriMesh_tstrips.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_tstrips.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_tstrips.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_tstrips.o $(SRCDIR1)/Trimesh/TriMesh_tstrips.cpp

$(OBJDIR1)/TriMesh_normals.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_normals.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_normals.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_normals.o $(SRCDIR1)/Trimesh/TriMesh_normals.cpp

$(OBJDIR1)/remove.o:  \
 $(SRCDIR1)/Trimesh/remove.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/reorder_verts.h  \
 $(HDRDIR1)/Trimesh/TriMesh_stats.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/remove.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/remove.o $(SRCDIR1)/Trimesh/remove.cpp

$(OBJDIR1)/TriMesh_curvature.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_curvature.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_curvature.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_curvature.o $(SRCDIR1)/Trimesh/TriMesh_curvature.cpp

$(OBJDIR1)/diffuse.o:  \
 $(SRCDIR1)/Trimesh/diffuse.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/timestamp.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/diffuse.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/diffuse.o $(SRCDIR1)/Trimesh/diffuse.cpp

$(OBJDIR1)/common.o:  \
 $(SRCDIR1)/binvox/math/common.cpp  \
 $(HDRDIR1)/binvox/math/common.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/binvox/math/common.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/common.o $(SRCDIR1)/binvox/math/common.cpp

$(OBJDIR1)/Matrix.o:  \
 $(SRCDIR1)/binvox/math/Matrix.cpp  \
 $(HDRDIR1)/binvox/math/Matrix.h  \
 $(HDRDIR1)/binvox/math/common.h  \
 $(HDRDIR1)/binvox/math/Vector.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/binvox/math/Matrix.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/Matrix.o $(SRCDIR1)/binvox/math/Matrix.cpp

$(OBJDIR1)/MailleTriangulaire.o:  \
 $(SRCDIR1)/WRL/MailleTriangulaire.cpp  \
 $(HDRDIR1)/WRL/MailleTriangulaire.h  \
 $(HDRDIR1)/WRL/Coin.h  \
 $(HDRDIR1)/WRL/calcul.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/WRL/MailleTriangulaire.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/MailleTriangulaire.o $(SRCDIR1)/WRL/MailleTriangulaire.cpp

$(OBJDIR1)/MyRandom.o:  \
 $(SRCDIR1)/binvox/math/MyRandom.cpp  \
 $(HDRDIR1)/binvox/math/MyRandom.h  \
 $(HDRDIR1)/binvox/math/common.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/binvox/math/MyRandom.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/MyRandom.o $(SRCDIR1)/binvox/math/MyRandom.cpp

$(OBJDIR1)/calcul.o:  \
 $(SRCDIR1)/WRL/calcul.cpp  \
 $(HDRDIR1)/WRL/calcul.h  \
 $(HDRDIR1)/WRL/Coin.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/WRL/calcul.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/calcul.o $(SRCDIR1)/WRL/calcul.cpp

$(OBJDIR1)/TriMesh_connectivity.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_connectivity.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_connectivity.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_connectivity.o $(SRCDIR1)/Trimesh/TriMesh_connectivity.cpp

$(OBJDIR1)/reorder_verts.o:  \
 $(SRCDIR1)/Trimesh/reorder_verts.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/TriMesh_algo.h  \
 $(HDRDIR1)/Trimesh/XForm.h  \
 $(HDRDIR1)/Trimesh/lineqn.h  \
 $(HDRDIR1)/Trimesh/TriMesh_pointareas.h  \
 $(HDRDIR1)/Trimesh/TriMesh_bounding.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/reorder_verts.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/reorder_verts.o $(SRCDIR1)/Trimesh/reorder_verts.cpp

$(OBJDIR1)/TriMesh_stats.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_stats.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_stats.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_stats.o $(SRCDIR1)/Trimesh/TriMesh_stats.cpp

$(OBJDIR1)/TriMesh_pointareas.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_pointareas.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_pointareas.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_pointareas.o $(SRCDIR1)/Trimesh/TriMesh_pointareas.cpp

$(OBJDIR1)/TriMesh_bounding.o:  \
 $(SRCDIR1)/Trimesh/TriMesh_bounding.cpp  \
 $(HDRDIR1)/Trimesh/TriMesh.h  \
 $(HDRDIR1)/Trimesh/Vec.h  \
 $(HDRDIR1)/Trimesh/Color.h  \
 $(HDRDIR1)/Trimesh/bsphere.h
	@echo " * Compilation $(BITS) bits : $(SRCDIR1)/Trimesh/TriMesh_bounding.cpp"
	@$(CXX) -c -I$(HDRDIR0) -I$(HDRDIR1) -o $(OBJDIR1)/TriMesh_bounding.o $(SRCDIR1)/Trimesh/TriMesh_bounding.cpp

