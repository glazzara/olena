// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_DEBUG_OPTION_PARSER_HH
# define SCRIBO_DEBUG_OPTION_PARSER_HH

# include <map>
# include <scribo/debug/option_check.hh>

namespace scribo
{

  namespace debug
  {

    struct arg_data
    {
      const char* name;
      const char* desc;
    };


    struct toggle_data
    {
      const char* name;
      const char* desc;
      bool enabled;
    };


    struct opt_data
    {
      const char *name;
      const char *desc;
      const char *format;
      bool (*fcheck)(const std::vector<const char *>&);
      int n_args;
      const char *by_default;
    };


    class option_parser
    {
    public:
      option_parser(const arg_data arg_desc[],
		    const toggle_data toggle_desc[],
		    const opt_data opt_desc[]);

      bool parse(int argc, char *argv[]);

      void print_help() const;

      bool is_enabled(const char *toggle_name) const;
      bool is_set(const char *opt_name) const;

      std::vector<const char *> opt_values(const char *opt_name) const;
      std::string opt_value(const char *opt_name) const;

      const char* arg(const char *) const;

      void set_verbose_enabled(bool b);


    private:
      typedef std::map<std::string, bool> toggles_t;
      typedef std::vector<const char *> vector_t;
      typedef std::map<std::string, vector_t> opts_t;
      typedef std::map<std::string, const char *> args_t;

      bool validate_toggle(const char *name, const char *fullname) const;
      bool validate_option(const char *name, const char *fullname, int& opt_id) const;

      args_t args_;
      toggles_t toggles_;
      opts_t opts_;
      unsigned required_argc_;
      const char *name_;

      const arg_data *arg_desc_;
      const toggle_data *toggle_desc_;
      const opt_data *opt_desc_;

      bool verbose;
    };


# ifndef MLN_INCLUDE_ONLY

    inline
    option_parser::option_parser(const arg_data arg_desc[],
				 const toggle_data toggle_desc[],
				 const opt_data opt_desc[])
      : arg_desc_(arg_desc), toggle_desc_(toggle_desc), opt_desc_(opt_desc),
	verbose(false)
    {
      for (required_argc_ = 0; arg_desc_[required_argc_].name; ++required_argc_)
	; // None

      // Setting up toggles.
      for (int i = 0; toggle_desc_[i].name; ++i)
	toggles_[toggle_desc_[i].name] = toggle_desc_[i].enabled;

      // Settings up default option values (if available)
      for (int i = 0; opt_desc_[i].name; ++i)
	if (opt_desc_[i].by_default)
	  opts_[opt_desc_[i].name].push_back(opt_desc_[i].by_default);
    }


    inline
    bool
    option_parser::validate_toggle(const char *name, const char *fullname) const
    {
      if (toggles_.find(name) == toggles_.end())
      {
	std::cerr << "Error: invalid toggle " << fullname << std::endl;
	return false;
      }

      return true;
    }


    inline
    bool
    option_parser::validate_option(const char *name, const char *fullname,
				   int& opt_id) const
    {
      // Get option id.
      opt_id = 0;
      for (; opt_desc_[opt_id].name && strcmp(opt_desc_[opt_id].name, name); ++opt_id)
	;

      // Does this option really exist?
      if (!opt_desc_[opt_id].name)
      {
	std::cerr << "Error: invalid option " << fullname << std::endl;
	return false;
      }

      return true;
    }


    inline
    bool
    option_parser::parse(int argc, char *argv[])
    {
      bool no_error = true;
      unsigned nrequired_argc = 0;

      // Get application name.
      name_ = argv[0];
      char **arg_end = argv + argc;
      ++argv;

      while (argv < arg_end)
      {
	switch(argv[0][0])
	{
	  case '-':
	    switch(argv[0][1])
	    {
	      // Toggle or option.
	      case '-':
		if (! strncmp(argv[0], "--enable-", 9))
		{
		  if (!validate_toggle(argv[0] + 9, argv[0]))
		  {
		    no_error = false;
		    break;
		  }

		  toggles_[argv[0] + 9] = true; // enable
		  if (verbose)
		    std::cout << "> Toggle " << argv[0] + 9
			      << " enabled" << std::endl;
		}
		else if (! strncmp(argv[0], "--disable-", 10))
		{
		  if (!validate_toggle(argv[0] + 10, argv[0]))
		  {
		    no_error = false;
		    break;
		  }

		  toggles_[argv[0] + 10] = false; // disable
		  if (verbose)
		    std::cout << "> Toggle " << argv[0] + 10
			      << " disabled" << std::endl;
		}
		else // Option
		{
		  char *opt = argv[0] + 2;
		  opts_[opt] = vector_t();

		  // Checking if it is --help
		  if (!strcmp(argv[0], "--help"))
		  {
		    print_help();
		    exit(0);
		  }

		  int opt_id = 0;
		  if (!validate_option(opt, argv[0], opt_id))
		  {
		    no_error = false;
		    break;
		  }

		  if (verbose)
		    std::cout << " * Looking for arguments for option "
			      << opt << std::endl;

		  // Clear potential previous values
		  opts_[opt].clear();

		  // Switch to option args.
		  char **arg_lookup = argv + opt_desc_[opt_id].n_args;
		  ++argv;

		  // Retrieving the expected arguments
		  while (argv < arg_end && argv <= arg_lookup)
		  {
		    opts_[opt].push_back(argv[0]);
		    ++argv;
		  }

		  // Checking if we found all the option arguments.
		  if (argv <= arg_lookup)
		  {
		    std::cerr << "Error: missing arguments for option "
			      << opt << std::endl;
		    no_error = false;
		    break;
		  }

		  --argv; // Don't move now to the next program argument,

		  // Checking option parameter if possible.
		  if (opt_desc_[opt_id].fcheck
		      && !opt_desc_[opt_id].fcheck(opts_[opt]))
		  {
		    no_error = false;
		    break;
		  }

		  // it will be done here [1].

		  if (verbose)
		  {
		    std::cout << "> Set option " << opt << " to ";
		    for (vector_t::const_iterator it = opts_[opt].begin();
			 it != opts_[opt].end(); ++it)
		      std::cout << *it << " ";
		    std::cout << std::endl;
		  }
		}
		break;

		// Error
	      default:
		std::cerr << "Error: invalid option " << argv[0] << std::endl;
		no_error = false;
	    }
	    ++argv; // Moving to the next program argument. [1]
	    break;

	    // I/O parameters
	  default:
	    if (nrequired_argc == required_argc_)
	    {
	      std::cerr << "Error: wrong number of arguments! \""
			<< argv[0] << "\" was not expected!" << std::endl;
	      no_error = false;
	    }
	    else
	    {
	      args_[arg_desc_[nrequired_argc].name] = argv[0];
	      ++nrequired_argc;
	    }
	    ++argv;
	    break;
	}
      }

      if (nrequired_argc != required_argc_)
      {
	if (nrequired_argc)
	  std::cerr << "Error: wrong number of arguments!" << std::endl;
	no_error = false;
      }

      if (! no_error)
      {
	std::cout << std::endl;
	std::cout << "--------------" << std::endl;
	print_help();
      }

      return no_error;
    }

    std::string add_space(const char* name)
    {
      std::string str;

      int len = 20 - strlen(name);
      for (int i = 0; i < len; ++i)
	str += " ";
      str += " ";

      return str;
    }

    inline
    void
    option_parser::print_help() const
    {
      std::cout << " Usage: " << name_;
      for (int i = 0; arg_desc_[i].name; ++i)
	std::cout << " " << arg_desc_[i].name;

      if (opt_desc_[0].name)
	std::cout << " [OPTIONS]";

      if (toggle_desc_[0].name)
	std::cout << " [TOGGLES]" << std::endl;

      std::cout << std::endl << std::endl;

      std::cout << "  List of the mandatory arguments:"
		<< std::endl << std::endl;

      for (int i = 0; arg_desc_[i].name; ++i)
	std::cout << "  " << arg_desc_[i].name << add_space(arg_desc_[i].name)
		  << arg_desc_[i].desc << std::endl;

      std::cout << std::endl << std::endl;

      // Display options if needed.
      if (opt_desc_[0].name)
      {
	std::cout << "  [OPTIONS] can be replaced by one or several "
		  << "following options :" << std::endl << std::endl;

	for (int i = 0; opt_desc_[i].name; ++i)
	{
	  std::cout <<  "  " << "--" << opt_desc_[i].name << " ";

	  if (opt_desc_[i].format)
	    std::cout << opt_desc_[i].format << std::endl;

	  std::cout << "\t" << opt_desc_[i].desc << std::endl;

	  if (opt_desc_[i].by_default)
	    std::cout << "\tDefault value: " << opt_desc_[i].by_default << std::endl;

	  std::cout << std::endl;
	}

	std::cout << "  --help" << std::endl;
	std::cout << "\t Display this help." << std::endl << std::endl;

	std::cout << std::endl << std::endl;
      }

      // Display toggles if needed.
      if (toggle_desc_[0].name)
      {
	std::cout << "  [TOGGLES] can be replaced by one or several "
		  << "following toggle names prefixed by --enable- or --disable- :"
		  << std::endl << std::endl;

	for (int i = 0; toggle_desc_[i].name; ++i)
	  std::cout <<  "  " << toggle_desc_[i].name << add_space(toggle_desc_[i].name)
		    << toggle_desc_[i].desc << std::endl;

	std::cout << std::endl << std::endl;
      }

      std::cout << "------------" << std::endl;
      std::cout << "EPITA/LRDE - Scribo 2008-2011" << std::endl;
    }


    inline
    bool
    option_parser::is_enabled(const char *toggle_name) const
    {
      toggles_t::const_iterator it = toggles_.find(toggle_name);
      return it->second;
    }


    inline
    bool
    option_parser::is_set(const char *opt_name) const
    {
      return opts_.find(opt_name) != opts_.end();
    }


    inline
    std::vector<const char*>
    option_parser::opt_values(const char *opt_name) const
    {
      opts_t::const_iterator it = opts_.find(opt_name);
      mln_assertion(it != opts_.end());
      return it->second;
    }


    inline
    std::string
    option_parser::opt_value(const char *opt_name) const
    {
      opts_t::const_iterator it = opts_.find(opt_name);
      mln_assertion(it != opts_.end());
      return it->second[0];
    }


    inline
    const char*
    option_parser::arg(const char* name) const
    {
      mln_assertion(args_.find(name) != args_.end());
      return args_.find(name)->second;
    }


    inline
    void
    option_parser::set_verbose_enabled(bool b)
    {
      verbose = b;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_DEBUG_OPTION_PARSER_HH
