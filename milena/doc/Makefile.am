# FIXME: To be overhauled! (See ticket #134).

include $(top_srcdir)/milena/doc/doc.mk

SUBDIRS = tutorial

DOXYGEN = doxygen

.PHONY: doc user-doc complete-doc html_complete html_user

doc: complete-doc

complete-doc: html_complete

user-doc: html_user tutorial

html_complete: tuto_html figures Doxyfile
	$(DOXYGEN) Doxyfile_complete

html_user: tuto_html figures Doxyfile
	$(DOXYGEN) Doxyfile_user

figures: all
	@failcom='exit 1'; 							\
	list='${wildcard $(FIGURES_DIR)/*.p*m}'; for img in $$list; do 		\
	  echo "Converting $$img to png"; 					\
	  convert $$img $(FIGURES_DIR)/`basename $$img .ppm`.png 		\
	    || eval $$failcom; 							\
	done; 

tuto_html:
	$(MAKE) -C tutorial tuto_html

tuto: all
	$(MAKE) -C tutorial tuto

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
		-e 's,EXCLUDE_SYMBOLS	     =,EXCLUDE_SYMBOLS	       	  = *::internal*,g'	



EXTRA_DIST = Doxyfile.in

CLEANFILES = Doxyfile_complete \
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

