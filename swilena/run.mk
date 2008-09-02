# A test/script wrapper.

RUN = $(top_builddir)/swilena/run
RUN_IN = $(top_srcdir)/swilena/run.in
$(RUN): $(RUN_IN)
	cd $(top_builddir)/swilena && $(MAKE) $(AM_MAKEFLAGS) run
