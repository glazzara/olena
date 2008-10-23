// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_IO_OFF_LOAD_HH
# define MLN_IO_OFF_LOAD_HH

/// \file mln/io/off/load.hh
/// \brief Input loading function for OFF files.
///
/// \see http://shape.cs.princeton.edu/benchmark/documentation/off_format.html

# include <cstdlib>

# include <iostream>
# include <fstream>

# include <string>

# include <mln/core/alias/bin_2complex_image3df.hh>


namespace mln
{

  namespace io
  {

    namespace off
    {

      /** \brief Load a (binary) OFF image into a complex image.

	  \param[out] ima      A reference to the image to construct.
	  \param[in]  filename The name of the file to load.

	  The image is said binary since data only represent the
	  existence of faces.  */
      void load(bin_2complex_image3df& ima, const std::string& filename);

      namespace internal
      {
	/// A stream manipulator eating comments starting with a `#'
	/// and ending at an end of line
	std::istream& eat_comment (std::istream& istr);
      }


# ifndef MLN_INCLUDE_ONLY

      void load(bin_2complex_image3df& ima, const std::string& filename)
      {
	const std::string me = "mln::io::off::load";

	std::ifstream istr(filename.c_str());
	if (!istr)
	{
	  std::cerr << me << ": `" << filename << "' not found." << std::endl;
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
	istr >> internal::eat_comment >> type;
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
	istr >> internal::eat_comment >> nvertices
	     >> internal::eat_comment >> nfaces
	     >> internal::eat_comment >> nedges;

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
	   one per line. */

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
	    istr >> internal::eat_comment >> x
		 >> internal::eat_comment >> y
		 >> internal::eat_comment >> z;
	    geom.add_location(point3df(x, y, z));
	  }

	// --------------- //
	// Faces & edges.  //
	// --------------- //

	/* After the list of vertices, the faces are listed, with one
	   face per line. For each face, the number of vertices is
	   specified, followed by indices into the list of
	   vertices.''  */

	// An adjacenty matrix recording the edges seen so far.
	typedef std::vector< std::vector<bool> > complex_edges_t;
	complex_edges_t complex_edges (nvertices,
				       std::vector<bool>(nvertices, false));

	for (unsigned f = 0; f < nfaces; ++f)
	  {
	    unsigned nface_vertices;
	    istr >> internal::eat_comment >> nface_vertices;
	    if (nface_vertices <= 2)
	      {
		std::cerr << me << ": `" << filename
			  << "': ill-formed face (" << nface_vertices
			  << " vertices)" << std::endl;
		std::exit(1);
	      }

	    // The edges of the face.
	    topo::n_faces_set<1, D> face_edges_set;
	    face_edges_set.reserve(nface_vertices);

	    // The first vertex id of the face.
	    unsigned first_vertex_id;
	    istr >> internal::eat_comment >> first_vertex_id;
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
		/* The next vertex id.  The pair (vertex_id, next_vertex_id)
		   is an edge of the mesh/complex.  */
		unsigned next_vertex_id;
		/* When V is the id of the last vertex of the face F,
		   set next_vertex_id to first_vertex_id; otherwise,
		   read it from the input.  */
		if (v == nface_vertices - 1)
		  next_vertex_id = first_vertex_id;
		else
		  {
		    istr >> internal::eat_comment >> next_vertex_id;
		    if (next_vertex_id >= nvertices)
		      {
			std::cerr << me << ": `" << filename
				  << "': invalid vertex id " << next_vertex_id
				  << std::endl;
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
		      make_algebraic_n_face(c.add_face(vertex - next_vertex),
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
	    
	    // Add face.
	    c.add_face(face_edges_set);
	  }

	/*--------.
	| Image.  |
	`--------*/

	// Value type.
	typedef bool V;

	// Site set.
	p_complex<D, G> pc(c, geom);

	// Values.
	metal::vec<D + 1, std::vector<V> > values;
	for (unsigned i = 0; i <= D; ++i)
	  values[i].insert(values[i].begin(), pc.cplx().nfaces(i), true);

	// Image.
	ima.init_(pc, values);

	/*--------------.
	| End of file.  |
	`--------------*/

	istr >> internal::eat_comment;
	if (!istr.eof())
	  {
	    std::cerr << me << ": `" << filename
		      << "': end of file not reached" << std::endl;
	    std::exit(1);
	  }
	istr.close();
      }


      namespace internal
      {
	/// A stream manipulator eating comments starting with a `#'
	/// and ending at an end of line
	std::istream& eat_comment (std::istream& istr)
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
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::off

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_OFF_LOAD_HH
