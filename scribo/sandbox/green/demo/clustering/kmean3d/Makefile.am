#
# Generic Makefile
#

#########
# TOOLS #
#########

#LOADLIBES=      -lboost_filesystem
INCLUDES1=       -I$(HOME)/git/olena/scribo/sandbox/green
INCLUDES2=       -I$(HOME)/git/olena/milena
INCLUDES=	$(INCLUDES1) $(INCLUDES2)
CXXFLAGS=       -ggdb -O0 -Wall -W -pedantic -ansi -pipe $(INCLUDES)
#CXXFLAGS=       -DNDEBUG -O1 -Wall -W -pedantic -ansi -pipe $(INCLUDES)
#CXXFLAGS=       -DNDEBUG -O3 -Wall -W -pedantic -ansi -pipe $(INCLUDES)
ECHO=           echo
RM=             rm
MKDIR=          mkdir -p
CP=             cp

SOURCE_PATTERN= green/demo
BUILD__PATTERN= green/build/demo


ifeq ($(findstring $(BUILD__PATTERN),$(PWD)), $(BUILD__PATTERN))
# Case where make is done from build directory.
SOURCE_DIR=     $(subst $(BUILD__PATTERN),$(SOURCE_PATTERN),$(PWD))
BUILD__DIR=     $(PWD)/
else
# Case where make is done from source directory.
SOURCE_DIR=     $(PWD)/
BUILD__DIR=     $(subst $(SOURCE_PATTERN),$(BUILD__PATTERN),$(PWD))
endif

SRC=            $(notdir $(wildcard $(SOURCE_DIR)/*.cc))
OLD=            $(notdir $(wildcard $(SOURCE_DIR)/*~))
OBJ=            $(patsubst %.cc,%.o,$(SRC))
SOURCE_MAKEFILE=Makefile.am
BUILD__MAKEFILE=Makefile
TARGET_FILE=    $(notdir $(PWD))
SOURCE_FILES=   $(notdir $(wildcard $(SOURCE_DIR)/*.*))
BUILD__FILES=   $(filter-out $(SRC) $(SOURCE_MAKEFILE), $(SOURCE_FILES))

BUILD__F_PATH=  $(addprefix $(BUILD__DIR)/,$(BUILD__FILES))
SOURCE_F_PATH=  $(addprefix $(SOURCE_DIR)/,$(SOURCE_FILES))

BUILD__M_PATH=  $(addprefix $(BUILD__DIR)/,$(BUILD__MAKEFILE))
SOURCE_M_PATH=  $(addprefix $(SOURCE_DIR)/,$(SOURCE_MAKEFILE))

TARGET_F_PATH=  $(addprefix $(BUILD__DIR)/,$(TARGET_FILE))
OBJ_F_PATH=     $(addprefix $(BUILD__DIR)/,$(OBJ))
SRC_F_PATH=     $(addprefix $(SOURCE_DIR)/,$(SRC))
OLD_F_PATH=     $(addprefix $(SOURCE_DIR)/,$(OLD))

#############
# BOOTSTRAP #
#############


bootstrap: $(BUILD__DIR) $(BUILD__F_PATH) $(BUILD__M_PATH)

# Create, if nessary, the destination directory
$(BUILD__DIR):
	$(MKDIR) $(BUILD__DIR)

# Copy, if nessary, all the files, except the Makefile.am
$(BUILD__F_PATH): $(SOURCE_F_PATH)
	$(CP) $(addprefix $(SOURCE_DIR),$(@F)) $@

# Copy if nessary, the Makefile.am into Makefile
$(BUILD__M_PATH): $(SOURCE_M_PATH)
	$(CP) $(SOURCE_M_PATH) $(BUILD__M_PATH)


#######
# ALL #
#######

# We assume that the call is done from the build directory.
# With the directive vpath, hidden files are found in the source directory.

all: $(TARGET_F_PATH)


$(TARGET_F_PATH): $(OBJ_F_PATH)
	$(LINK.cc) $< $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ_F_PATH):$(SRC_F_PATH)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<


#########
# CLEAN #
#########

# Force every time the deletion
clean: clean_target clean_obj clean_dst clean_old #clean_make


clean_target:
	-@$(RM) $(TARGET_F_PATH)  &> /dev/null

clean_obj:
	-@$(RM) $(OBJ_F_PATH) &> /dev/null

clean_dst:
	-@$(RM) $(BUILD_F_PATH) &> /dev/null

clean_make:
	-@$(RM) $(BUILD_M_PATH) &> /dev/null

clean_old:
	-@$(RM) $(OLD_F_PATH) &> /dev/null


#########
# PRINT #
#########

print: print_tools print_bootstrap

print_tools:
	@$(ECHO) "HOME            = $(HOME)"
	@$(ECHO) "INCLUDES        = $(INCLUDES)"
	@$(ECHO) "CXXFLAGS        = $(CXXFLAGS)"
	@$(ECHO) "ECHO            = $(ECHO)"
	@$(ECHO) "RM              = $(RM)"
	@$(ECHO) "MKDIR           = $(MKDIR)"
	@$(ECHO) "CP              = $(CP)"
	@$(ECHO)

print_bootstrap:
	@$(ECHO) "PWD             = $(PWD)"
	@$(ECHO) "SOURCE_PATTERN  = $(SOURCE_PATTERN)"
	@$(ECHO) "BUILD__PATTERN  = $(BUILD__PATTERN)"
	@$(ECHO) "SOURCE_DIR      = $(SOURCE_DIR)"
	@$(ECHO) "BUILD__DIR      = $(BUILD__DIR)"
	@$(ECHO) "SOURCE_MAKEFILE = $(SOURCE_MAKEFILE)"
	@$(ECHO) "BUILD__MAKEFILE = $(BUILD__MAKEFILE)"
	@$(ECHO) "TARGET_FILE     = $(TARGET_FILE)"
	@$(ECHO) "SOURCE_FILES    = $(SOURCE_FILES)"
	@$(ECHO) "SOURCE_F_PATH   = $(SOURCE_F_PATH)"
	@$(ECHO) "BUILD__FILES    = $(BUILD__FILES)"
	@$(ECHO) "BUILD__F_PATH   = $(BUILD__F_PATH)"
	@$(ECHO) "BUILD__M_PATH   = $(BUILD__M_PATH)"
	@$(ECHO) "SOURCE_M_PATH   = $(SOURCE_M_PATH)"
	@$(ECHO) "SRC             = $(SRC)"
	@$(ECHO) "OBJ             = $(OBJ)"
	@$(ECHO) "OLD             = $(OLD)"
	@$(ECHO) "SRC_F_PATH      = $(SRC_F_PATH)"
	@$(ECHO) "OBJ_F_PATH      = $(OBJ_F_PATH)"
	@$(ECHO) "OLD_F_PATH      = $(OLD_F_PATH)"
	@$(ECHO)
