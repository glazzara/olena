# FIXME: To be overhauled! (See ticket #134).

include $(top_srcdir)/milena/doc/doc.mk

SUBDIRS =		\
  tutorial 		\
  white_paper


DOXYGEN = doxygen



.PHONY: doc-all doc doc-html doc-dev doc-dev-html ref-doc-dev ref-doc-dev-html\
	ref-doc ref-doc-html tutorial tutorial-html white-paper \
	white-paper-html ref-guide ref-guide-html examples fix-refdata \
	fig-convert regen-dist


# Doxygen documentation output directory.
user/html: ref-doc-html

user/latex/refman.pdf: ref-doc


doc-all: doc doc-html doc-dev doc-dev-html


doc: tutorial white-paper ref-guide ref-doc

doc-html: tutorial-html white-paper-html ref-guide-html ref-doc-html 

doc-dev: tutorial ref-guide white-paper ref-doc-dev 

doc-dev-html: tutorial-html ref-guide-html white-paper-html ref-doc-dev-html 



ref-doc-dev: Doxyfile examples
	$(DOXYGEN) Doxyfile_complete_pdf
	cd complete/latex && make $(AM_MAKEFLAGS)

ref-doc-dev-html: Doxyfile examples
	$(DOXYGEN) Doxyfile_complete


ref-doc: Doxyfile examples
	$(DOXYGEN) Doxyfile_user_pdf
	cd user/latex && make $(AM_MAKEFLAGS)

ref-doc-html: Doxyfile examples
	$(DOXYGEN) Doxyfile_user



tutorial: examples fig-convert
	cd tutorial && $(MAKE) $(AM_MAKEFLAGS) $@

tutorial-html: examples fig-convert
	cd tutorial && $(MAKE) $(AM_MAKEFLAGS) $@


white-paper:
	cd white_paper && $(MAKE) $(AM_MAKEFLAGS) $@

white-paper-html:
	cd white_paper && $(MAKE) $(AM_MAKEFLAGS) $@


ref-guide: examples
	cd ref_guide && $(MAKE) $(AM_MAKEFLAGS) $@

ref-guide-html: examples
	cd ref_guide && $(MAKE) $(AM_MAKEFLAGS) $@


examples:
	cd examples && $(MAKE) $(AM_MAKEFLAGS) $@


fix-refdata:
	cd examples && $(MAKE) $(AM_MAKEFLAGS) $@

fig-convert:
	@failcom='exit 1'; 							\
	list="$(FIGURES_BUILDDIR)/*.p*m"; for img in $$list; do 		\
	  echo "Converting $$img to png"; 					\
	  name=`basename $$img` 						\
	  ext=`echo $$name | cut -d '.' -f 2`; 					\
	  convert -scale 250 $$img $(FIGURES_BUILDDIR)/`basename $$img $$ext`png\
	    || eval $$failcom; 							\
	done; 

edit = sed -e "s|@ID@|$$Id|" 					\
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' 		\
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' 	\
	   -e 's,@top_builddir\@,$(top_builddir),g' 		\
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

edit_pdf = sed -e 's,GENERATE_LATEX         = NO,GENERATE_LATEX = YES,g' \
	       -e 's,GENERATE_HTML          = YES,GENERATE_HTML = NO,g'

# FIXME: This is not good.  We should set these parameters for both
# documentation (complete and user) using @VARIABLES@.  Don't generate
# Doxyfile_user from Doxyfile_complete!  Both should be a product
# derived from a single source, Doxyfile.in.
edit_user = sed -e 's,OUTPUT_DIRECTORY       = ./complete/,OUTPUT_DIRECTORY       = ./user/,g' 		\
		-e 's,EXTRACT_ALL            = YES,EXTRACT_ALL            = NO,g'			\
		-e 's,EXTRACT_PRIVATE        = YES,EXTRACT_PRIVATE        = NO,g'			\
		-e 's,EXTRACT_STATIC         = YES,EXTRACT_STATIC         = NO,g'			\
		-e 's,EXTRACT_LOCAL_CLASSES  = YES,EXTRACT_LOCAL_CLASSES  = NO,g'			\
		-e 's,HIDE_UNDOC_MEMBERS     = NO,HIDE_UNDOC_MEMBERS      = YES,g'			\
		-e 's,HIDE_UNDOC_CLASSES     = NO,HIDE_UNDOC_CLASSES      = YES,g'			\
		-e 's,HIDE_FRIEND_COMPOUNDS  = NO,HIDE_FRIEND_COMPOUNDS   = YES,g'			\
		-e 's,HIDE_IN_BODY_DOCS      = NO,HIDE_IN_BODY_DOCS       = YES,g'			\
		-e 's,INTERNAL_DOCS          = YES,INTERNAL_DOCS          = NO,g'			\
		-e 's,GENERATE_TODOLIST      = YES,GENERATE_TODOLIST      = NO,g'			\
		-e 's,PROJECT_NUMBER         = \",PROJECT_NUMBER          = \"User documentation ,g'	\
		-e 's,EXCLUDE_SYMBOLS	       =,EXCLUDE_SYMBOLS	  = *::internal* *_ mln::trait::*,g'	


regen-dist: $(srcdir)/headers.stamp

$(srcdir)/examples/examples.mk: $(srcdir)/headers.stamp
$(srcdir)/figures/figures.mk: $(srcdir)/headers.stamp
$(srcdir)/outputs/outputs.mk: $(srcdir)/headers.stamp


EXTRA_DIST = $(srcdir)/headers.stamp
$(srcdir)/headers.stamp: $(srcdir)/generate_dist_files.sh
	@rm -f $@.tmp
	@touch $@.tmp
	cd $(srcdir) && ./generate_dist_files.sh
	@mv -f $@.tmp $@


include $(srcdir)/examples/examples.mk
include $(srcdir)/figures/figures.mk
include $(srcdir)/outputs/outputs.mk


EXTRA_DIST += 			\
  Doxyfile.in 			\
  groups 			\
  user/html 			\
  user/latex/refman.pdf 	\
  tools/clearbanner.sh 		\
  tools/sample_utils.hh 	\
  tools/split_sample.sh 	\
  tools/todoxygen.sh

CLEANFILES = 		\
Doxyfile_complete 	\
Doxyfile_user 		

# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile: $(top_srcdir)/milena/ChangeLog $(srcdir)/Doxyfile.in
	   Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog`;		\
	   $(edit) $(srcdir)/Doxyfile.in >Doxyfile_complete &&		\
	   $(edit_pdf) Doxyfile_complete > Doxyfile_complete_pdf && 	\
	   $(edit_user) Doxyfile_complete >Doxyfile_user && 		\
	   $(edit_pdf) Doxyfile_user > Doxyfile_user_pdf

clean-local:
	rm -rf complete user

