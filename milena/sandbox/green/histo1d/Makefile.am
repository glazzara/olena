#
# Generic Makefile
#

BIN_PATTERN= trunk/build/milena
SRC_PATTERN= trunk/milena

CXXFLAGS=    -ggdb -O0 -Wall -W -pedantic -ansi -pipe
ECHO=        echo
RM=          rm
MKDIR=       mkdir
CP=          cp

ifeq ($(findstring $(BIN_PATTERN),$(PWD)), $(BIN_PATTERN))
BIN_DIR=
SRC_DIR= $(subst $(BIN_PATTERN),$(SRC_PATTERN), $(PWD))/
else
BIN_DIR= $(subst $(SRC_PATTERN),$(BIN_PATTERN), $(PWD))/
SRC_DIR=
endif

TARGET= $(BIN_DIR)$(notdir $(PWD))
BIN_MAKE= $(BIN_DIR)Makefile
SRC_MAKE= $(SRC_DIR)Makefile.am


SRC_FILES= $(wildcard $(SRC_DIR)*.cc)
OLD_FILES= $(addsuffix ~, $(SRC_FILES))
OBJ_FILES= $(addprefix $(BIN_DIR), $(notdir $(SRC_FILES:%.cc=%.o)))


all: $(BIN_DIR) $(TARGET)
	@$(ECHO) "Compilation terminée" 

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)
	$(CP) $(SRC_MAKE) $(BIN_MAKE)

$(TARGET):$(OBJ_FILES)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(BIN_DIR)%.o:$(SRC_DIR)%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

clean:
	-@$(RM) $(TARGET)
	-@$(RM) $(OBJ_FILES)
	-@$(RM) $(OLD_FILES)

