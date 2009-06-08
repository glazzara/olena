# tests-recursive.mk					-*- Automake -*-
# Compile tests recursively, but don't run them.

include $(top_srcdir)/build-aux/extra-recursive-targets.mk

# Sometimes, we just want to check whether tests compile, without
# actually running them.  Hence this `tests' target, which does half
# the job of the `check' target.
EXTRA_RECURSIVE_TARGETS += tests-recursive
.PHONY: tests-am tests-recursive tests
# ``Halved'' version of check-am.
tests-am: $(check_PROGRAMS)
	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
tests: tests-recursive
