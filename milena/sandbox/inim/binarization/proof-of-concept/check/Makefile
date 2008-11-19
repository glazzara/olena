# Vars

PROGS := $(notdir $(wildcard ../bin/*))
IMG   := $(wildcard *.pgm)
HISTS := $(shell seq 2)
PARAM := 5 10 20 40

all: check



# Histo rule generation

define HISTO_RULE
$1-$2/$4.png: $1-$2/$4.histo histo.gp graph.sh
	./graph.sh $$< $$@ "$(W)" "$(H)"
$1-$2/$4.histo: $3 ../bin/$1
	../bin/$1 $3 $1-$2/$3 $1-$2/$(3:.pgm=) $2

TODO += $1-$2/$4.png
endef



# Images rule generation

define IMG_RULE
$1-$2/$3: $3 ../bin/$1
	../bin/$1 $$< $$@ $$(@:.pgm=) $2
TODO += $1-$2/$3
$(foreach num,$(HISTS),$(eval $(call HISTO_RULE,$1,$2,$3,$(3:.pgm=_$(strip $(num))))))
endef



# Folder rule generation

define PROG_RULE
FOLDS += $1-$2
$1-$2: ../bin/$1
$(foreach img,$(IMG),$(eval $(call IMG_RULE,$1,$2,$(strip $(img)))))
endef



# Param rule generation

define PARAM_RULE
$(foreach param,$(PARAM),$(eval $(call PROG_RULE,$1,$(strip $(param)))))
endef



# Main

$(foreach prog,$(PROGS),$(eval $(call PARAM_RULE,$(strip $(prog)))))



# Main

check: $(FOLDS) $(TODO)

$(FOLDS):
	mkdir -p $@

clean:
	find . -name "*.histo" -delete

distclean: clean
	find * -type d \! -path '*svn*' | xargs rm -Rf
