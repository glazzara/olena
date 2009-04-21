#include <string>
#include <iostream>
#include "cuttor.hh"

int
main (int argc,
      char** argv)
{
  for (int i = 1; i < argc; i++)
  {
    std::cout << ">>> Starting Treatment on " << argv[i] << std::endl;

    std::string tmp (argv[i]);
    Cuttor<mln::value::int_u8> cut (tmp);

    cut.start ();

    std::cout << "<<< Treatment achieved on " << argv[i] << std::endl
              << "See corresponding subdirectories for results." << std::endl;
  }
}
