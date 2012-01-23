// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_IO_FLD_SAVE_HH
# define MLN_IO_FLD_SAVE_HH

/// \file
/// \brief Save an image to AVS field file format.
///
/// \todo Handle not high speed images.

# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/gpoint.hh>
# include <mln/io/fld/header.hh>
# include <mln/io/fld/write_header.hh>
# include <mln/io/fld/max_components.hh>

# include <mln/algebra/vec.hh>

# include <mln/geom/bbox.hh>

namespace mln
{

  namespace io
  {

    namespace fld
    {

      /*! \brief Save an image to AVS field file format.
       *
       * \ingroup iofld
       */
      template <typename I>
      void save(const Image<I>& ima_, const char* filename);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {
	template <typename I>
	inline
	void save_data_contiguous(std::ofstream& file, const I& ima)
	{
	  typedef mln_site(I) P;
	  typedef mln_value(I) V;
	  enum { dim = P::dim };

	  P pmin = ima.domain().pmin();
	  P pmax = ima.domain().pmax();

	  std::size_t len = pmax[dim - 1] - pmin[dim - 1] + 1;
	  std::size_t n = len * sizeof(V);
	  P p = pmin;
	  if (dim == 1)
	    {
	      file.write((char*)(&ima(p)), n);
	      return;
	    }

	  while (true)
	    {
	      file.write((char*)(&ima(p)), n);
	      ++p[dim - 2];

	      for (int i = dim - 2; p[i] > pmax[i]; --i)
		{
		  if (i == 0)
		    return;
		  p[i] = pmin[i];
		  ++p[i - 1];
		}
	    }
	}

	template <typename I>
	inline
	fld::fld_header make_header(const I& ima)
	{
	  fld_header hdr;
	  typedef mln_site(I) P;
	  typedef mln_value(I) V;
	  enum { dim = P::dim };

	  hdr.ndim = dim;
	  hdr.nspace = dim;
	  hdr.veclen = mln_dim(V);
	  hdr.dim = new int[dim];
	  hdr.min_ext = new float[dim];
	  hdr.max_ext = new float[dim];

	  box<P> bbox = geom::bbox(ima);
	  P pmin = bbox.pmin();
	  P pmax = bbox.pmax();

	  for (unsigned i = 0; i < dim; i++)
	    {
	      hdr.dim[i] = pmax[i] - pmin[i] + 1;
	      hdr.min_ext[i] = pmin[i];
	      hdr.max_ext[i] = pmax[i];
	    }

	  unsigned max_c = max_component(V ());
	  if (max_c == max_component(data_type::BYTE))
	    hdr.data = data_type::BYTE;
	  else if (max_c == max_component(data_type::SHORT))
	    hdr.data = data_type::SHORT;
	  else if (max_c == max_component(data_type::INTEGER))
	    hdr.data = data_type::INTEGER;
	  else if (max_c == max_component(data_type::FLOAT))
	    hdr.data = data_type::FLOAT;
	  else if (max_c == max_component(data_type::DOUBLE))
	    hdr.data = data_type::DOUBLE;
	  else
	    hdr.data = data_type::UNKNOWN;

	  hdr.field = field_type::UNIFORM;

	  return hdr;
	}

      } // end of namespace mln::io::fld::internal

      template <typename I>
      void save(const Image<I>& ima_, const char* filename)
      {
	trace::entering("mln::io::fld::save");
	// For the moment, just the fast version.
	mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	std::ofstream file(filename);
	fld_header hdr = internal::make_header(ima);

	write_header(file, hdr);
	internal::save_data_contiguous(file, ima);

	file.close();
	trace::exiting("mln::io::fld::save");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::fld

  } // end of namespace mln::io

} // end of namespace mln
#endif // !MLN_IO_FLD_SAVE_HH
