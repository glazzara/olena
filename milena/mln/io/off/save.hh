// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_IO_OFF_SAVE_HH
# define MLN_IO_OFF_SAVE_HH

/// \file
/// Input saving function for OFF files.
///
/// \see http://shape.cs.princeton.edu/benchmark/documentation/off_format.html
/// \see https://people.scs.fsu.edu/~burkardt/html/off_format.html

# include <cstdlib>

# include <iostream>
# include <fstream>
# include <sstream>

# include <string>

# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>


namespace mln
{

  namespace io
  {

    namespace off
    {

      /*! \brief Save a (binary) OFF image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the image.

	  The image is said binary since data represent only the
	  existence of faces.

	  \ingroup iooff
      */
      void save(const bin_2complex_image3df& ima,
		const std::string& filename);

      /*! \brief Save an 8-bit grey-level OFF image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the image.

	  Only data is attached to 2-faces is saved; the OFF file
	  cannot store data attached to faces of other dimensions.

	  \ingroup iooff
      */
      void save(const int_u8_2complex_image3df& ima,
		const std::string& filename);

      /*! \brief Save a floating-point value grey-level OFF image into
	  a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the image.

	  Only data is attached to 2-faces is saved; the OFF file
	  cannot store data attached to faces of other dimensions.

	  \ingroup iooff
      */
      void save(const float_2complex_image3df& ima,
		const std::string& filename);

      /*! \brief Save a 3x8-bit RGB (color) OFF image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the image.

	  Only data is attached to 2-faces is saved; the OFF file
	  cannot store data attached to faces of other dimensions.

	  \ingroup iooff
      */
      void save(const rgb8_2complex_image3df& ima,
		const std::string& filename);


      namespace internal
      {

	template <typename I, typename E>
	struct off_saver : public Object<E>
	{
	  /// Type of the values.
	  typedef mln_value(I) value;

	  /// Dimension of the built complex.
	  static const unsigned D = 2;

	  /// \brief Constructor, with static checks.
	  off_saver();

	  /// Load an image from \a filename into \a ima.
	  void operator()(const I& ima, const std::string& filename) const;
	};


	struct bin_off_saver
	  : public off_saver< bin_2complex_image3df, bin_off_saver >
	{
	  /// \brief Save face data.
	  ///
	  /// Dummy, does nothings (but required by the super class).
	  void write_face_data(std::ostream& ostr, const value& v) const;
	};

	struct int_u8_off_saver
	  : public off_saver< int_u8_2complex_image3df, int_u8_off_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const value& v) const;
	};


	/* FIXME: We should turn float_off_saver into a
	   float01_off_saver (see FIXME/comment in implementation
	   below).  */
	struct float_off_saver
	  : public off_saver< float_2complex_image3df, float_off_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const value& v) const;
	};


	struct rgb8_off_saver
	  : public off_saver< rgb8_2complex_image3df, rgb8_off_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const value& v) const;
	};

      } // end of namespace mln::io::off::internal



# ifndef MLN_INCLUDE_ONLY

      /*----------.
      | Facades.  |
      `----------*/

      inline
      void
      save(const bin_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::save");
	internal::bin_off_saver()(ima, filename);
	trace::exiting("mln::io::off::save");
      }

      inline
      void
      save(const int_u8_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::save");
	internal::int_u8_off_saver()(ima, filename);
	trace::exiting("mln::io::off::save");
      }

      inline
      void
      save(const float_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::save");
	internal::float_off_saver()(ima, filename);
	trace::exiting("mln::io::off::save");
      }

      inline
      void
      save(const rgb8_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::save");
	internal::rgb8_off_saver()(ima, filename);
	trace::exiting("mln::io::off::save");
      }


      /*-------------------------.
      | Actual implementations.  |
      `-------------------------*/

      // -------- //
      // Canvas.  //
      // -------- //

      namespace internal
      {

	template <typename I, typename E>
	off_saver<I, E>::off_saver()
	{
	  // Concept checking.
	  void (E::*m1)(std::ostream&, const value&) const =
	    &E::write_face_data;
	  (void) m1;
	}


	template <typename I, typename E>
	void
	off_saver<I, E>::operator()(const I& ima,
				    const std::string& filename) const
	{
	  const std::string me = "mln::io::off::save";

	  std::ofstream ostr(filename.c_str());
	  if (!ostr)
	    {
	      std::cerr << me << ": `" << filename << "' invalid file."
			<< std::endl;
	      /* FIXME: Too violent.  We should allow the use of
		 exceptions, at least to have Milena's code behave
		 correctly in interpreted environments (std::exit() or
		 std::abort() causes the termination of a Python
		 interpreter, for instance!).  */
	      std::exit(1);
	    }

	  /*---------.
	  | Header.  |
	  `---------*/

	  /* ``The .off files in the Princeton Shape Benchmark conform
	     to the following standard.'' */

	  /* ``OFF files are all ASCII files beginning with the
	     keyword OFF. ''  */
	  ostr << "OFF" << std::endl;

	  // A comment.
	  ostr << "# Generated by Milena 1.0  http://olena.lrde.epita.fr\n"
	       << "# EPITA Research and Development Laboratory (LRDE)"
	       << std::endl;

	  /* ``The next line states the number of vertices, the number
	     of faces, and the number of edges. The number of edges can
	     be safely ignored.'' */
	  /* FIXME: This is too long.  We shall be able to write

	     ima.domain().template nfaces_of_static_dim<0>()

	     or even

	     ima.template nfaces_of_static_dim<0>().
	  */
	  ostr << ima.domain().cplx().template nfaces_of_static_dim<0>() << ' '
	       << ima.domain().cplx().template nfaces_of_static_dim<2>() << ' '
	       << ima.domain().cplx().template nfaces_of_static_dim<1>()
	       << std::endl;

	  /*-------.
	  | Data.  |
	  `-------*/

	  // --------- //
	  // Complex.  //
	  // --------- //

	  typedef mln_geom(I) G;

	  // ------------------------------------------ //
	  // Vertices & geometry (vertices locations).  //
	  // ------------------------------------------ //

	  /* ``The vertices are listed with x, y, z coordinates, written
	     one per line.'' */

	  // Traverse the 0-faces (vertices).
	  p_n_faces_fwd_piter<D, G> v(ima.domain(), 0);
	  for_all(v)
	  {
	    mln_invariant(v.to_site().size() == 1);
	    ostr << v.to_site().front()[0] << ' '
		 << v.to_site().front()[1] << ' '
		 << v.to_site().front()[2] << std::endl;
	  }

	  // --------------- //
	  // Faces & edges.  //
	  // --------------- //

	  /* ``After the list of vertices, the faces are listed, with one
	     face per line. For each face, the number of vertices is
	     specified, followed by indices into the list of
	     vertices.''  */

	  // Traverse the 2-faces (polygons).
	  p_n_faces_fwd_piter<D, G> f(ima.domain(), 2);

	  typedef complex_m_face_neighborhood<D, G> nbh_t;
	  // A neighborhood where neighbors are the set of 0-faces
	  // transitively adjacent to the reference point.
	  nbh_t nbh;
	  mln_fwd_niter(nbh_t) u(nbh, f);
	  /* FIXME: We should be able to pas this value (m) either at
	     the construction of the neighborhood or at the construction
	     of the iterator.  */
	  u.iter().set_m(0);

	  // For each (2-)face, iterate on (transitively) ajacent
	  // vertices (0-faces).
	  for_all(f)
	  {
	    unsigned nvertices = 0;
	    std::ostringstream vertices;
	    for_all(u)
	    {
	      // FIXME: Likewise, this is a bit too long...
	      vertices << ' ' << u.unproxy_().face().face_id();
	      ++nvertices;
	    }
	    ostr << nvertices << vertices.str();
	    // Possibly save a value (depends on the actual format).
	    exact(this)->write_face_data(ostr, ima(f));
	    ostr << std::endl;
	  }

	  ostr.close();
	}

	// ---------------- //
	// Specific parts.  //
	// ---------------- //

	/** \brief Writing values.

	    From https://people.scs.fsu.edu/~burkardt/html/off_format.html:

	    ``Following these [coordinates] are the face descriptions,
	    typically written with one line per face. Each has the
	    form

	    N  Vert1 Vert2 ... VertN  [color]

	    Here N is the number of vertices on this face, and Vert1
	    through VertN are indices into the list of vertices (in
	    the range 0..NVertices-1).

	    The optional color may take several forms. Line breaks
	    are significant here: the color description begins after
	    VertN and ends with the end of the line (or the next #
	    comment). A color may be:

	    nothing
		the default color
	    one integer
		index into "the" colormap; see below
	    three or four integers
		RGB and possibly alpha values in the range 0..255
	    three or four floating-point numbers
		RGB and possibly alpha values in the range 0..1

	    For the one-integer case, the colormap is currently read
	    from the file `cmap.fmap' in Geomview's `data'
	    directory. Some better mechanism for supplying a colormap
	    is likely someday.

	    The meaning of "default color" varies. If no face of the
	    object has a color, all inherit the environment's default
	    material color. If some but not all faces have colors, the
	    default is gray (R,G,B,A=.666).''

	    \{ */
	inline
	void
	bin_off_saver::write_face_data(std::ostream& /* ostr */,
				       const value& /* v */) const
	{
	  // Do nothing (no data associated to faces).
	}

	inline
	void
	int_u8_off_saver::write_face_data(std::ostream& ostr,
					  const value& v) const
	{
	  /* Using RGBA colors to represent an 8-bit integer value.

	     Each channel (R, G, B) of the color V is an integer in
	     the range 0..255.  A fourth channel, A, controls the
	     transparency.

	     We just set the same value for each channel, as the OFF
	     file format does not support gray-level values as-is.  */
	  ostr << ' ' << v << ' ' << v << ' ' << v
	       << ' ' << 1.0f << std::endl;
	}

	/* FIXME: We should turn float_off_saver into a
	   float01_off_saver to avoid the assertions below.  */
	inline
	void
	float_off_saver::write_face_data(std::ostream& ostr,
					 const value& v) const
	{
	  /* Using RGBA colors to represent a floating-point value.

	     Each channel (R, G, B) of the color V is a floating-point
	     number in the range 0..1.  A fourth channel, A, controls
	     the transparency.

	     We just set the same value for each channel, as the OFF
	     file format does not support gray-level values as
	     such.  */
	  mln_assertion(0.0f <= v);
	  mln_assertion(v <= 1.0f);
	  ostr << ' ' << v << ' ' << v << ' ' << v
	       << ' ' << 1.0f;
	}

	inline
	void
	rgb8_off_saver::write_face_data(std::ostream& ostr,
					const value& v) const
	{
	  /* RGBA color.

	     Each channel (R, G, B) of the color V is an integer in
	     the range 0..255.  A fourth channel, A, controls the
	     transparency.  */
	  ostr << ' ' << v.red() << ' ' << v.green() << ' ' << v.blue()
	       << ' ' << 1.0f;
	}
	/* \} */

      } // end of namespace mln::io::off::internal


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::off

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_OFF_SAVE_HH
