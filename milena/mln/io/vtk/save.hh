// Copyright (C) 2008, 2009, 2010, 2011, 2013 EPITA Research and Development
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

#ifndef MLN_IO_VTK_SAVE_HH
# define MLN_IO_VTK_SAVE_HH

/// \file
/// Input saving function for VTK files.
///
/// \see http://www.vtk.org/VTK/img/file-formats.pdf
/// \see http://dunne.uni-hd.de/VisuSimple/documents/vtkfileformat.html

# include <cstdlib>

# include <iostream>
# include <fstream>
# include <sstream>

# include <string>

# include <mln/version.hh>
# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>


namespace mln
{

  namespace io
  {

    namespace vtk
    {

      /** \brief Save a (binary) VTK image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the
			      image.
      */
      void save(const bin_2complex_image3df& ima,
		const std::string& filename);

      /** \brief Save an 8-bit gray-level VTK image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the
			      image.
      */
      void save(const int_u8_2complex_image3df& ima,
		const std::string& filename);

      /** \brief Save a gray-level VTK image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the
			      image.  */
      void save(const unsigned_2complex_image3df& ima,
		const std::string& filename);

      /** \brief Save a floating-point value gray-level VTK image into
	  a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the
			      image.  */
      void save(const float_2complex_image3df& ima,
		const std::string& filename);

      /** \brief Save a 3x8-bit RGB (color) VTK image into a complex image.

	  \param[in] ima      The image to save.
	  \param[in] filename The name of the file where to save the
			      image.  */
      void save(const rgb8_2complex_image3df& ima,
		const std::string& filename);


      namespace internal
      {

	template <typename I, typename E>
	struct vtk_saver : public Object<E>
	{
	  /// Type of the image.
	  typedef I image;

	  /// Type of the values.
	  typedef mln_value(I) value;

	  /// Dimension of the built complex.
	  static const unsigned D = 2;

	  /// \brief Constructor, with static checks.
	  vtk_saver();

	  /// Save an image \a ima into \a filename.
	  void operator()(const I& ima, const std::string& filename) const;

	protected:
	  /// Helper factoring the task of writing scalar data
	  /// associated to faces.
	  void write_scalar_data(std::ostream& ostr, const image& ima,
				 const std::string& data_type) const;
	};


	struct bin_vtk_saver
	  : public vtk_saver< bin_2complex_image3df, bin_vtk_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const image& ima) const;
	};


	struct int_u8_vtk_saver
	  : public vtk_saver< int_u8_2complex_image3df, int_u8_vtk_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const image& ima) const;
	};


	struct unsigned_vtk_saver
	  : public vtk_saver< unsigned_2complex_image3df, unsigned_vtk_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const image& ima) const;
	};


	struct float_vtk_saver
	  : public vtk_saver< float_2complex_image3df, float_vtk_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const image& ima) const;
	};


	struct rgb8_vtk_saver
	  : public vtk_saver< rgb8_2complex_image3df, rgb8_vtk_saver >
	{
	  /// \brief Save face data.
	  void write_face_data(std::ostream& ostr, const image& ima) const;
	};

      } // end of namespace mln::io::vtk::internal



# ifndef MLN_INCLUDE_ONLY

      /*----------.
      | Facades.  |
      `----------*/

      void
      save(const bin_2complex_image3df& ima, const std::string& filename)
      {
	mln_trace("mln::io::vtk::save");
	internal::bin_vtk_saver()(ima, filename);
      }

      void
      save(const int_u8_2complex_image3df& ima, const std::string& filename)
      {
	mln_trace("mln::io::vtk::save");
	internal::int_u8_vtk_saver()(ima, filename);
      }

      void
      save(const unsigned_2complex_image3df& ima, const std::string& filename)
      {
	mln_trace("mln::io::vtk::save");
	internal::unsigned_vtk_saver()(ima, filename);
      }

      void
      save(const float_2complex_image3df& ima, const std::string& filename)
      {
	mln_trace("mln::io::vtk::save");
	internal::float_vtk_saver()(ima, filename);
      }

      void
      save(const rgb8_2complex_image3df& ima, const std::string& filename)
      {
	mln_trace("mln::io::vtk::save");
	internal::rgb8_vtk_saver()(ima, filename);
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
	vtk_saver<I, E>::vtk_saver()
	{
	  // Concept checking.
	  void (E::*m1)(std::ostream&, const I&) const =
	    &E::write_face_data;
	  (void) m1;
	}


	template <typename I, typename E>
	void
	vtk_saver<I, E>::operator()(const I& ima,
				    const std::string& filename) const
	{
	  const std::string me = "mln::io::vtk::save";

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

	  /* ``The legacy VTK file formats consist of five basic
	     parts.'' */

	  /* ``1. The first part is the file version and
		  identifier.  This part contains the single line:

		    # vtk DataFile Version x.x.

		  This line must be exactly as shown with the
		  exception of the version number x.x, which will vary
		  with different releases of VTK.  (Note: the current
		  version number is 3.0.  Version 1.0 and 2.0 files are
		  compatible with version 3.0 files.)'' */
	  ostr << "# vtk DataFile Version 2.0" << std::endl;

	  /* ``2. The second part is the header.  The header consists
		  of a character string terminated by end-of-line
		  character `\n'.  The header is 256 characters
		  maximum.  The header can be used to describe the data
		  and include any other pertinent information.'' */
	  ostr << "Generated by " OLN_PACKAGE_STRING " (" OLN_PACKAGE_URL ")"
	       << std::endl;

	  /* ``3. The next part is the file format.  The file format
		  describes the type of file, either ASCII or
		  binary.  On this line the single word ASCII or BINARY
		  must appear.'' */
	  ostr << "ASCII" << std::endl;

	  /*-------.
	  | Data.  |
	  `-------*/

	  /* ``4. The fourth part is the dataset structure.  The
		  geometry part describes the geometry and topology of
		  the dataset.  This part begins with a line containing
		  the keyword DATASET followed by a keyword describing
		  the type of dataset.  Then, depending upon the type
		  of dataset, other keyword/data combinations define
		  the actual data.''

		  [...]

		  Dataset Format.  The Visualization Toolkit supports
		    five different dataset formats: structured points,
		    structured grid, rectilinear grid, unstructured
		    grid, and polygonal data.'' */

	  ostr << "DATASET POLYDATA" << std::endl << std::endl;

	  // --------- //
	  // Complex.  //
	  // --------- //

	  typedef mln_geom(I) G;

	  /*	  ``* Polygonal Data
		      The polygonal dataset consists of arbitrary
		      combinations of surface graphics primitives
		      vertices (and polyvertices), lines (and
		      polylines), polygons (of various types), and
		      triangle strips. Polygonal data is defined by
		      the POINTS, VERTICES, LINES, POLYGONS, or
		      TRIANGLE_STRIPS sections.  The POINTS definition
		      is the same as we saw for structured grid
		      datasets.'' */

	  // ---------------------------- //
	  // Geometry (point locations).  //
	  // ---------------------------- //

	  ostr << "POINTS "
	       << ima.domain().cplx().template nfaces_of_static_dim<0>()
	       << " float" << std::endl;
	  // Iterate on 0-faces (vertices).
	  p_n_faces_fwd_piter<D, G> v(ima.domain(), 0);
	  for_all(v)
	  {
	    mln_invariant(v.to_site().size() == 1);
	    ostr << v.to_site().front()[0] << ' '
		 << v.to_site().front()[1] << ' '
		 << v.to_site().front()[2] << std::endl;
	  }
	  ostr << std::endl;

	  /*        ``The VERTICES, LINES, POLYGONS, or
		      TRIANGLE_STRIPS keywords define the polygonal
		      dataset topology.  Each of these keywords
		      requires two parameters: the number of cells `n'
		      and the size of the cell list `size'.  The cell
		      list size is the total number of integer values
		      required to represent the list (i.e., sum of
		      `numPoints' and connectivity indices over each
		      cell).  None of the keywords VERTICES, LINES,
		      POLYGONS, or TRIANGLE_STRIPS is required.'' */

	  // ---------- //
	  // Vertices.  //
	  // ---------- //

	  /* We do not use

	       ima.domain().cplx().template nfaces_of_static_dim<N>()

	     to get the number of N-faces, since the image may be
	     masked, and exhibit less N-faces than its underlying
	     complex.  Iterating on the N-faces is safer.  */
	  /* FIXME: Is there anything faster?  See what the interface
	     of the (morphed) image can provide.  */
	  unsigned nvertices = 0;
	  for_all(v)
	    ++nvertices;

	  if (nvertices > 0)
	    {
	      ostr << "VERTICES " << nvertices << ' '
		/* Each vertex requires two numbers: the cardinal of
		   its ends (which is always 1) and the indices of the
		   point among the POINTS section.  Hence the total
		   number of values in the VERTEX section is
		   nvertices * 2.  */
		   << nvertices * 2 << std::endl;

	      for_all(v)
		ostr << "1 " << v.unproxy_().face().face_id() << std::endl;
	      ostr << std::endl;
	    }

	  // ------- //
	  // Edges.  //
	  // ------- //

	  // Same comment as above about the count of N-faces.
	  unsigned nedges = 0;
	  p_n_faces_fwd_piter<D, G> e(ima.domain(), 1);
	  for_all (e)
	    ++nedges;

	  if (nedges > 0)
	    {
	      ostr << "LINES " << nedges << ' '
		/* Each edge requires three numbers: the cardinal of
		   its ends (which is always 2) and the indices of
		   these ends among the POINTS section.  Hence the
		   total number of values in the LINES section is
		   nedges * 3.  */
		   << nedges * 3 << std::endl;

	      // Vertices adjacent to edges.
	      typedef complex_lower_neighborhood<D, G> adj_vertices_nbh_t;
	      adj_vertices_nbh_t adj_vertices_nbh;
	      mln_niter(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, e);
	      // Iterate on 1-faces (edges).
	      for_all (e)
	      {
		ostr << "2";
		// Iterate on vertices (0-faces).
		for_all (adj_v)
		{
		  // FIXME: Accessing the face id is too complicated.
		  ostr << " " << adj_v.unproxy_().face().face_id();
		}
		ostr << std::endl;
	      }
	      ostr << std::endl;
	    }

	  // ---------- //
	  // Polygons.  //
	  // ---------- //

	  // Same comment as above about the count of N-faces.
	  unsigned npolygons = 0;
	  p_n_faces_fwd_piter<D, G> p(ima.domain(), 2);
	  for_all (p)
	    ++npolygons;

	  if (npolygons > 0)
	    {
	      // A neighborhood where neighbors are the set of 0-faces
	      // transitively adjacent to the reference point.
	      typedef complex_m_face_neighborhood<D, G> nbh_t;
	      nbh_t nbh;
	      mln_fwd_niter(nbh_t) u(nbh, p);
	      /* FIXME: We should be able to pass this value (m)
		 either at the construction of the neighborhood or at
		 the construction of the iterator.  */
	      u.iter().set_m(0);

	      /* Compute the number of values (`size') to be passed as
		 second parameter of the POLYGONS keyword.  */
	      unsigned polygons_size = 0;
	      // Iterate on polygons (2-face).
	      for_all(p)
	      {
		unsigned nvertices = 0;
		/* FIXME: There may be a faster way to do this (e.g.,
		   the neighbordhood may provide a method returning
		   the number of P's neighbors.  */
		// Iterate on transitively adjacent vertices (0-face).
		for_all(u)
		  ++nvertices;
		// The number of values describing this polygon P is
		// the cardinal of its set of vertices (1 value) plus
		// the NVERTICES indices of these vertices.
		polygons_size += 1 + nvertices;
	      }
	      ostr << "POLYGONS " << npolygons << ' ' << polygons_size
		   << std::endl;

	      /* Output polygons (one per line), with their number of
		 vertices and the indices of these vertices.  */
	      // Iterate on polygons (2-face).
	      for_all(p)
	      {
		unsigned nvertices = 0;
		std::ostringstream vertices;
		// Iterate on transitively adjacent vertices (0-face).
		for_all(u)
		{
		  // FIXME: Likewise, this is a bit too long.
		  vertices << ' ' << u.unproxy_().face().face_id();
		  ++nvertices;
		}
		ostr << nvertices << vertices.str() << std::endl;
	      }
	      ostr << std::endl;
	    }

	  // -------------------- //
	  // Dataset attributes.  //
	  // -------------------- //

	  /* ``5. The final part describes the dataset attributes.
		  This part begins with the keywords POINT_DATA or
		  CELL_DATA,followed by an integer number specifying
		  the number of points or cells, respectively.  (It
		  doesn't matter whether POINT_DATA or CELL_DATA comes
		  first.)  Other keyword/data combinations then define
		  the actual dataset attribute values (i.e., scalars,
		  vectors, tensors, normals, texture coordinates, or
		  field data).'' */

	  unsigned nfaces = nvertices + nedges + npolygons;
	  if (nfaces > 0)
	    {
	      // We don't use POINT_DATA (to associate values to
	      // POINTs), since CELL_DATA is used to associate values
	      // to VERTICES, EDGES and POLYGONS.
	      ostr << "CELL_DATA " << nfaces << std::endl;
	      exact(this)->write_face_data(ostr, ima);
	    }

	  ostr.close();
	}

	// ---------------- //
	// Specific parts.  //
	// ---------------- //

	/*      ``Dataset Attribute Format.  The Visualization Toolkit
		    supports the following dataset attributes: scalars
		    (one to four components), vectors, normals,
		    texture coordinates (1D, 2D, and 3D), 3 x 3
		    tensors, and field data.  In addition, a lookup
		    table using the RGBA color specification,
		    associated with the scalar data, can be defined as
		    well.  Dataset attributes are supported for both
		    points and cells.
		      Each type of attribute data has a `dataName'
		    associated with it.  This is a character string
		    (without embedded whitespace) used to identify a
		    particular data.  The `dataName' is used by the VTK
		    readers to extract data.  As a result, more than
		    one attribute data of the same type can be
		    included in a file.  For example, two different
		    scalar fields defined on the dataset points,
		    pressure and temperature, can be contained in the
		    same file.  (If the appropriate dataName is not
		    specified in the VTK reader, then the first data
		    of that type is extracted from the file.)

		    * Scalars
		      Scalar definition includes specification of a
		      lookup table.  The definition of a lookup table
		      is optional.

		      [...]

			SCALARS dataName dataType numComp
			LOOKUP_TABLE tableName
			s0
			s1
			...
			sn-1''

	   Note: values accepted by Paraview 3.8 for `dataType' are:
	   "bit", "char", "unsigned_char", "short", "unsigned_short",
	   "vtkidtype", "int", "unsigned_int", "long",
	   "unsigned_long", "vtktypeint64", "vtktypeuint64", "float",
	   "double", "string", "utf8_string" and "variant".  */

	/** \{ */
	template <typename I, typename E>
	void
	vtk_saver<I, E>::write_scalar_data(std::ostream& ostr,
					   const image& ima,
					   const std::string& data_type) const
	{
	  ostr << "SCALARS values " << data_type << std::endl
	       << "LOOKUP_TABLE default" << std::endl;
	  // Iterate on all faces, dimension increasing.
	  mln_fwd_piter(image) p(ima.domain());
	  for_all(p)
	    ostr << ima(p) << std::endl;
	}


	void
	bin_vtk_saver::write_face_data(std::ostream& ostr,
				       const image& ima) const
	{
	  write_scalar_data(ostr, ima, "bit");
	}

	void
	int_u8_vtk_saver::write_face_data(std::ostream& ostr,
					  const image& ima) const
	{
	  write_scalar_data(ostr, ima, "unsigned_char");
	}

	void
	unsigned_vtk_saver::write_face_data(std::ostream& ostr,
					    const image& ima) const
	{
	  write_scalar_data(ostr, ima, "unsigned_int");
	}

	void
	float_vtk_saver::write_face_data(std::ostream& ostr,
					 const image& ima) const
	{
	  write_scalar_data(ostr, ima, "float");
	}

	/*          ``The definition of color scalars (i.e., unsigned
		      char values directly mapped to color) varies
		      depending upon the number of values (`nValues')
		      per scalar.  If the file format is ASCII, the
		      color scalars are defined using nValues float
		      values between (0,1).

			COLOR_SCALARS dataName nValues
			c00 c01 ... c0(nValues-1)
			c10 c11 ... c1(nValues-1)
			...
			c(n-1)0 c(n-1)1 ... c(n-1)(nValues-1)'' */

	void
	rgb8_vtk_saver::write_face_data(std::ostream& ostr,
					const image& ima) const
	{
	  ostr << "COLOR_SCALARS values 4" << std::endl;
	  // Iterate on all faces, dimension increasing.
	  mln_fwd_piter_(image) p(ima.domain());
	  for_all(p)
	    // RGBA values (with alpha channel always set to 1.0).
	    ostr << float(ima(p).red())   / mln_max(value::red_t)   << ' '
		 << float(ima(p).green()) / mln_max(value::green_t) << ' '
		 << float(ima(p).blue())  / mln_max(value::blue_t)  << ' '
		 << 1.f
		 << std::endl;
	}
	/** \} */

      } // end of namespace mln::io::vtk::internal

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::vtk

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_VTK_SAVE_HH
