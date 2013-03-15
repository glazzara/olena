// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_TEXT_BOXES_SAVE_HH
# define SCRIBO_IO_TEXT_BOXES_SAVE_HH

/// \file
///
/// \brief Save text line information as XML.


# include <fstream>
# include <sstream>

# include <scribo/core/line_set.hh>

namespace scribo
{

  namespace io
  {

    namespace text_boxes
    {

      template <typename L>
      void
      save(const line_set<L>& lines,
	   const std::string& output_name,
	   const mln_site(L)& crop_shift = literal::origin);


# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      void
      save(const line_set<L>& lines,
	   const std::string& output_name,
	   const mln_site(L)& crop_shift)
      {
	mln_trace("scribo::io::text_boxes:save");
	mln_precondition(lines.is_valid());

	std::ofstream file(output_name.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << output_name << "'!";
	  abort();
	}

	for_all_lines(l, lines)
	{
	  if (! lines(l).is_valid() || lines(l).tag() != line::None || lines(l).type() != line::Text || lines(l).text().empty())
	    continue;

	  file << lines(l).bbox().pmin().row() + crop_shift.row()<< " "
	       << lines(l).bbox().pmin().col() + crop_shift.col()<< " "
	       << lines(l).bbox().pmax().row() + crop_shift.row() << " "
	       << lines(l).bbox().pmax().col() + crop_shift.col()<< " "
	       << lines(l).text()              << std::endl;
	}

	file.close();


      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::io::text_boxes

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // ! SCRIBO_IO_TEXT_BOXES_SAVE_HH
