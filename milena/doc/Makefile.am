DOXYGEN = doxygen

.PHONY: doc user-doc internal-doc html html_user

doc: html

internal-doc: html

user-doc: html_user


html-local: Doxyfile
	$(DOXYGEN)

html_user: Doxyfile_user
	$(DOXYGEN)


edit = sed -e "s|@ID@|$$Id|" \
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	   -e 's,@top_builddir\@,$(top_builddir),g' \
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

EXTRA_DIST = Doxyfile.in \
             Doxyfile_user.in
CLEANFILES = Doxyfile
# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile: $(top_srcdir)/ChangeLog $(srcdir)/Doxyfile.in
	Id=`grep '^\$$Id' $(top_srcdir)/ChangeLog`; \
	$(edit) $(srcdir)/Doxyfile.in >Doxyfile

Doxyfile_user: $(top_srcdir)/ChangeLog $(srcdir)/Doxyfile_user.in
	Id=`grep '^\$$Id' $(top_srcdir)/ChangeLog`; \
	$(edit) $(srcdir)/Doxyfile_user.in >Doxyfile

clean-local:
	rm -rf html latex user

