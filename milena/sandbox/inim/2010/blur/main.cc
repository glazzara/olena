#include <string>
#include <iostream>
#include "cuttor.hh"

void usage (const std::string& name)
{
  std::cout << "Usage: " << name
            << " {-p|-v} FILE [MORE_FILES...]" << std::endl;
  exit (1);
}

int
main (int argc,
      char** argv)
{
  if (argc <= 2)
    usage(argv[0]);

  bool use_vect = false;
  if (argv[1] == std::string("-p"))
    use_vect = false;
  else if (argv[1] == std::string("-v"))
    use_vect = true;
  else
    usage(argv[0]);

  for (int i = 2; i < argc; i++)
  {
    std::cout << ">>> Starting Treatment on " << argv[i] << std::endl;

    std::string tmp (argv[i]);
    Cuttor<mln::value::int_u8> cut (tmp);

    cut.start (use_vect);

    std::cout << "<<< Treatment achieved on " << argv[i] << std::endl
              << "See corresponding subdirectories for results." << std::endl;
  }
}
