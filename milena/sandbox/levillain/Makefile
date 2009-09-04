# FIXME: A part of this should move out of this sandbox and into milena/apps.

milena_dir = ../..
img_dir = $(milena_dir)/img

CPPFLAGS = -I. -I$(milena_dir)
CXXFLAGS = -ggdb -Wall -Werror

PROGRAMS = double min-max constrained-connectivity

all: $(PROGRAMS)

check: check-double check-min-max check-constrained-connectivity
check-double: double
	./$<
check-min-max: min-max
	./$<
check-constrained-connectivity: constrained-connectivity soille.pgm
	./$< soille.pgm

CLEANFILES = double min-max constrained-connectivity

clean:
	rm -f $(CLEANFILES)

.PHONY: all check clean
.PHONY: check-double check-min-max check-constrained-connectivity
