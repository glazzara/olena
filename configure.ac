#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

m4_pattern_forbid([^OLN_])

AC_PREREQ([2.61])


## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([Olena], [1.0a], [olena@lrde.epita.fr], [olena])

# M4 macros.
AC_CONFIG_MACRO_DIR([m4])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
AM_INIT_AUTOMAKE([1.10 foreign check-news dist-bzip2 nostdinc -Wall])

# Package metadata.
AC_CONFIG_FILES([milena/mln/version.hh])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_NAME], ["$PACKAGE_NAME"],
		   [Package Full name.])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_BUGREPORT], ["$PACKAGE_BUGREPORT"],
		   [Bug report address.])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_STRING], ["$PACKAGE_STRING"],
		   [Full name and version.])
AC_DEFINE_UNQUOTED([OLN_PACKAGE_VERSION], ["$PACKAGE_VERSION"],
		   [Package Version.])

## --------------------- ##
## C++ compiler set up.  ##
## --------------------- ##

# If the user doesn't provide any CXXFLAGS, prevent Autoconf from
# settings its own default ones (e.g., `-g -O2' for g++).
if test ! ${CXXFLAGS+set}; then
   CXXFLAGS=""
fi

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed up compiling times.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi


# Adjusting warning options according to compilers.
AC_ARG_VAR([WARNINGS_CXXFLAGS])
case "$CXX" in
     # Intel compiler
     *icpc*)
	WARNINGS_CXXFLAGS="-Wall -wd111,193,279,383,444,522,654,810,981,1418"
	;;
     *)
	WARNINGS_CXXFLAGS="-Wall -W"
	;;
esac


# ------------------------------ #
# C++ compiler flags for tests.  #
# ------------------------------ #

# FIXME: We might want to write an Autoconf macro to factor this.

# Standard flags for tests.
AC_ARG_VAR([TESTS_CXXFLAGS])
# We want no optimization for the tests (it slows down compiling
# times), and debugging information.
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS"; then
  TESTS_CXXFLAGS="-O0 -ggdb $WARNINGS_CXXFLAGS"
fi

# Flags for complex tests.
AC_ARG_VAR([TESTS_CXXFLAGS_SPEED])
# We want optimization for complex tests, and keep debugging flags
# (still useful).
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS_SPEED"; then
  TESTS_CXXFLAGS_SPEED="-O3 -DNDEBUG -ggdb -Wall -W"
fi

# Flags for tests with with all debugging features turned on.
AC_ARG_VAR([TESTS_CXXFLAGS_DEBUG])
# We want no optimization for the tests (it slows down compiling
# times), and a lot of debugging features.
# * GNU C++ Library Debug Mode:
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html
# * GNU C++ Library Compile Time Checks (a.k.a. concept checking):
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt12ch29.html
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS_DEBUG"; then
  TESTS_CXXFLAGS_DEBUG="-O0 -ggdb $WARNINGS_CXXFLAGS -D_GLIBCXX_DEBUG -D_GLIBCXX_CONCEPT_CHECKS"
fi


## ------------------------------ ##
## ``Enable Everything'' Switch.  ##
## ------------------------------ ##

# Enable all bundled features (trimesh2, Swilena, apps, tools).  This
# option is useful to maintainers to ensure they do not break optional
# parts while modifying the core of the project. If both
# ``--enable-all'' is set and a given feature is disabled (say,
# ``--disable-apps''), then the disable flag has priority.

# FIXME: Due to with-swilena.m4's limitations, we have to explicitly
# enable SWIG.  Will be repaired when OLN_WITH_SWILENA is revamped.

AC_ARG_ENABLE([all],
              [AS_HELP_STRING([--enable-all],
                              [enable maintainer mode])],
              [
               enable_trimesh=yes
               with_swig=yes
               enable_swilena=yes
dnl<<lrde
               enable_scribo=yes
dnl>>
               enable_apps=yes
               enable_tools=yes
              ])


## ------------------ ##
## Libraries set up.  ##
## ------------------ ##

# Use Libtool.
# To be replaced by a call to LT_INIT as soon as Libtool 2.2 is used.
AC_PROG_LIBTOOL

# Check for Darwin.
AC_CANONICAL_HOST
AM_CONDITIONAL([DARWIN], [echo "$host_os" | grep '^darwin'])


## -------------------- ##
## External libraries.  ##
## -------------------- ##

### Support for FreeImagePlus has been disabled for Olena 1.0.
## OLN_WITH_LIB([FreeImagePlus], [FreeImagePlus.h], [freeimageplus])

# CFITSIO library.
OLN_WITH_LIB([CFITSIO], [fitsio.h], [cfitsio])

# Magick++.
# FIXME: We should handle Magick++ using pkgconfig (and/or
# Magick++-config?) instead of OLN_WITH_LIB.  See how other projects
# handle this.
OLN_WITH_LIB([Magick++], [Magick++.h], [Magick++], [magickxx], [MAGICKXX])

# TIFF library.
OLN_WITH_LIB([TIFF], [tiff.h], [tiff])

# Grassroots DiCoM (GDCM).
OLN_WITH_LIB([GDCM], [gdcm-2.0/gdcmReader.h], [gdcmCommon], [gdcm], [GDCM],
	     [-lgdcmzlib])

# Tesseract
OLN_WITH_LIB([TESSERACT], [tesseract/baseapi.h], [tesseract_full], [tesseract], [TESSERACT])



## ---------------------------- ##
## (Bundled) trimesh2 library.  ##
## ---------------------------- ##

AC_ARG_ENABLE([trimesh],
              [AS_HELP_STRING([--enable-trimesh],
                              [build the (bundled) trimesh2 library])])
AM_CONDITIONAL([ENABLE_TRIMESH], [test x$enable_trimesh = xyes])
AC_CONFIG_SUBDIRS([external/trimesh])

## --------- ##
## Swilena.  ##
## --------- ##

OLN_WITH_SWILENA([enable_swilena=yes], [])
AM_CONDITIONAL([ENABLE_SWILENA], [test x$enable_swilena = xyes])

AC_CONFIG_FILES([swilena/Makefile
		 swilena/python/Makefile])

AC_CONFIG_FILES([swilena/run], [chmod +x swilena/run])

AC_CONFIG_FILES(m4_do([swilena/python/sps],
		      [:swilena/python/sps-common.in],
		      [:swilena/python/sps.in]),
		[chmod +x swilena/python/sps])
AC_CONFIG_FILES(m4_do([swilena/python/sps-local],
		      [:swilena/python/sps-common.in],
		      [:swilena/python/sps-local.in]),
		[chmod +x swilena/python/sps-local])

dnl<<lrde
## -------- ##
## Scribo.  ##
## -------- ##

AM_CONDITIONAL([ENABLE_SCRIBO], [test x$enable_scribo = xyes])

AC_CONFIG_FILES([
  scribo/Makefile
    scribo/demo/Makefile
    scribo/src/Makefile
      scribo/src/binarization/Makefile
      scribo/src/debug/Makefile
      scribo/src/filter/Makefile
      scribo/src/misc/Makefile
      scribo/src/multi_scale/Makefile
      scribo/src/preprocessing/Makefile
      scribo/src/primitive/Makefile
	scribo/src/primitive/extract/Makefile
	scribo/src/primitive/find/Makefile
        scribo/src/primitive/group/Makefile
      scribo/src/table/Makefile
      scribo/src/text/Makefile
])

AC_CONFIG_FILES([scribo/tests/data.hh])

AC_CONFIG_FILES([
  scribo/tests/Makefile
    scribo/tests/filter/Makefile
    scribo/tests/preprocessing/Makefile
    scribo/tests/table/Makefile
    scribo/tests/text/Makefile
    scribo/tests/unit_test/Makefile
])
dnl>>

## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h creation.
AC_CONFIG_HEADERS([config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
    external/Makefile
    milena/Makefile
      milena/doc/Makefile
      milena/doc/examples/Makefile
      milena/doc/technical/Makefile
      milena/doc/tutorial/Makefile
      milena/doc/white_paper/Makefile
      milena/mesh/Makefile
])


## ------- ##
## Tests.  ##
## ------- ##

# Ask for the creation of a milena/tests/data.hh, used to access to
# data (images) from tests.
AC_CONFIG_FILES([milena/tests/data.hh])

# Ask for the creation of tests' Makefiles.
AC_CONFIG_FILES([
  milena/tests/Makefile
    milena/tests/accu/Makefile
    milena/tests/accu/image/Makefile
    milena/tests/accu/site_set/Makefile
    milena/tests/accu/stat/Makefile
    milena/tests/accu/math/Makefile
    milena/tests/accu/shape/Makefile
    milena/tests/algebra/Makefile
    milena/tests/arith/Makefile
    milena/tests/binarization/Makefile
    milena/tests/border/Makefile
    milena/tests/canvas/Makefile
      milena/tests/canvas/browsing/Makefile
      milena/tests/canvas/morpho/Makefile
    milena/tests/convert/Makefile
    milena/tests/convert/impl/Makefile
    milena/tests/core/Makefile
      milena/tests/core/alias/Makefile
      milena/tests/core/image/Makefile
	milena/tests/core/image/dmorph/Makefile
	milena/tests/core/image/imorph/Makefile
	milena/tests/core/image/vmorph/Makefile
      milena/tests/core/other/Makefile
      milena/tests/core/routine/Makefile
      milena/tests/core/site_set/Makefile
    milena/tests/data/Makefile
      milena/tests/data/approx/Makefile
      milena/tests/data/naive/Makefile
    milena/tests/debug/Makefile
    milena/tests/display/Makefile
    milena/tests/draw/Makefile
    milena/tests/estim/Makefile
    milena/tests/extension/Makefile
    milena/tests/fun/Makefile
      milena/tests/fun/i2v/Makefile
      milena/tests/fun/p2b/Makefile
      milena/tests/fun/p2p/Makefile
      milena/tests/fun/p2v/Makefile
      milena/tests/fun/stat/Makefile
      milena/tests/fun/v2v/Makefile
      milena/tests/fun/vv2v/Makefile
      milena/tests/fun/x2x/Makefile
    milena/tests/geom/Makefile
    milena/tests/graph/Makefile
      milena/tests/graph/attribute/Makefile
    milena/tests/histo/Makefile
    milena/tests/io/Makefile
      milena/tests/io/dicom/Makefile
      milena/tests/io/dump/Makefile
      milena/tests/io/fits/Makefile
      milena/tests/io/fld/Makefile
      milena/tests/io/magick/Makefile
      milena/tests/io/off/Makefile
      milena/tests/io/pbm/Makefile
      milena/tests/io/pbms/Makefile
      milena/tests/io/pfm/Makefile
      milena/tests/io/pgm/Makefile
      milena/tests/io/pgms/Makefile
      milena/tests/io/pnm/Makefile
      milena/tests/io/ppm/Makefile
      milena/tests/io/ppms/Makefile
      milena/tests/io/tiff/Makefile
    milena/tests/labeling/Makefile
    milena/tests/linear/Makefile
      milena/tests/linear/local/Makefile
    milena/tests/literal/Makefile
    milena/tests/logical/Makefile
    milena/tests/make/Makefile
    milena/tests/math/Makefile
    milena/tests/metal/Makefile
      milena/tests/metal/make/Makefile
      milena/tests/metal/math/Makefile
    milena/tests/morpho/Makefile
      milena/tests/morpho/approx/Makefile
      milena/tests/morpho/attribute/Makefile
      milena/tests/morpho/closing/Makefile
      milena/tests/morpho/closing/approx/Makefile
      milena/tests/morpho/elementary/Makefile
      milena/tests/morpho/opening/Makefile
      milena/tests/morpho/opening/approx/Makefile
      milena/tests/morpho/reconstruction/Makefile
        milena/tests/morpho/reconstruction/by_dilation/Makefile
        milena/tests/morpho/reconstruction/by_erosion/Makefile
      milena/tests/morpho/tree/Makefile
	milena/tests/morpho/tree/filter/Makefile
      milena/tests/morpho/watershed/Makefile
    milena/tests/norm/Makefile
    milena/tests/opt/Makefile
    milena/tests/pw/Makefile
    milena/tests/set/Makefile
    milena/tests/tag/Makefile
    milena/tests/test/Makefile
    milena/tests/topo/Makefile
      milena/tests/topo/skeleton/Makefile
    milena/tests/trace/Makefile
    milena/tests/trait/Makefile
      milena/tests/trait/image/Makefile
      milena/tests/trait/op/Makefile
      milena/tests/trait/value/Makefile
    milena/tests/transform/Makefile
    milena/tests/unit_test/Makefile
    milena/tests/upscaling/Makefile
      milena/tests/upscaling/art/Makefile
    milena/tests/util/Makefile
    milena/tests/value/Makefile
      milena/tests/value/builtin/Makefile
      milena/tests/value/concept/Makefile
    milena/tests/win/Makefile
    milena/tests/world/Makefile
      milena/tests/world/binary_2d/Makefile
      milena/tests/world/inter_pixel/Makefile
	milena/tests/world/inter_pixel/dim2/Makefile
])

dnl<<lrde
AC_CONFIG_FILES([
  milena/tests/extract/Makefile
  milena/tests/linear/gaussian/Makefile
  milena/tests/registration/Makefile
  milena/tests/subsampling/Makefile
])
dnl>>


## -------------- ##
## Applications.  ##
## -------------- ##

# Ask for the creation of a milena/apps/data.hh, used to access to
# data (images) from apps.
AC_CONFIG_FILES([milena/apps/data.hh])

AC_ARG_ENABLE([apps],
              [AS_HELP_STRING([--enable-apps],
                              [enable application])])
AM_CONDITIONAL([ENABLE_APPS], [test "x$enable_apps" = "xyes"])

# Ask for the creation of applications' Makefiles.
AC_CONFIG_FILES([
  milena/apps/Makefile
    milena/apps/constrained-connectivity/Makefile
    milena/apps/graph-morpho/Makefile
    milena/apps/mesh-segm-skel/Makefile
    milena/apps/papers/Makefile
      milena/apps/papers/levillain.09.ismm/Makefile
])

# Configure tests.
# FIXME: Consider using `sed' instead of `configure' to create these
# tests for the sake of speed.
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-max-curv],
		[chmod +x milena/apps/mesh-segm-skel/test-mesh-max-curv])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-max-curv],
		[chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-max-curv])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-segm],
		[chmod +x milena/apps/mesh-segm-skel/test-mesh-segm])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-segm],
		[chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-segm])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-max-curv-segm],
		[chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-max-curv-segm])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-skel],
		[chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-skel])

AC_CONFIG_FILES([milena/apps/constrained-connectivity/test-constrained-connectivity],
		[chmod +x milena/apps/constrained-connectivity/test-constrained-connectivity])

# Flags for apps.
AC_ARG_VAR([APPS_CXXFLAGS])
# We want fast binaries for apps.
if test "$GXX" = yes && test -z "$APPS_CXXFLAGS"; then
  APPS_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
fi

## ------- ##
## Tools.  ##
## ------- ##

AC_ARG_ENABLE([tools],
              [AS_HELP_STRING([--enable-tools],
                              [enable tools])])
AM_CONDITIONAL([ENABLE_TOOLS], [test "x$enable_tools" = "xyes"])

# Ask for the creation of tools' Makefiles.
AC_CONFIG_FILES([milena/tools/Makefile])

# Flags for tools.
AC_ARG_VAR([TOOLS_CXXFLAGS])
# We want fast binaries for tools.
if test "$GXX" = yes && test -z "$TOOLS_CXXFLAGS"; then
  TOOLS_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
fi


AC_OUTPUT
