// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_IO_PFM_LOAD_HH
# define MLN_IO_PFM_LOAD_HH

/// \file
///
/// Define a function which loads an image of kind pfm with
/// given path.

# include <iostream>
# include <fstream>

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>


namespace mln
{

  namespace io
  {

    namespace pfm
    {

      /*! \brief Load a pfm image in a Milena image.
       *
       * \param[out] ima A reference to the image2d<float> which will receive
       * data.
       * \param[in] filename The source.
       *
       * \ingroup iopfm
       */
      void load(image2d<float>& ima,
		const std::string& filename);

      /*! \brief Load a pfm image in a image2d<float>.
       *
       * \param[in] filename The image source.
       *
       * \return An image2d<float> which contains loaded data.
       *
       * \ingroup iopfm
       */
      image2d<float> load(const std::string& filename);

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	inline
	void abort()
	{
	  std::cerr << " aborting." << std::endl;
	  exit(0);
	}

	inline
	bool read_pfm_header(std::istream& file,
			     int& nrows, int& ncols,
			     bool test = false)
	{
	  std::string tag;

	  // get size
	  file >> nrows;
	  file >> ncols;

	  if (file.get() != '\n')
	    goto err;

	  // check tag == float
	  file >> tag;
	  if (tag != "float")
	    goto err;

	  if (file.get() != '\n')
	    goto err;

	  return true;

	err:
	  if (! test)
	    {
	      std::cerr << "error: badly formed header!"
			<< std::endl;
	      abort();
	    }
	  return false;
	}


	/// load_raw_2d.
	template <typename I>
	inline
	void load_raw_2d(std::ifstream& file, I& ima)
	{
	  point2d p = point2d(0, 0);

	  const mln_deduce(I, site, coord)
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);

	  unsigned int
	    ncols = geom::ncols(ima);

	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    file.read((char*)(&(ima(p))),
		      sizeof(float) * ncols);

	}


      } // end of namespace mln::io::internal

      inline
      image2d<float> load(const std::string& filename)
      {
	trace::entering("mln::io::pfm::load");

	std::ifstream file(filename.c_str());
	if (! file)
	  {
	    std::cerr << "error: file '" << filename
		      << "' not found!";
	    abort();
	  }
	int nrows, ncols;
	internal::read_pfm_header(file, nrows, ncols);

	image2d<float> ima(nrows, ncols);
	internal::load_raw_2d(file, ima);

	trace::exiting("mln::io::pfm::load");

	return ima;
      }


      inline
      void load(image2d<float>& ima,
		const std::string& filename)
      {
	ima = load(filename);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pfm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PFM_LOAD_HH
