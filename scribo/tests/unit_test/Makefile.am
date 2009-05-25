## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

.PHONY: regen-tests
regen-tests: $(srcdir)/unit-tests.mk
# FIXME: Change build_unit_test.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
# `$(abs_top_srcdir)' is required because of the change of directory
# at the beginning og the action.
$(srcdir)/unit-tests.mk: $(srcdir)/build_unit_test.sh
	cd $(srcdir)						\
	&& ./build_unit_test.sh $(abs_top_srcdir)/scribo	\
	&& chmod a-w unit-tests.mk

include $(srcdir)/unit-tests.mk

MAINTAINERCLEANFILES = $(srcdir)/scribo_*.cc

TESTS = $(check_PROGRAMS)
