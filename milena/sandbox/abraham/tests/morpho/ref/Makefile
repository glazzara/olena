TOPOWSHD = .
# Version LINUX
  CC = cc
  CCFLAGS = -g -DUNIXIO
  ODIR = $(TOPOWSHD)/obj
  BDIR = $(TOPOWSHD)/bin
  IDIR = $(TOPOWSHD)/include
  CDIR = $(TOPOWSHD)/src/com
  LDIR = $(TOPOWSHD)/src/lib
  LIBS = -lm
  OBJ_COMMON = $(ODIR)/mcimage.o

all:	$(BDIR)/wshedtopo

clean:
	rm -f $(TOPOWSHD)/bin/*
	rm -f $(TOPOWSHD)/obj/*

# ===============================================================
# EXECUTABLES
# ===============================================================

$(BDIR)/wshedtopo:	$(CDIR)/wshedtopo.c $(IDIR)/mcimage.h $(IDIR)/mclifo.h $(IDIR)/mcindic.h $(IDIR)/mcfahsalembier.h $(IDIR)/mccodimage.h $(IDIR)/mccomptree.h $(IDIR)/lwshedtopo.h $(OBJ_COMMON) $(ODIR)/mclifo.o $(ODIR)/mcindic.o $(ODIR)/mcfahsalembier.o $(ODIR)/mccodimage.o $(ODIR)/mccomptree.o $(ODIR)/mcunionfind.o $(ODIR)/lwshedtopo.o
	$(CC) $(CCFLAGS) -I$(IDIR) $(CDIR)/wshedtopo.c $(OBJ_COMMON) $(ODIR)/mcindic.o $(ODIR)/mclifo.o $(ODIR)/mcfahsalembier.o $(ODIR)/mccodimage.o $(ODIR)/mccomptree.o $(ODIR)/mcunionfind.o $(ODIR)/lwshedtopo.o $(LIBS) -o $(BDIR)/wshedtopo

# *********************************
# OBJECTS
# *********************************

$(ODIR)/lwshedtopo.o:	$(LDIR)/lwshedtopo.c $(LDIR)/lattrib.c $(IDIR)/mcimage.h $(IDIR)/mccodimage.h $(IDIR)/mclifo.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/lwshedtopo.c -o $(ODIR)/lwshedtopo.o

$(ODIR)/mccodimage.o:	$(LDIR)/mccodimage.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mccodimage.c -o $(ODIR)/mccodimage.o

$(ODIR)/mccomptree.o:	$(LDIR)/mccomptree.c $(IDIR)/mccomptree.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mccomptree.c -o $(ODIR)/mccomptree.o

$(ODIR)/mcfahsalembier.o:	$(LDIR)/mcfahsalembier.c $(IDIR)/mccodimage.h $(IDIR)/mcfahsalembier.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcfahsalembier.c -o $(ODIR)/mcfahsalembier.o

$(ODIR)/mcimage.o:	$(LDIR)/mcimage.c $(IDIR)/mccodimage.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcimage.c -o $(ODIR)/mcimage.o

$(ODIR)/mcindic.o:	$(LDIR)/mcindic.c $(IDIR)/mccodimage.h $(IDIR)/mcindic.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcindic.c -o $(ODIR)/mcindic.o

$(ODIR)/mclifo.o:	$(LDIR)/mclifo.c $(IDIR)/mclifo.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mclifo.c -o $(ODIR)/mclifo.o

$(ODIR)/mcunionfind.o:	$(LDIR)/mcunionfind.c $(IDIR)/mcunionfind.h
	$(CC) -c $(CCFLAGS) -I$(IDIR) $(LDIR)/mcunionfind.c -o $(ODIR)/mcunionfind.o
