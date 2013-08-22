# Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 EPITA
# Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

m4_pattern_forbid([^OLN_])

AC_PREREQ([2.61])


## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([Olena], [2.0a], [olena@lrde.epita.fr], [olena],
        [http://olena.lrde.epita.fr])

# M4 macros.
AC_CONFIG_MACRO_DIR([m4])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
#
# Disable the -Wextra-portability option until developers can all use
# Automake 1.11.2 or newer (Debian GNU/Linux 6 ships Automake 1.11.1
# only), at which point we'll use the new AM_PROG_AR macro instead.
AM_INIT_AUTOMAKE([1.11 subdir-objects check-news tar-pax dist-bzip2
                  parallel-tests nostdinc silent-rules
                  -Wall -Wno-extra-portability])
# By default, let "make" hide the compilation command and display a
# "GEN ..." message, unless V=1 is passed to "make".
AM_SILENT_RULES([yes])

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

# Set `ICPC' to `yes' if the Intel C++ compiler is used.
test $CXX --version 2>/dev/null | grep '\bICC\b' >/dev/null 2>&1 && ICPC=yes

# GNU C++ compiler setup.
if test "$GXX" = yes; then
  # Speed up compiling times.
  CXXFLAGS="$CXXFLAGS -pipe"

  # The code generated for mln::data::impl::memcpy__() by g++ 4.2 with
  # a high optimization level (`-O3') and without
  # `-fno-strict-aliasing' might be wrong, at least with Debian's g++
  # 4.2 on IA-32 (see also milena/mln/memcpy_.hh).  We observed this
  # behavior with e.g. milena/apps/graph-morpho/samples-image2d.cc.
  # Note that Debian's g++ 4.0, 4.1, 4.3 and 4.4 are fine.
  #
  # So, when the C++ compiler is g++ 4.2, set STRICT_ALIASING_CXXFLAGS
  # to `-fno-strict-aliasing'.
  if $CXX --version | head -n 1 | grep '\b4\.2' >/dev/null 2>&1; then
    STRICT_ALIASING_CXXFLAGS=-fno-strict-aliasing
  fi
fi
AC_SUBST([STRICT_ALIASING_CXXFLAGS])


# Adjusting warning options according to compilers.
AC_ARG_VAR([WARNINGS_CXXFLAGS], [C++ compiler warning flags])
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
AC_ARG_VAR([TESTS_CXXFLAGS], [C++ compiler flags for tests])
# We want no optimization for the tests (it slows down compiling
# times), and debugging information.
if test -z "$TESTS_CXXFLAGS"; then
  if test "$GXX" = yes; then
    # GNU C++ compiler setup.
    TESTS_CXXFLAGS="-O0 -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    # Intel C++ compiler setup.
    TESTS_CXXFLAGS="-O0 -g $WARNINGS_CXXFLAGS"
  fi
fi

# Flags for complex tests.
AC_ARG_VAR([TESTS_CXXFLAGS_SPEED],
           [C++ compiler optimization flags for (complex) tests])
# We want optimization for complex tests, and keep debugging flags
# (still useful).
if test -z "$TESTS_CXXFLAGS_SPEED"; then
  if test "$GXX" = yes; then
    # GNU C++ compiler setup.
    TESTS_CXXFLAGS_SPEED="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    # Intel C++ compiler setup.
    TESTS_CXXFLAGS_SPEED="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi

# Flags for tests with with all debugging features turned on.
AC_ARG_VAR([TESTS_CXXFLAGS_DEBUG], [C++ compiler debug flags])
# We want no optimization for the tests (it slows down compiling
# times), and a lot of debugging features.
# * GNU C++ Library Debug Mode:
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html
# * GNU C++ Library Compile Time Checks (a.k.a. concept checking):
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt12ch29.html
if test -z "$TESTS_CXXFLAGS_DEBUG"; then
  if test "$GXX" = yes; then
    TESTS_CXXFLAGS_DEBUG="-O0 -ggdb $WARNINGS_CXXFLAGS -D_GLIBCXX_DEBUG -D_GLIBCXX_CONCEPT_CHECKS"
  elif test "$ICPC" = yes; then
    TESTS_CXXFLAGS_DEBUG="-O0 -g $WARNINGS_CXXFLAGS -D_GLIBCXX_DEBUG -D_GLIBCXX_CONCEPT_CHECKS"
  fi
fi


## --------------- ##
## Documentation.  ##
## --------------- ##

# Tools used to build the documentation.

# rst2html can have many names, especially in the MacPorts, where
# py-docutils binaries include their version numbers in their names.
AC_ARG_VAR([RST2HTML], [reStructuredText-to-HTML translator])
AC_CHECK_PROGS([RST2HTML],
  m4_do([rst2html],
        [ rst2html.py],
        [ rst2html-3.3.py rst2html-3.2.py rst2html-3.1.py],
        [ rst2html-2.7.py rst2html-2.6.py rst2html-2.5.py rst2html-2.4.py]))
if test -z "$RST2HTML"; then
  AC_MSG_WARN([rst2html not found, documentation rebuild will not be possible])
fi

AC_ARG_VAR([BIB2BIB], [BibTeX extracting tool])
AC_CHECK_PROGS([BIB2BIB], [bib2bib])
if test -z "$BIB2BIB"; then
  AC_MSG_WARN([bib2bib not found, documentation rebuild will not be possible])
fi

AC_ARG_VAR([BIBTEX2HTML], [BibTeX-to-HTML translator])
AC_CHECK_PROGS([BIBTEX2HTML], [bibtex2html])
if test -z "$BIBTEX2HTML"; then
  AC_MSG_WARN(m4_do([bibtex2html not found,],
                    [ documentation rebuild will not be possible]))
fi


## ------------------------------ ##
## ``Enable Everything'' Switch.  ##
## ------------------------------ ##

# Enable all bundled features (Swilena, apps, tools).  This
# option is useful to maintainers to ensure they do not break optional
# parts while modifying the core of the project. If both
# ``--enable-all'' is set and a given feature is disabled (say,
# ``--disable-apps''), then the disable flag has priority.

AC_ARG_ENABLE([all],
  [AS_HELP_STRING([--enable-all], [enable maintainer mode])],
  [if test "x$enable_all" != xno; then
     enable_swilena=yes
     enable_scribo=yes
     enable_apps=yes
     enable_tools=yes
   fi])


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

# ------- #
# Boost.  #
# ------- #

# Check if Boost is available, if not just print a warning, otherwise
# look for boost:tuple.
BOOST_REQUIRE(1.3, [AC_DEFINE([HAVE_BOOST], [0])])
BOOST_TUPLE
AM_CONDITIONAL([HAVE_BOOST_TUPLE],
               [test "x$ac_cv_header_boost_tuple_tuple_hpp" = xyes])

# ----------------- #
# CFITSIO library.  #
# ----------------- #

OLN_WITH_LIB([CFITSIO], [fitsio.h], [cfitsio])

# ---------- #
# Magick++.  #
# ---------- #

OLN_WITH_LIB_PKG_CONFIG([ImageMagick++], [Magick++.h], [Magick++],
                        [imagemagickxx], [IMAGEMAGICKXX])

OLN_WITH_LIB_PKG_CONFIG([GraphicsMagick++], [Magick++.h], [GraphicsMagick++],
                        [graphicsmagickxx], [GRAPHICSMAGICKXX])

# If both ImageMagick and GraphicsMagic are available, choose the
# former by default.  (Use `--with-imagemagickxx=no' to ignore an
# installation of ImageMagick and let configure try GraphicsMagick).
if test "x$with_imagemagickxx" != xno \
   && test "x$oln_have_imagemagickxx" = xyes; then
  MAGICKXX_CPPFLAGS=$IMAGEMAGICKXX_CPPFLAGS
  MAGICKXX_LDFLAGS=$IMAGEMAGICKXX_LDFLAGS
elif test "x$oln_have_graphicsmagickxx" = xyes; then
  MAGICKXX_CPPFLAGS=$GRAPHICSMAGICKXX_CPPFLAGS
  MAGICKXX_LDFLAGS=$GRAPHICSMAGICKXX_LDFLAGS
fi
AC_SUBST([MAGICKXX_CPPFLAGS])
AC_SUBST([MAGICKXX_LDFLAGS])
AM_CONDITIONAL([HAVE_MAGICKXX],
               [test "x$oln_have_imagemagickxx" = xyes \
                || test "x$oln_have_graphicsmagickxx" = xyes])

# ---- #
# Qt.  #
# ---- #

AT_WITH_QT([xml], [], [], [:], [oln_have_qt=yes])
if test x$oln_have_qt = xyes; then
  AT_REQUIRE_QT_VERSION([4],
    AC_MSG_WARN([Qt-dependent programs will be disabled.]),
    oln_have_expected_qt_version=yes)
fi
AM_CONDITIONAL([HAVE_QT], [test "x$oln_have_expected_qt_version" = xyes])

# ----------- #
# Tesseract.  #
# ----------- #

OLN_WITH_TESSERACT

# -------------- #
# TIFF library.  #
# -------------- #

OLN_WITH_LIB([TIFF], [tiff.h], [tiff])

# ----------------- #
# Poppler library.  #
# ----------------- #

OLN_WITH_LIB_PKG_CONFIG([poppler-cpp], [poppler/cpp/poppler-version.h], [poppler-cpp],
                        [poppler], [POPPLER])


# --------------------------------- #
# GDCM library (Grassroots DiCom).  #
# --------------------------------- #

OLN_WITH_LIB([GDCM], [gdcm-2.0/gdcmReader.h], [gdcmCommon], [gdcm],
             [GDCM],
             m4_do([-lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF],
                   [ -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8],
                   [ -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib]))


## --------- ##
## Swilena.  ##
## --------- ##

AC_ARG_ENABLE([swilena],
              [AS_HELP_STRING([--enable-swilena],
                              [build Swilena Python bindings])])
AS_IF([test "x$enable_swilena" = xyes],
      [# Ensure dynamic libraries are enabled.
       if test "x$enable_shared" = xno; then
         AC_MSG_ERROR([`--enable-swilena' was given, but dynamic libraries are
disabled.  Try to invoke configure with `--enable-shared'.])
       fi
       # Check for SWIG.
       AC_PROG_SWIG([1.3.35])
       if (eval "$SWIG -version") >/dev/null 2>&1; then :; else
         AC_MSG_ERROR([SWIG 1.3.35 is required for Swilena])
       fi
       # Check for Python.
       AM_PATH_PYTHON([2.4], [],
                      [AC_MSG_ERROR([Python 2.4 is required for Swilena.])])
       adl_CHECK_PYTHON
       # Check for Python headers.
       save_CPPFLAGS=$CPPFLAGS
       CPPFLAGS="$CPPFLAGS -I$PYTHONINC"
       AC_CHECK_HEADERS([Python.h], [],
                        [AC_MSG_ERROR([`Python.h' is required for Swilena.
Try adding `-I <Python include path>' to `CPPFLAGS'.])])
       CPPFLAGS=$save_CPPFLAGS])
AM_CONDITIONAL([ENABLE_SWILENA], [test "x$enable_swilena" = xyes])

AC_CONFIG_FILES([swilena/Makefile
                 swilena/python/Makefile])

AC_CONFIG_FILES([swilena/run], [chmod +x swilena/run])

## -------- ##
## Scribo.  ##
## -------- ##

AC_ARG_ENABLE([scribo],
              [AS_HELP_STRING([--enable-scribo],
                              [enable Scribo])])
AM_CONDITIONAL([ENABLE_SCRIBO], [test "x$enable_scribo" = xyes])

AC_CONFIG_FILES([
  scribo/Makefile
    scribo/demo/Makefile
      scribo/demo/viewer/Makefile
      scribo/demo/xml2doc/Makefile
    scribo/doc/Makefile
    scribo/src/Makefile
      scribo/src/binarization/Makefile
      scribo/src/contest/Makefile
      scribo/src/contest/hdibco-2010/Makefile
      scribo/src/contest/hdlac-2011/Makefile
      scribo/src/contest/DAE-2011/Makefile
      scribo/src/contest/hnla-2013/Makefile
      scribo/src/contest/hbr-2013/Makefile
      scribo/src/debug/Makefile
      scribo/src/filter/Makefile
      scribo/src/misc/Makefile
      scribo/src/postprocessing/Makefile
      scribo/src/preprocessing/Makefile
      scribo/src/primitive/Makefile
        scribo/src/primitive/extract/Makefile
        scribo/src/primitive/find/Makefile
        scribo/src/primitive/group/Makefile
	scribo/src/primitive/remove/Makefile
      scribo/src/table/Makefile
      scribo/src/text/Makefile
      scribo/src/toolchain/Makefile
        scribo/src/toolchain/nepomuk/Makefile
      scribo/src/util/Makefile
])

AC_CONFIG_FILES([scribo/tests/data.hh])

AC_CONFIG_FILES([
  scribo/tests/Makefile
    scribo/tests/binarization/Makefile
    scribo/tests/convert/Makefile
    scribo/tests/core/Makefile
    scribo/tests/estim/Makefile
    scribo/tests/filter/Makefile
    scribo/tests/layout/Makefile
    scribo/tests/preprocessing/Makefile
    scribo/tests/primitive/Makefile
    scribo/tests/primitive/extract/Makefile
    scribo/tests/primitive/link/Makefile
    scribo/tests/table/Makefile
    scribo/tests/text/Makefile
    scribo/tests/toolchain/Makefile
    scribo/tests/toolchain/nepomuk/Makefile
    scribo/tests/unit_test/Makefile
    scribo/tests/util/Makefile
])

# Flags for Scribo
AC_ARG_VAR([SCRIBO_CXXFLAGS], [C++ compiler flags for Scribo binaries])
AC_ARG_VAR([SCRIBO_TESTS_CXXFLAGS], [C++ compiler flags for Scribo tests])
if test "$GXX" = yes || test "$ICPC" = yes; then
  test -z "$SCRIBO_CXXFLAGS" && SCRIBO_CXXFLAGS="-DNDEBUG -O2"
  # FIXME: We might want to use -ggdb when g++ is used.
  test -z "$SCRIBO_TESTS_CXXFLAGS" && SCRIBO_TESTS_CXXFLAGS="-g -O0"
fi

## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h creation.
AC_CONFIG_HEADERS([config.h])

# Ask for the creation of a milena/doc/tool/data.hh, used to access to
# data (images) from the documentation.
AC_CONFIG_FILES([milena/doc/tools/data.hh])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
    doc/Makefile
    milena/Makefile
      milena/doc/Makefile
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
      milena/tests/fun/v2i/Makefile
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
      milena/tests/io/pdf/Makefile
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

## -------------- ##
## Applications.  ##
## -------------- ##

# Ask for the creation of a milena/apps/data.hh, used to access to
# data (images) from apps.
AC_CONFIG_FILES([milena/apps/data.hh])

AC_ARG_ENABLE([apps],
              [AS_HELP_STRING([--enable-apps],
                              [enable applications])])
AM_CONDITIONAL([ENABLE_APPS], [test "x$enable_apps" = "xyes"])

# Ask for the creation of applications' Makefiles.
AC_CONFIG_FILES([
  milena/apps/Makefile
    milena/apps/constrained-connectivity/Makefile
    milena/apps/graph-morpho/Makefile
    milena/apps/mesh-segm-skel/Makefile
    milena/apps/morphers/Makefile
    milena/apps/papers/Makefile
      milena/apps/papers/levillain.09.ismm/Makefile
])

# Configure tests.
# FIXME: Consider using `sed' instead of `configure' to create these
# tests for the sake of speed.
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-max-curv],
                [chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-max-curv])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-segm],
                [chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-segm])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-max-curv-segm],
                [chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-max-curv-segm])
AC_CONFIG_FILES([milena/apps/mesh-segm-skel/test-mesh-complex-skel],
                [chmod +x milena/apps/mesh-segm-skel/test-mesh-complex-skel])

AC_CONFIG_FILES([milena/apps/constrained-connectivity/test-constrained-connectivity],
                [chmod +x milena/apps/constrained-connectivity/test-constrained-connectivity])

# Flags for apps.
AC_ARG_VAR([APPS_CXXFLAGS], [C++ compiler flags for applications])
# We want fast binaries for apps.
if test -z "$APPS_CXXFLAGS"; then
  if test "$GXX" = yes; then
    APPS_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    APPS_CXXFLAGS="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi

## ------- ##
## Tools.  ##
## ------- ##

AC_ARG_ENABLE([tools],
              [AS_HELP_STRING([--enable-tools], [enable tools])])
AM_CONDITIONAL([ENABLE_TOOLS], [test "x$enable_tools" = "xyes"])

# Ask for the creation of tools' Makefiles.
AC_CONFIG_FILES([milena/tools/Makefile])

# Flags for tools.
AC_ARG_VAR([TOOLS_CXXFLAGS], [C++ compiler flags for tools])
# We want fast binaries for tools.
if test -z "$TOOLS_CXXFLAGS"; then
  if test "$GXX" = yes; then
    TOOLS_CXXFLAGS="-O3 -DNDEBUG -ggdb $WARNINGS_CXXFLAGS"
  elif test "$ICPC" = yes; then
    TOOLS_CXXFLAGS="-O3 -DNDEBUG -g $WARNINGS_CXXFLAGS"
  fi
fi


## ----------------- ##
## Verbose display.  ##
## ----------------- ##

# Do not print this information by default, since `configure' outputs
# should be kept short.  Longer explanations are always available in
# config.log.

AC_ARG_ENABLE([verbose],
  [AS_HELP_STRING([--enable-verbose], [enable verbose display])],
  [if test "x$enable_verbose" != xno; then
     AC_MSG_RESULT([
-------------------------------------------------------------------------------
Configuration summary.


Host system type: $host
Build system type: $build

================
| Dependencies |
================


                        Option                  Enabled and available
-------------------------------------------------------------------------------
Boost Tuple             --with-boost=@<:@=DIR@:>@     $oln_have_boost_tuple
CFITSIO                 --with-cfitsio@<:@=DIR@:>@    $oln_have_cfitsio
GDCM                    --with-gdcm@<:@=DIR@:>@       $oln_have_gdcm
Magick++                --with-magickxx         $oln_have_magickxx
Qt                      --with-qt               $oln_have_qt
Tesseract               --with-tesseract@<:@=DIR@:>@  $oln_have_tesseract
TIFF                    --with-tiff@<:@=DIR@:>@       $oln_have_tiff
-------------------------------------------------------------------------------




===========
| Modules |
===========


                        Option                  Enabled
-------------------------------------------------------------------------------
Scribo                  --enable-scribo         $enable_scribo
Swilena                 --enable-swilena        $enable_swilena
-------------------------------------------------------------------------------




=============
| Utilities |
=============


                        Option                  Enabled
-------------------------------------------------------------------------------
Apps                    --enable-apps           $enable_apps
Tools                   --enable-tools          $enable_tools
-------------------------------------------------------------------------------



Options used to compile and link:
  PREFIX          = $PREFIX_DIR
  EXEC-PREFIX     = $EXEC_PREFIX_DIR
  VERSION         = $PACKAGE_VERSION
  CC              = $CC
  CFLAGS          = $CFLAGS
  CPPFLAGS        = $MAGICK_CPPFLAGS
  DEFS            = $DEFS
  LDFLAGS         = $LDFLAGS
  LIBS            = $MAGICK_LIBS
  CXX             = $CXX
  CXXFLAGS        = $CXXFLAGS
  PKG_CONFIG      = $PKG_CONFIG
  QT_PATH         = $QT_PATH
  QMAKE           = $QMAKE
  MOC             = $MOC
  UIC             = $UIC
  RCC             = $RCC
  BOOST_ROOT      = $BOOST_ROOT
  MAGICKXX_CFLAGS = $MAGICKXX_CPPFLAGS
  MAGICKXX_LIBS   = $MAGICKXX_LDFLAGS
  SCRIBO_CXXFLAGS = $SCRIBO_CXXFLAGS
  APPS_CXXFLAGS   = $APPS_CXXFLAGS
  TOOLS_CXXFLAGS  = $TOOLS_CXXFLAGS



*******************************************************************************
Olena is configured as stated above.  Please verify that this configuration
matches your expectations.

Then, type 'make' to build Olena and 'make install' to install it on
your system.
])
fi])


## -------- ##
## Output.  ##
## -------- ##

AC_OUTPUT
