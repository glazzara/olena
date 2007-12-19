DOXYGEN = doxygen

.PHONY: doc user-doc internal-doc html html_user

doc: html

internal-doc: html

user-doc: html_user


html-local: Doxyfile
	$(DOXYGEN) $(srcdir)/Doxyfile_internal

html_user:: Doxyfile
	$(DOXYGEN) $(srcdir)/Doxyfile_user


edit = sed -e "s|@ID@|$$Id|" 					\
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' 		\
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' 	\
	   -e 's,@top_builddir\@,$(top_builddir),g' 		\
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

edit_user = sed -e 's,OUTPUT_DIRECTORY       = ./internal/,OUTPUT_DIRECTORY       = ./user/,g' 		\
		-e 's,EXTRACT_ALL            = YES,EXTRACT_ALL            = NO,g'			\
		-e 's,EXTRACT_PRIVATE        = YES,EXTRACT_PRIVATE        = NO,g'			\
		-e 's,EXTRACT_STATIC         = YES,EXTRACT_STATIC         = NO,g'			\
		-e 's,EXTRACT_LOCAL_CLASSES  = YES,EXTRACT_LOCAL_CLASSES  = NO,g'			\
		-e 's,HIDE_UNDOC_MEMBERS     = NO,HIDE_UNDOC_MEMBERS     = YES,g'			\
		-e 's,HIDE_UNDOC_CLASSES     = NO,HIDE_UNDOC_CLASSES     = YES,g'			\
		-e 's,HIDE_FRIEND_COMPOUNDS  = NO,HIDE_FRIEND_COMPOUNDS  = YES,g'			\
		-e 's,HIDE_IN_BODY_DOCS      = NO,HIDE_IN_BODY_DOCS      = YES,g'			\
		-e 's,INTERNAL_DOCS          = YES,INTERNAL_DOCS          = NO,g'			\
		-e 's,GENERATE_TODOLIST      = YES,GENERATE_TODOLIST      = NO,g'			\
		-e 's,PROJECT_NUMBER         = \",PROJECT_NUMBER         = \"User documentation ,g'

EXTRA_DIST = Doxyfile.in

CLEANFILES = Doxyfile_internal \
	     Doxyfile_user

# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile: $(top_srcdir)/ChangeLog $(srcdir)/Doxyfile.in
	   Id=`grep '^\$$Id' $(top_srcdir)/ChangeLog`; 			\
	   $(edit) $(srcdir)/Doxyfile.in >Doxyfile_internal; 		\
	   $(edit_user) $(srcdir)/Doxyfile_internal >Doxyfile_user

clean-local:
	rm -rf internal user

