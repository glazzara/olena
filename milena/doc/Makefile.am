# FIXME: To be overhauled! (See ticket #134).

include $(top_srcdir)/milena/doc/doc.mk

SUBDIRS = tutorial white_paper

DOXYGEN = doxygen

.PHONY: doc user-doc complete-doc html-complete html-user tutorial white-paper regen-dist examples

doc: user-doc tutorial white_paper ref-guide

complete-doc: html-complete

user-doc: html-user

html-complete: Doxyfile tuto-html ref-guide-html
	$(DOXYGEN) Doxyfile_complete

html-user: Doxyfile tuto-html ref-guide-html
	$(DOXYGEN) Doxyfile_user

tuto-html: examples fig-convert
	$(MAKE) -C tutorial tuto-html

tutorial: examples fig-convert
	$(MAKE) -C tutorial tutorial

white-paper:
	$(MAKE) -C white_paper white-paper

ref-guide:
	$(MAKE) -C ref_guide ref-guide

ref-guide-html:
	$(MAKE) -C ref_guide ref-guide-html


examples:
	make -C examples examples

fix-refdata:
	make -C examples fix-refdata

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
user 				\
tools/sample_utils.hh 		\
tools/split_sample.sh 		\
tools/todoxygen.sh

CLEANFILES = 		\
Doxyfile_complete 	\
Doxyfile_user 		

# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile: $(top_srcdir)/milena/ChangeLog $(srcdir)/Doxyfile.in
	   Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog`;	\
	   $(edit) $(srcdir)/Doxyfile.in >Doxyfile_complete &&	\
	   $(edit_user) Doxyfile_complete >Doxyfile_user

clean-local:
	rm -rf complete user

