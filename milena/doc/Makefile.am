DOXYGEN = doxygen

.PHONY: doc html

doc: html

html-local: Doxyfile
	$(DOXYGEN)

edit = sed -e "s|@ID@|$$Id|" \
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' \
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' \
	   -e 's,@top_builddir\@,$(top_builddir),g' \
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

EXTRA_DIST = Doxyfile.in
CLEANFILES = Doxyfile
# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile: $(top_srcdir)/ChangeLog $(srcdir)/Doxyfile.in
	Id=`grep '^\$$Id' $(top_srcdir)/ChangeLog`; \
	$(edit) $(srcdir)/Doxyfile.in >Doxyfile

clean-local:
	rm -rf html latex
