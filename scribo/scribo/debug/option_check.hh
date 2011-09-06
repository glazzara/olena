// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEBUG_OPTION_CHECK_HH
# define SCRIBO_DEBUG_OPTION_CHECK_HH

# include <string.h>
# include <cstdlib>
# include <vector>
# include <iostream>
# include <mln/core/contract.hh>

namespace scribo
{

  namespace debug
  {

    bool check_xml_format(const std::vector<const char *>& args);
    bool check_ocr_lang(const std::vector<const char *>& args);
    bool check_sauvola_first_subsampling(const std::vector<const char *>& args);
    bool check_sauvola_split_ntrue(const std::vector<const char *>& args);

#  ifndef MLN_INCLUDE_ONLY

    inline
    bool check_xml_format(const std::vector<const char *>& args)
    {
      static const char *values[] =
	{
	  "page",
	  "page-ext",
	  "full",
	  0
	};
      mln_assertion(args.size() == 1);

      for (int i = 0; values[i]; ++i)
	if (!strcmp(values[i], args[0]))
	  return true;

      std::cerr << "Error: invalid xml format : " << args[0] << std::endl;
      return false;
    }


    inline
    bool check_ocr_lang(const std::vector<const char *>& args)
    {
      static const char *values[] =
	{
	  "eng",
	  "fra",
	  "deu",
	  "ita",
	  "nld",
	  "por",
	  "spa",
	  "vie",
	  0
	};
      mln_assertion(args.size() == 1);

      for (int i = 0; values[i]; ++i)
	if (!strcmp(values[i], args[0]))
	  return true;

      std::cerr << "Error: invalid OCR language : " << args[0] << std::endl;
      return false;
    }

    inline
    bool check_sauvola_first_subsampling(const std::vector<const char *>& args)
    {
      mln_assertion(args.size() == 1);

      int ratio = atoi(args[0]);

      if (ratio == 2 || ratio == 3)
	return true;

      std::cerr << "Error: invalid subsampling ratio : " << args[0] << std::endl;
      return false;
    }


    inline
    bool check_sauvola_split_ntrue(const std::vector<const char *>& args)
    {
      mln_assertion(args.size() == 1);

      int ntrue = atoi(args[0]);

      if (ntrue == 1 || ntrue == 2 || ntrue == 3)
	return true;

      std::cerr << "Error: invalid subsampling ratio : " << args[0] << std::endl;
      return false;
    }


#  endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_DEBUG_OPTION_CHECK_HH
