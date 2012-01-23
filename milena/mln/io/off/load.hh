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

#ifndef MLN_IO_OFF_LOAD_HH
# define MLN_IO_OFF_LOAD_HH

/// \file
///
/// \brief Input loading function for OFF files.
///
/// \see http://shape.cs.princeton.edu/benchmark/documentation/off_format.html
/// \see https://people.scs.fsu.edu/~burkardt/html/off_format.html

# include <cstdlib>
# include <iostream>
# include <fstream>
# include <string>

# include <mln/literal/black.hh>
# include <mln/core/concept/object.hh>
# include <mln/core/alias/complex_image.hh>


namespace mln
{

  namespace io
  {

    namespace off
    {

      /*! \brief Load a (binary) OFF image into a complex image.

	  \param[out] ima      A reference to the image to construct.
	  \param[in]  filename The name of the file to load.

	  The image is said binary since data only represent the
	  existence of faces.

	  \ingroup iooff
      */
      void load(bin_2complex_image3df& ima, const std::string& filename);

      // FIXME: Implement a load routine for for
      // int_u8_2complex_image3df.

      /*! brief Load a floating-point OFF image into a complex image.

	  \param[out] ima      A reference to the image to construct.
	  \param[in]  filename The name of the file to load.

	  Read floating-point data is attached to 2-faces only;
	  1-faces and 0-faces are set to 0.0f.

	  \ingroup iooff
      */
      void load(float_2complex_image3df& ima, const std::string& filename);

      /*! \brief Load a 3x8-bit RGB (color) OFF image into a complex
          image.

	  \param[out] ima      A reference to the image to construct.
	  \param[in]  filename The name of the file to load.

	  Read floating-point data is attached to 2-faces only;
	  1-faces and 0-faces are set to 0.0f.

	  \ingroup iooff
      */
      void load(rgb8_2complex_image3df& ima, const std::string& filename);


      namespace internal
      {

	template <typename I, typename E>
	struct off_loader : public Object<E>
	{
	  typedef off_loader<I, E> self;

	  /// Dimension of the built complex.
	  static const unsigned D = 2;
	  /// Type of the set of values.
	  typedef metal::vec<D + 1, std::vector< mln_value(I) > > values;
	  /// Type of the domain.
	  typedef mln_domain(I) domain;

	  /// Constructor, with static checks.
	  off_loader();

	  /// Load an image from \a filename into \a ima.
	  void operator()(I& ima, const std::string& filename);

	  /// A stream manipulator eating comments starting with a `#'
	  /// and ending at an end of line.
	  static std::istream& eat_comment(std::istream& istr);
	};


	struct bin_off_loader
	  : public off_loader< bin_2complex_image3df, bin_off_loader >
	{
	  /// Read face data.
	  ///
	  /// Dummy, does nothings (but required by the super class).
	  void read_face_data(std::istream& istr);

	  /// Assign values to image.
	  void assign(values& vs, const domain& s);

	  /// Pre-allocate data.
	  ///
	  /// Dummy, does nothings (but required by the super class).
	  void reserve(unsigned nvertices, unsigned nedges, unsigned nfaces);
	};


	/* Factor float_off_loader::reserve and rgb8_off_loader::reserve
	   and float_off_loader::assign and rgb8_off_loader::assign
	   by introducing an intermediate class.  */

	struct float_off_loader
	  : public off_loader< float_2complex_image3df, float_off_loader >
	{
	  /// Read face data.
	  void read_face_data(std::istream& istr);

	  /// Pre-allocate data.
	  void reserve(unsigned nvertices, unsigned nedges, unsigned nfaces);

	  /// Assign values to image.
	  void assign(values& vs, const domain& s);

	  /// 2-face floating-point values.
	  std::vector<float> face_value;
	};


	struct rgb8_off_loader
	  : public off_loader< rgb8_2complex_image3df, rgb8_off_loader >
	{
	  /// Read face data.
	  void read_face_data(std::istream& istr);

	  /// Pre-allocate data.
	  void reserve(unsigned nvertices, unsigned nedges, unsigned nfaces);

	  /// Assign values to image.
	  void assign(values& vs, const domain& s);

	  /// 2-face floating-point values.
	  std::vector<value::rgb8> face_value;
	};

      } // end of namespace mln::io::off::internal



# ifndef MLN_INCLUDE_ONLY

      /*----------.
      | Facades.  |
      `----------*/

      inline
      void
      load(bin_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::load");
	internal::bin_off_loader()(ima, filename);
	trace::exiting("mln::io::off::load");
      }

      inline
      void
      load(float_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::load");
	internal::float_off_loader()(ima, filename);
	trace::exiting("mln::io::off::load");
      }

      inline
      void
      load(rgb8_2complex_image3df& ima, const std::string& filename)
      {
	trace::entering("mln::io::off::load");
	internal::rgb8_off_loader()(ima, filename);
	trace::exiting("mln::io::off::load");
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
	inline
	off_loader<I, E>::off_loader()
	{
	  // Concept checking.
	  void (E::*m1)(std::istream&) = &E::read_face_data;
	  (void) m1;
	  void (E::*m2)(unsigned, unsigned, unsigned) = &E::reserve;
	  (void) m2;
	  void (E::*m3)(values&, const domain&) = &E::assign;
	  (void) m3;
	}


	template <typename I, typename E>
	inline
	void
	off_loader<I, E>::operator()(I& ima, const std::string& filename)
	{
	  const std::string me = "mln::io::off::load";

	  std::ifstream istr(filename.c_str());
	  if (!istr)
	    {
	      std::cerr << me << ": `" << filename << "' not found."
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
	     to the following standard''. */

	  /* ``OFF files are all ASCII files beginning with the keyword
	     OFF. ''  */
	  std::string type;
	  istr >> &self::eat_comment >> type;
	  if (type != "OFF")
	    {
	      std::cerr << me << ": `" << filename << "': ill-formed header."
			<< std::endl;
	      std::exit(1);
	    }

	  /* ``The next line states the number of vertices, the number
	     of faces, and the number of edges. The number of edges can
	     be safely ignored.'' */
	  unsigned nvertices, nfaces, nedges;
	  istr >> &self::eat_comment >> nvertices
	       >> &self::eat_comment >> nfaces
	       >> &self::eat_comment >> nedges;

	  /// Possilby pre-allocate data.
	  exact(this)->reserve(nvertices, nedges, nfaces);

	  /*-------.
	  | Data.  |
	  `-------*/

	  /* FIXME: Maybe we could sugar all this (using make_whatever
	     helpers?).  */

	  // --------- //
	  // Complex.  //
	  // --------- //

	  const unsigned D = 2;
	  topo::complex<D> c;

	  // ------------------------------------------ //
	  // Vertices & geometry (vertices locations).  //
	  // ------------------------------------------ //

	  /* ``The vertices are listed with x, y, z coordinates, written
	     one per line.''  */

	  /* FIXME: We should have a faster way to create a bunch of
	     0-faces (vertices).  */
	  for (unsigned v = 0; v < nvertices; ++v)
	    c.add_face();

	  typedef point3df P;
	  typedef mln_coord_(P) C;
	  typedef geom::complex_geometry<D, P> G;
	  G geom;
	  for (unsigned v = 0; v < nvertices; ++v)
	    {
	      C x, y, z;
	      istr >> &self::eat_comment >> x
		   >> &self::eat_comment >> y
		   >> &self::eat_comment >> z;
	      geom.add_location(point3df(x, y, z));
	    }

	  // --------------- //
	  // Faces & edges.  //
	  // --------------- //

	  /* ``After the list of vertices, the faces are listed, with
	     one face per line. For each face, the number of vertices
	     is specified, followed by indices into the list of
	     vertices.''  */

	  // An adjacenty matrix recording the edges seen so far.
	  typedef std::vector< std::vector<bool> > complex_edges_t;
	  complex_edges_t complex_edges (nvertices,
					 std::vector<bool>(nvertices, false));

	  for (unsigned f = 0; f < nfaces; ++f)
	    {
	      unsigned nface_vertices;
	      istr >> &self::eat_comment >> nface_vertices;
	      if (nface_vertices <= 2)
		{
		  std::cerr << me << ": `" << filename
			    << "': ill-formed face (having "
			    << nface_vertices << ' '
			    << (nface_vertices < 2 ? "vertex" : "vertices")
			    << ')' << std::endl;
		  std::exit(1);
		}

	      // The edges of the face.
	      topo::n_faces_set<1, D> face_edges_set;
	      face_edges_set.reserve(nface_vertices);

	      // The first vertex id of the face.
	      unsigned first_vertex_id;
	      istr >> &self::eat_comment >> first_vertex_id;
	      // The current vertex id initialized with the first id.
	      unsigned vertex_id = first_vertex_id;
	      if (first_vertex_id >= nvertices)
		{
		  std::cerr << me << ": `" << filename
			    << "': invalid vertex id " << first_vertex_id
			    << std::endl;
		  std::exit(1);
		}
	      // Iterate on vertices and form edges.
	      for (unsigned v = 0; v < nface_vertices; ++v)
		{
		  /* The next vertex id.  The pair (vertex_id,
		     next_vertex_id) is an edge of the
		     mesh/complex.  */
		  unsigned next_vertex_id;
		  /* When V is the id of the last vertex of the face F,
		     set next_vertex_id to first_vertex_id; otherwise,
		     read it from the input.  */
		  if (v == nface_vertices - 1)
		    next_vertex_id = first_vertex_id;
		  else
		    {
		      istr >> &self::eat_comment >> next_vertex_id;
		      if (next_vertex_id >= nvertices)
			{
			  std::cerr << me << ": `" << filename
				    << "': invalid vertex id "
				    << next_vertex_id << std::endl;
			  std::exit(1);
			}
		    }
		  // The ends of the current edge.
		  topo::n_face<0, D> vertex(c, vertex_id);
		  topo::n_face<0, D> next_vertex(c, next_vertex_id);
		  // The current edge.
		  topo::algebraic_n_face<1, D> edge;
		  // If the edge has been constructed yet, create it;
		  // otherwise, retrieve its id from the complex.
		  if (!complex_edges[vertex_id][next_vertex_id])
		    {
		      complex_edges[vertex_id][next_vertex_id] = true;
		      complex_edges[next_vertex_id][vertex_id] = true;
		      edge =
			make_algebraic_n_face(c.add_face(vertex -
							 next_vertex),
					      true);
		    }
		  else
		    {
		      edge = topo::edge(vertex, next_vertex);
		      mln_assertion(edge.is_valid());
		    }
		  // Record this edge.
		  face_edges_set += edge;
		  // Next vertex.
		  vertex_id = next_vertex_id;
		}

	      // Possibly read a value (depends on the actual format).
	      exact(this)->read_face_data(istr);

	      // Add face.
	      c.add_face(face_edges_set);
	    }

	  /*--------.
	  | Image.  |
	  `--------*/

	  // Site set.
	  domain s(c, geom);

	  // Values
	  values vs;
	  exact(this)->assign(vs, s);

	  // Image.
	  ima.init_(s, vs);

	  /*--------------.
	  | End of file.  |
	  `--------------*/

	  istr >> &self::eat_comment;
	  if (!istr.eof())
	    {
	      std::cerr << me << ": `" << filename
			<< "': end of file not reached" << std::endl;
	      std::exit(1);
	    }
	  istr.close();
	}


	// ---------------- //
	// Specific parts.  //
	// ---------------- //

	/* FIXME: We do not honor the part

	     ``Line breaks are significant here: the color description
	     begins after VertN and ends with the end of the line (or
	     the next # comment).

	  in the following comment.  */

	/** Reading values.

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
	bin_off_loader::read_face_data(std::istream& /* istr */)
	{
	  // Do nothing (no data associated to faces).
	}

	inline
	void
	float_off_loader::read_face_data(std::istream& istr)
	{
	  /* We just use R and ignore G, B and A (transparency) when
	     considering the value (``color'') associated to a face as
	     a (scalar) floating-point value (though it really is an
	     RGB triplet).

	     To ensure consistency, we /might/ (later) check that R, G
	     and B are equal---or better, ``almost equal'', as they
	     are floats.

	     Moreover, R must (and G, B and A should) be
	     floating-point values between 0 and 1, according to the
	     OFF file format definition.  */
	  // FIXME: `A' should be optional.
	  float r, g, b, a;
	  istr >> r >> g >> b >> a;
	  mln_assertion(0.0f <= r);  mln_assertion(r <= 1.0f);
	  mln_assertion(0.0f <= g);  mln_assertion(g <= 1.0f);
	  mln_assertion(0.0f <= b);  mln_assertion(b <= 1.0f);
	  mln_assertion(0.0f <= a);  mln_assertion(a <= 1.0f);
	  face_value.push_back(r);
	}

	inline
	void
	rgb8_off_loader::read_face_data(std::istream& istr)
	{
	  /* We just use R, G, and B and ignore A (transparency) when
	     considering the value (``color'') associated to a face.

	     R must (and G, B and A should) be floating-point values
	     between 0 and 1, according to the OFF file format
	     definition.  */
	  // FIXME: `A' should be optional.
	  float r, g, b, a;
	  istr >> r >> g >> b >> a;
	  mln_assertion(0.0f <= r);  mln_assertion(r <= 1.0f);
	  mln_assertion(0.0f <= g);  mln_assertion(g <= 1.0f);
	  mln_assertion(0.0f <= b);  mln_assertion(b <= 1.0f);
	  mln_assertion(0.0f <= a);  mln_assertion(a <= 1.0f);
	  face_value.push_back(value::rgb8(int(255 * r),
					   int(255 * g),
					   int(255 * b)));
	}
	/* \} */


	inline
	void
	bin_off_loader::reserve(unsigned /* nvertices */,
				unsigned /* nedges */,
				unsigned /* nfaces */)
	{
	  // Do nothing (no data associated to faces).
	}

	inline
	void
	float_off_loader::reserve(unsigned /* nvertices */,
				  unsigned /* nedges */,
				  unsigned nfaces)
	{
	  face_value.reserve(nfaces);
	}


	inline
	void
	rgb8_off_loader::reserve(unsigned /* nvertices */,
				 unsigned /* nedges */,
				 unsigned nfaces)
	{
	  face_value.reserve(nfaces);
	}


	inline
	void
	bin_off_loader::assign(values& vs, const domain& s)
	{
	  // Default values.
	  for (unsigned i = 0; i <= D; ++i)
	    vs[i].insert(vs[i].begin(), s.cplx().nfaces_of_dim(i), true);
	}

	inline
	void
	float_off_loader::assign(values& vs, const domain& s)
	{
	  // Default values for n-face with n in [0, D[.
	  for (unsigned i = 0; i < D; ++i)
	    vs[i].insert(vs[i].begin(), s.cplx().nfaces_of_dim(i), 0.0f);
	  // Values for D-faces.
	  vs[D] = face_value;
	}

	inline
	void
	rgb8_off_loader::assign(values& vs, const domain& s)
	{
	  // Default values for n-face with n in [0, D[.
	  for (unsigned i = 0; i < D; ++i)
	    vs[i].insert(vs[i].begin(), s.cplx().nfaces_of_dim(i),
			 literal::black);
	  // Values for D-faces.
	  vs[D] = face_value;
	}


	// --------- //
	// Helpers.  //
	// --------- //

	template <typename I, typename E>
	inline
	std::istream&
	off_loader<I, E>::eat_comment(std::istream& istr)
	{
	  // Skip whitespace and newlines.
	  std::ws(istr);
	  while (istr.peek() == '#')
	    {
	      /* Eat the `#' and the rest of the line until `\n' or
		 `\r' is found or the end of the file is reached.  */
	      char c;
	      do
		istr.get(c);
	      while (c != '\n' && c != '\r' && !istr.eof());
	      // Skip whitespace and newlines.
	      std::ws(istr);
	    }
	  return istr;
	}

      } // end of namespace mln::io::off::internal


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::io::off

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_OFF_LOAD_HH
