##
## Makefile for owl
## Made by nicuveo <nicuveo@lrde.epita.fr>
##

include Makefile.rules



# VARIABLES

EXE1  = ../bin/basic
EXE2  = ../bin/gradient

OBJS1 = $(notdir $(CFIL1:.cc=.o))
OBJS2 = $(notdir $(CFIL2:.cc=.o))

CFIL  = $(sort $(CFIL1) $(CFIL2))
OBJS  = $(sort $(OBJS1) $(OBJS2))
DEPS  = $(notdir $(CFIL:.cc=.d))



# MAIN

all: $(EXE1) $(EXE2)

basic: $(EXE1)

gradient : $(EXE2)

clean: do_clean

distclean: do_distclean



# CLEAN

do_clean:
	rm -f $(OBJS)

do_distclean: do_clean
	rm -f $(EXE1) $(EXE2) $(DEPS)
	rm -f Makefile.rules Makefile.deps tmp



# COMPIL

$(EXE1): $(OBJS1)
	$(CC) $(OBJS1) $(LDFLAGS) -o $(EXE1)

$(EXE2): $(OBJS2)
	$(CC) $(OBJS2) $(LDFLAGS) -o $(EXE2)

$(OBJS):
	$(CC) $(CFLAGS) -c $< -o $@



# DEPS

Makefile.deps:
	rm -f $@
	@echo "-- Regenerating basic dependencies"
	@for f in $(CFIL) ; do \
	echo "`basename $$f`: \\" | sed 's/\(.*\)\.cc/\1.d/' >> $@; \
	echo "$$f" >> $@ ; done
	@echo "-include $(DEPS)" >> Makefile.deps

$(DEPS):
	@echo "$(CC) $(DEPFLAGS) $< > $@"
	@$(CC) $(DEPFLAGS) $< -MT "$(@:.d=.o) $@" > tmp
	@mv tmp $@



include Makefile.deps
