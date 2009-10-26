// algorithms.cc: this file is part of the Vaucanson project.
// 
// Vaucanson, a generic library for finite state machines.
// 
// Copyright (C) 2004, 2005 The Vaucanson Group.
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
// 
// The Vaucanson Group consists of people listed in the `AUTHORS' file.
//
#include <fstream>
// #include <sstream>
// #include <cstdlib>
// #include <list>

#include "dyn.hh"
#include "dyn_vaucanson_dyn_mirror.hh"
#include "vaucanson_tools_dyn.hh"
#include "vaucanson_tools_dyn_xml.hh"

using namespace dyn::vcsn::io;
using dyn::vcsn::xml::XML;
using namespace dyn::language;
using namespace dyn::vcsn::tools;

// static
// void
// usage(int, char** argv)
// {
  // std::cerr << "Usage: " << std::endl
            // << "\t" << argv[0] << " <algorithm> <args...>" << std::endl;
  // exit(1);
// }

// static
// var
// alphabet()
// {
  // var a = dyn::alphabet('a');
  // a.insert('a');
  // a.insert('b');
  // return a;
// }

// static
// var
// get_exp(std::string s)
// {
  // return new_rat_exp(alphabet(), s);
// }

static
var
get_aut(std::string s)
{
  std::istream* is (s == "-" ? &std::cin : new std::ifstream (s.c_str()));
  if (not is->fail())
    {
      var automaton = dyn::vcsn::tools::xml::load_from_xml(*is);
      // var loader = dyn::automaton_loader(context_automaton, string_out (), XML ());
      // *is >> loader;

      if (s != "-")
        delete is;
      return automaton;
    }
  else
    {
      std::cerr << "FATAL: Could not load automaton." << std::endl;
      exit(1);
    }
}

#if 0
static
void
derived_term_automaton_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

  rat_exp_t	e = get_exp(argv[2]);
  automaton_t	a = new_automaton(alphabet());
  derived_term_automaton(a, e);
  std::cout << automaton_saver(a, string_out (), XML ());
}

static
void
aut_to_exp_command(int argc, char** argv)
{
  if (argc < 3)
    usage(argc, argv);
  else if (argc == 3)
    std::cout << aut_to_exp(get_aut(argv[2]), DMChooser()) << std::endl;
  else
    {
      automaton_t		a = get_aut(argv[2]);
      if (a.states().size() != unsigned (argc - 3))
	{
	  std::cerr << "ERROR: Invalid state list." << std::endl;
	  exit(2);
	}

      std::list<hstate_t>	l;
      for (int i = 3; i < argc; ++i)
	{
	  std::istringstream	is (argv[i]);
	  int			s;

	  is >> s;
	  if (s < 0 or unsigned (s) >= a.states().size())
	    {
	      std::cerr << "ERROR: " << s << " is not a valid state."
			<< std::endl;
	      exit(2);
	    }
	  l.push_back(s);
	}

      std::cout << aut_to_exp(a, ListChooser (l)) << std::endl;
    }
}

static
void
product_command(int argc, char** argv)
{
  if (argc != 4)
    usage(argc, argv);

  std::cout << automaton_saver(product(get_aut(argv[2]), get_aut(argv[3])),
			       string_out (),
			       XML ());
}


static
void
are_isomorphic_command(int argc, char** argv)
{
  if (argc != 4)
    usage(argc, argv);

  std::cout << are_isomorphic(get_aut(argv[2]), get_aut(argv[3])) << std::endl;
}


void
eval_command(int argc, char** argv)
{
  if (argc != 4)
    usage(argc, argv);

  std::cout << eval(realtime(get_aut(argv[2])), std::string (argv[3]))
	    << std::endl;
}

void
is_empty_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

  automaton_t a = coaccessible(accessible(get_aut(argv[2])));

  if (a.states().size() > 0)
    std::cout << false << std::endl;
  else
    std::cout << true << std::endl;
}


void
power_command(int argc, char** argv)
{
  if (argc != 4)
    usage(argc, argv);

  int n = atoi(argv[3]);

  automaton_t a = get_aut(argv[2]);
  automaton_t p(a);

  for (int i = 1; i < n; ++i)
    p = product(p, a);

  std::cout << automaton_saver(p, string_out (), XML ());
}

static
void
display_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

  vcsn::tools::dot_display(get_aut(argv[2]), "A", true);
}

static
void
info_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

  automaton_t a = get_aut(argv[2]);

  std::cout << "States: " << a.states().size() << std::endl;
  std::cout << "Transitions: " << a.edges().size() << std::endl;
  std::cout << "Initial states: " << a.initial().size() << std::endl;
  std::cout << "Final states: " << a.final().size() << std::endl;
}

static
void
expand_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

  std::cout << expand(get_exp(argv[2])) << std::endl;
}

static
void
minimize_command(int argc, char** argv)
{
  if (argc < 3)
    usage(argc, argv);
  else if (argc == 3)
    std::cout <<  automaton_saver(minimization_hopcroft(get_aut(argv[2])),
				  string_out (), XML ())
	      << std::endl;
  else
    {
      std::string	arg (argv[3]);
      automaton_t	a = get_aut(argv[2]);

      if (arg == "-m")
	std::cout <<  automaton_saver(minimization_moore(get_aut(argv[2])),
				      string_out (), XML ())
		  << std::endl;

      else if (arg == "-h")
	std::cout <<  automaton_saver(minimization_hopcroft(get_aut(argv[2])),
				      string_out (), XML ())
		  << std::endl;
    }
}

static
void
concatenate_command(int argc, char** argv)
{
  if (argc != 4)
    usage(argc, argv);

  std::cout << automaton_saver(concatenate(get_aut(argv[2]), get_aut(argv[3])),
			       string_out (), XML ())
	    << std::endl;
}

static
void
sum_command(int argc, char** argv)
{
  if (argc != 4)
    usage(argc, argv);

  std::cout << automaton_saver(sum(get_aut(argv[2]), get_aut(argv[3])),
			       string_out (), XML ())
	    << std::endl;
}

static
void
closure_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

   std::cout << automaton_saver(accessible(closure(get_aut(argv[2]))),
				string_out (), XML ())
	     << std::endl;
}

static
void
determinize_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

   std::cout << automaton_saver(determinize(realtime(get_aut(argv[2]))),
				string_out (), XML ())
	     << std::endl;
}

static
void
quotient_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

   std::cout << automaton_saver(quotient(realtime(get_aut(argv[2]))),
				string_out (), XML ())
	     << std::endl;
}

static
void
standardize_command(int argc, char** argv)
{
  if (argc != 3)
    usage(argc, argv);

  automaton_t a = get_aut(argv[2]);
  standardize(a);

  std::cout << automaton_saver(a, string_out (), XML ()) << std::endl;
}
#endif

#define ONE_ARG_COMMAND(GetArg, Algo) one_arg_command_ ## Algo ## _ ## GetArg

#define DEFINE_ONE_ARG_COMMAND(GetArg, Algo)		\
static							\
void							\
ONE_ARG_COMMAND(GetArg, Algo)(int argc, char** argv)	\
{							\
  if (argc != 3)					\
    usage(argc, argv);					\
  std::cout << dyn::automaton_saver(dyn::Algo(GetArg(argv[2])),	\
			       string_out (),		\
			       XML ());			\
}

// DEFINE_ONE_ARG_COMMAND(get_exp, standard_of)
// DEFINE_ONE_ARG_COMMAND(get_exp, thompson_of)
// DEFINE_ONE_ARG_COMMAND(get_aut, trim)
// DEFINE_ONE_ARG_COMMAND(get_aut, transpose)
// DEFINE_ONE_ARG_COMMAND(get_aut, realtime)
// DEFINE_ONE_ARG_COMMAND(get_aut, coaccessible)
// DEFINE_ONE_ARG_COMMAND(get_aut, accessible)
// DEFINE_ONE_ARG_COMMAND(get_aut, normalize)

#undef DEFINE_ONE_ARG_COMMAND

// const struct
// {
  // char*	name;
  // void	(*command)(int, char**);
// }
// command_map[] =
  // {
    // { "expand",			expand_command				},
    // { "standard_of",		ONE_ARG_COMMAND(get_exp, standard_of)	},
    // { "thompson_of",		ONE_ARG_COMMAND(get_exp, thompson_of)	},
    // { "derived_terms",		derived_term_automaton_command		},
    // { "aut_to_exp",		aut_to_exp_command			},
    // { "quotient",		quotient_command			},
    // { "product",		product_command				},
    // { "closure",		closure_command				},
    // { "is-empty",		is_empty_command			},
    // { "are-isomorphic",		are_isomorphic_command			},
    // { "closure",		closure_command				},
    // { "determinize",		determinize_command			},
    // { "minimize",		minimize_command			},
    // { "trim",			ONE_ARG_COMMAND(get_aut, trim)		},
    // { "transpose",		ONE_ARG_COMMAND(get_aut, transpose)	},
    // { "accessible",		ONE_ARG_COMMAND(get_aut, coaccessible)	},
    // { "coaccessible",		ONE_ARG_COMMAND(get_aut, accessible)	},
    // { "sum",			sum_command				},
    // { "concatenate",		concatenate_command			},
    // { "realtime",		ONE_ARG_COMMAND(get_aut, realtime)	},
    // { "eval",			eval_command				},
    // { "power",			power_command				},
    // { "normalize",		ONE_ARG_COMMAND(get_aut, normalize)	},
    // { "standardize",		standardize_command			},
    // { "display",		display_command				},
    // { "info",			info_command				},
    // { 0,			0					}
  // };

#undef ONE_ARG_COMMAND

int
main(int argc, char** argv)
{
  argc = 0;
  // if (argc < 2)
    // usage(argc, argv);

  dyn::include_dir(DYNDIR);
  dyn::include_dir("../../../include");
  dyn::include_dir(".");
  dyn::include("vaucanson/xml/XML.hh");

  std::cout << "context" << std::endl;
  // algebraic_context context(argv[1], argv[2]);

  // dyn::proc insert("insert", "method");

  // var context_automaton = context.mk_automaton();

  var automaton = get_aut(argv[4]);

  std::cout << dyn::vcsn::automaton_saver(automaton, string_out(), XML());

  std::string algorithms("vaucanson/algorithms/");
  dyn::fun algo(argv[3], algorithms + argv[3] + ".hh");

  std::cout << dyn::vcsn::automaton_saver(algo(automaton), string_out(), XML());

  // std::string cmd (argv[1]);
  // int i;

  // for (i = 0; command_map[i].name != 0; ++i)
    // if (cmd == command_map[i].name)
      // {
        // command_map[i].command(argc, argv);
        // break ;
      // }
  // if (command_map[i].name == 0)
    // {
      // std::cerr << "Available algorithms:" << std::endl;
      // std::cerr << " * is-empty"  << std::endl;
      // std::cerr << " * are-isomorphic"  << std::endl;
      // std::cerr << " * accessible"  << std::endl;
      // std::cerr << " * coaccessible"  << std::endl;
      // std::cerr << " * realtime"  << std::endl;
      // std::cerr << " * trim"  << std::endl;
      // std::cerr << " * transpose"  << std::endl;
      // std::cerr << " * aut_to_exp"  << std::endl;
      // std::cerr << " * expand"  << std::endl;
      // std::cerr << " * standard_of"  << std::endl;
      // std::cerr << " * thompson_of"  << std::endl;
      // std::cerr << " * derived_terms"  << std::endl;
      // std::cerr << " * product"  << std::endl;
      // std::cerr << " * power"  << std::endl;
      // std::cerr << " * determinize"  << std::endl;
      // std::cerr << " * minimize"  << std::endl;
      // std::cerr << " * quotient"  << std::endl;
      // std::cerr << " * closure"  << std::endl;
      // std::cerr << " * eval"  << std::endl;
      // std::cerr << " * display"  << std::endl;
      // std::cerr << " * info"  << std::endl;
      // exit(1);
    // }

  std::cout << "Exiting, segv in approch..." << std::endl;
  
}


