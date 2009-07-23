#
# HelloWorld
#
# This C++ program write an "HelloWorld" string on the crt.
#

ECHO=  echo
RM=    rm
MKDIR= mkdir
CP=    cp

MAK= Makefile
BIN= HelloWorld
SRC= $(BIN).cc
SAV= $(BIN).cc~
OBJ= $(BIN).o

OLNDIR= $(HOME)/svn/oln/trunk
DEVDIR= milena/sandbox/green/$(BIN)
SRCDIR= $(OLNDIR)/$(DEVDIR)
BINDIR= $(OLNDIR)/build/$(DEVDIR)

CCFLAGS= -ggdb -O0 -Wall -W -pedantic -ansi -pipe

.extension:.cc .hh .o

all: $(BINDIR) $(BINDIR)/$(BIN)


$(BINDIR):
	$(MKDIR) $(BINDIR)
	$(CP) $(SRCDIR)/$(MAK) $(BINDIR)


$(BINDIR)/$(BIN): $(BINDIR)/$(OBJ)
	$(CXX) $(BINDIR)/$(OBJ) $(CCFLAGS) -o $(BINDIR)/$(BIN)

$(BINDIR)/$(OBJ): $(SRCDIR)/$(SRC)
	$(CXX) $(SRCDIR)/$(SRC) $(CCFLAGS) -c -o $(BINDIR)/$(OBJ)

clean:
	-$(RM) $(BINDIR)/$(BIN)
	-$(RM) $(BINDIR)/$(OBJ)
	-$(RM) $(SRCDIR)/$(SAV)

