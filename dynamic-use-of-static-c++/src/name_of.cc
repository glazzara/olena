#ifndef DYN_NAME_OF_CC
# define DYN_NAME_OF_CC

# include "name_of.hh"

# ifndef NO_COMPILER_DEMANGLING

extern "C" {
#  include "demangle.h"
}

#  define DEMANGLE_OPTIONS DMGL_VERBOSE | DMGL_TYPES | DMGL_ANSI | DMGL_PARAMS

std::string demangle(const char* name)
{
  std::string result(cplus_demangle(name, DEMANGLE_OPTIONS));
  size_t pos;
  if ((pos = result.find("char_traints")) != std::string::npos)
    result.replace(pos, 12, "char_traits ");
  return result;
}

# endif

#endif // ! DYN_NAME_OF_CC
