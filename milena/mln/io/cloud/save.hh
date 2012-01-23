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

#ifndef MLN_IO_CLOUD_SAVE_HH
# define MLN_IO_CLOUD_SAVE_HH

/// \file
///
/// Save an image as a cloud of points.

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

      /*! \brief Load a cloud of points
       *
       * \param[in] arr the cloud of points to save.
       * \param[in] filename the destination.
       *
       * \ingroup iocloud
       */
      template <typename P>
      void save(const p_array<P>& arr, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename P>
        inline
        void save_data(const p_array<P>& arr, std::ofstream& file)
	{
	  mln_piter(p_array<P>) p(arr);
	  for_all(p)
          {
	    std::ostringstream sline;
	    algebra::vec<P::dim,float> v = p.to_site().to_vec();
	    sline << v[0];
	    for (unsigned i = 1; i < P::dim; ++i)
	      sline << ' ' << v[i];
	    sline << std::endl;
	    file << sline.str();
	  }
	}

      } // end of namespace mln::io::cloud::internal



      template <typename P>
      void save(const p_array<P>& arr, const std::string& filename)
      {
	trace::entering("mln::io::cloud::save");

	std::ofstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	internal::save_data(arr, file);

	trace::exiting("mln::io::cloud::save");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::cloud

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_CLOUD_SAVE_HH
