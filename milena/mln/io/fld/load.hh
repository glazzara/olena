// Copyright (C) 2008, 2009, 2010, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_IO_FLD_LOAD_HH
# define MLN_IO_FLD_LOAD_HH

/// \file
///
/// \brief Load an image from an AVS field file.
///
/// \note The current loader does not follow the whole specifications
/// of the format. Actually, it has the following restrictions:
/// - the dimension of the field and the space must be the same.
/// - the number of dimension is limited to 1D, 2D and 3D.
/// - the data format must be native (float, integer...) (XDR extension is not supported)
/// - the field must uniform (regular grid).
/// - dim1, dim2... dimn are parsed but ignored.
/// - min_ext and max_ext (pmin and pmax of the bbox) are not computed and are compulsory.
/// - label and unit keyword are not supported.
/// - external data source ('coord', and 'variable') is not supported.
///
/// FIXME: pnm::load uses special implementation if sizeof(int_u8) != 1 ?? what ??

# include <fstream>
# include <iostream>

# include <mln/core/concept/image.hh>
# include <mln/io/fld/header.hh>
# include <mln/io/fld/load_header.hh>
# include <mln/io/fld/max_components.hh>

# include <mln/algebra/vec.hh>
# include <mln/value/rgb.hh>
# include <mln/value/int_u8.hh>

# include <mln/geom/nsites.hh>

# include <mln/convert/from_to.hh>

namespace mln
{

  namespace io
  {

    namespace fld
    {

      /*! \brief Load an image from an AVS field file.
       *
       * \param[in,out] ima_ The image to load.
       * \param[in] filename The path to the AVS file.
       *
       * \ingroup iofld
       */
      template <typename I>
      inline
      void
      load(Image<I>& ima_, const char* filename);

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	inline
	void
	abort_load(const char* msg, const char* filename)
	{
	  std::cerr << "Error: file '" << filename << "'"
		    << "cannot be loaded." << std::endl
		    << "Error description: " << msg << std::endl;
	  abort();
	}

	// Read a Milena rgb value (sizeof(int_u8) != 1).
	template <unsigned int n>
	inline
	void read_value(std::ifstream& file, value::rgb<n>& v)
	{
	  typedef typename value::int_u<n>::enc E;

	  E c;
	  file.read((char*)(&c), sizeof(E));
	  v.red() = c;
	  file.read((char*)(&c), sizeof(E));
	  v.green() = c;
	  file.read((char*)(&c), sizeof(E));
	  v.blue() = c;
	}

	// Read a Milena scalar value (sizeof(int_u8) != 1).
	template <class V>
	inline
	void read_value(std::ifstream& file, value::Scalar<V>& v)
	{
	  typedef typename V::enc E;

	  E c;
	  file.read((char*)(&c), sizeof(E));
	  exact(v) = c;
	}

	// Read a builtin scalar value.
	template <typename V>
	inline
	void read_value(std::ifstream& file, V& v)
	{
	  V c;
	  file.read((char*)(&c), sizeof(V));
	  v = c;
	}

	// used when (sizeof(int_u8) != 1)
	template <typename I>
	inline
	void load_raw_uncontiguous(std::ifstream& file, I& ima)
	{
	  mln_piter(I) p(ima.domain());
	    read_value(file, ima(p));
	}

	// used in g++ > 2.95
	template <typename I>
	inline
	void load_raw_contiguous(std::ifstream& file, I& ima)
	{
	  mln_site(I) pmin = ima.domain().pmin();
	  mln_site(I) pmax = ima.domain().pmax();

	  typedef mln_site(I) P;
	  enum { dim = P::dim };

	  // The first array index varies most quickly (FORTRAN-style).
	  typedef mln_value(I) V;


	  std::size_t len = pmax[dim - 1] - pmin[dim - 1] + 1;
	  std::size_t n = len * sizeof(V);

	  P p = pmin;
	  if (dim == 1)
	    {
	      file.read((char*)(&ima(p)), n);
	      return;
	    }

	  while (true)
	    {
	      file.read((char*)(&ima(p)), n);
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
	void load_raw(std::ifstream& file, I& ima)
	{
	  if (sizeof(value::int_u8) == 1)
	    load_raw_contiguous(file, ima);
	  else
	    load_raw_uncontiguous(file, ima);
	}

      } // end of mln::io::internal

      template <typename I>
      inline
      void
      load(Image<I>& ima_, const char* filename)
      {
	mln_trace("mln::io::fld::load");

	std::ifstream file(filename);
	if (! file)
	  internal::abort_load("Fail to open the file.", filename);

	typedef mln_value(I) V;
	typedef mln_site(I)  P;

	I& ima		= exact(ima_);
	fld_header hder	= fld::read_header(file);
	int nspace	= P::dim;
	int veclen	= mln_dim(V);

	if (nspace != hder.nspace)
	  internal::abort_load("The dimension of the input does not match the one from the file.", filename);
	if (nspace > 3)
	  internal::abort_load("The loader does not handle image dimension greater than three.", filename);
	if (veclen != hder.veclen)
	  internal::abort_load("The dimension of the value does not match the one from the file.", filename);
	if (max_component(V ()) != max_component(hder.data))
	  internal::abort_load("The data type of the input mismatches the one from the file.", filename);

	box<mln_site(I)> bbox;
	for (int i = 0; i < hder.ndim; ++i)
	  {
	    convert::from_to(hder.min_ext[i], bbox.pmin()[i]);
	    convert::from_to(hder.max_ext[i], bbox.pmax()[i]);
	  }

	ima.init_(bbox);
	internal::load_raw(file, ima);

	file.close();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::fld

  } // end of namespace mln::io

} // end of namespace mln

#endif // !MLN_IO_FLD_LOAD_HH
