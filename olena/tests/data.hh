// FIXME: Should be compiled in libolncheck, as in Olena 0.10.
// (This is temporary.)

#ifndef OLN_TESTS_DATA_HH
# define OLN_TESTS_DATA_HH

#include <iostream>

std::string
rdata(const std::string& file)
{
  return OLN_IMG_DIR "/" + file;
}

#endif // ! OLN_TESTS_DATA_HH
