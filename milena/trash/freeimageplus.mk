## To be added to Makefile.am when support for FreeImagePlus is restored.

# Tests depending on the FreeImagePlus library.
if HAVE_FREEIMAGEPLUS
  check_PROGRAMS += fi_adaptor
  fi_adaptor_SOURCES = fi_adaptor.cc
  fi_adaptor_CPPFLAGS = $(AM_CPPFLAGS) $(FREEIMAGEPLUS_CPPFLAGS)
  fi_adaptor_LDFLAGS = $(AM_LDFLAGS) $(FREEIMAGEPLUS_LDFLAGS)
endif HAVE_FREEIMAGEPLUS
