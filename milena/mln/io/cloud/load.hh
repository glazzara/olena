// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_CLOUD_LOAD_HH
# define MLN_IO_CLOUD_LOAD_HH

/// \file
///
/// Load a cloud of points.
/// A cloud file must contain one point per line.
/// For a list of 3d points, a line would looks like the following:
///
/// 35 4 5
///
///
/// \todo Support 1 and 2d images.
/// \todo Make it generic?

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_array.hh>

namespace mln
{

  namespace io
  {

    namespace cloud
    {

      /*! \brief Load a cloud of points.
       *
       * \param[in,out] arr the site set where to load the data.
       * \param[in] filename file to load.
       *
       * \ingroup iocloud
      */
      template <typename P>
      void load(p_array<P>& arr, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename P>
        inline
        void load_data(p_array<P>& arr, std::ifstream& file)
	{
	  std::string line;
	  while (std::getline(file, line))
          {
	    algebra::vec<P::dim,float> v;
	    std::istringstream sline(line);
	    sline >> v[0] >> v[1] >> v[2];
	    arr.append(v);
	  }
	}

      } // end of namespace mln::io::cloud::internal



      template <typename P>
      void load(p_array<P>& arr, const std::string& filename)
      {
	trace::entering("mln::io::cloud::load");

	/// FIXME: do we really want this?
	mln_precondition(P::dim == 3u);

	std::ifstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	internal::load_data(arr, file);

	trace::exiting("mln::io::cloud::load");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::cloud

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_CLOUD_LOAD_HH
