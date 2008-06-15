# To be redefined if TriMesh is located elsewhere.
# FIXME: Trimesh could be part of the Olena distribution, as a
# dependency.  Or we could just rewrite the required features (I/O &
# curvature computation).
TRIMESH = $(HOME)/opt/trimesh2

# Warning: we should use the same compiler as the one that was used to
# compile TriMesh to avoid any ABI-related problems.

# Produce fast code.
CXXFLAGS = -O3 -DNDEBUG -ggdb
# Produce debuggable code.
#CXXFLAGS = -O0 -ggdb -fno-inline -fno-default-inline

# Milena and Trimesh paths.
CPPFLAGS = -I../../../ -I$(TRIMESH)/include
# Link with Trimesh.
LDFLAGS = -L$(TRIMESH)/lib.Darwin -ltrimesh

all: mesh-segm

mesh-segm: mesh-segm.o
	$(CXX) -o $@ $(LDFLAGS) $<

check: mesh-segm
	./mesh-segm

CLEANFILES = mesh-segm.o mesh-segm
clean:
	rm -f $(CLEANFILES)

.PHONY: all clean check
