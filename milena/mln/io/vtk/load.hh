// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_VTK_LOAD_HH
# define MLN_IO_VTK_LOAD_HH

/// \file
/// Input saving function for VTK files.
///
/// \see http://www.vtk.org/VTK/img/file-formats.pdf
/// \see http://dunne.uni-hd.de/VisuSimple/documents/vtkfileformat.html
///
/// The routines of this file only supported a subset of the file
/// format(s) defined in the VTK documentation.

# include <cstdlib>
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>

# include <mln/core/alias/complex_image.hh>


namespace mln
{

  namespace io
  {

    namespace vtk
    {

      /** \brief Load a (binary) VTK image into a complex image.

	  \param[in] ima      A reference to the image to construct.
	  \param[in] filename The name of the file to load.
      */
      void load(bin_2complex_image3df& ima, const std::string& filename);



# ifndef MLN_INCLUDE_ONLY

      // FIXME: To be put elsewehre (factored), or encapsulated in a
      // class containing the `load' routine as well.
      namespace internal
      {

	inline
	void
	error(const std::string& caller, const std::string& filename,
	      const std::string& message)
	{
	  std::cerr << caller << ": `" << filename << "': "
		    << message << std::endl;
	  std::exit(1);
	}

	inline
 	void
	accept(const std::string& token, const std::string& expected,
	       const std::string& caller, const std::string& filename,
	       const std::string& message)
	{
	  if (token != expected)
	    error(caller, filename, message);
	}

	// Likewise, with default message.
	inline
 	void
	accept(const std::string& token, const std::string& expected,
	       const std::string& caller, const std::string& filename)
	{
	  accept(token, expected, caller, filename,
		 std::string("parse error (`") + expected + "' expected, "
		 + "got `" + token + "').");
	}

	// FIXME: To be moved elsewhere.
	/// Create a string from X using operator<<.
	template <typename T>
	std::string
	str(const T& x)
	{
	  std::stringstream s;
	  s << x;
	  return s.str();
	}

      } // end of namespace mln::io::vtk::internal


      inline
      void
      load(bin_2complex_image3df& ima, const std::string& filename)
      {
	  typedef bin_2complex_image3df I;

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
	  std::string version;
	  std::getline(istr,  version);
	  // FIXME: Check the format of VERSION.

	  /* ``2. The second part is the header.  The header consists
		  of a character string terminated by end-of-line
		  character `\n'.  The header is 256 characters
		  maximum.  The header can be used to describe the data
		  and include any other pertinent information.'' */
	  std::string header;
	  std::getline(istr,  header);

	  /* ``3. The next part is the file format.  The file format
		  describes the type of file, either ASCII or
		  binary.  On this line the single word ASCII or BINARY
		  must appear.'' */
	  std::string format;
	  istr >> format;
	  if (format == "BINARY")
	    internal::error(me, filename,
			    "`BINARY' VTK format is not supported "
			    "(only `ASCII' is supported yet).");
	  else if (format != "ASCII")
	    internal::error(me, filename,
			    std::string("invalid file format: `")
			    + format + "'.");

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

	  std::string dataset_keyword, dataset_type;
	  istr >> dataset_keyword >> dataset_type;
	  internal::accept(dataset_keyword, "DATASET", me, filename);
	  internal::accept(dataset_type, "POLYDATA", me, filename,
			   "unsupported dataset structure "
			   "(only `POLYDATA' is supported yet).");

	  // --------- //
	  // Complex.  //
	  // --------- //

	  const unsigned D = 2;
	  topo::complex<D> c;

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

	  // ----------------------------------------- //
	  // Vertices and geometry (point locations).  //
	  // ----------------------------------------- //

	  std::string points_keyword;
	  unsigned npoints;
	  std::string points_datatype;
	  istr >> points_keyword >> npoints >> points_datatype;
	  internal::accept(points_keyword, "POINTS", me, filename);
	  internal::accept(points_datatype, "float", me, filename,
			   "unsupported points data type "
			   "(only `float' is supported yet).");

	  typedef point3df P;
	  typedef mln_coord_(P) C;
	  typedef mln_geom_(I) G;
	  G geom;
	  geom.reserve(npoints);
	  for (unsigned i = 0; i < npoints; ++i)
	    {
	      // Create a 0-face.
	      c.add_face();

	      // Record the geometry (point) associated to this vertex.
	      C x, y, z;
	      istr >> x >> y >> z;
	      geom.add_location(P(x, y, z));
	    }

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

	  /* FIXME: Add another constraint: a bijection between
	     VERTICES and POINTS shall exist.  Update the following
	     comment accordingly.  */

	  /* The VTK file format contains both a POINTS and a VERTICES
	     section.  Despite its name, the former is used to create
	     the set of vertices of the complex (because POINTS are
	     later used to form line (edges) and polygons.  The latter
	     (VERTICES) is used to attach values (data) to 0-faces,
	     and act as an indirection toward POINTS.  The fact is,
	     most VTK file we use have a VERTICES section where the
	     N-th line (starting at 0) contains

	       1 N

	     which makes the VERTICES section kind of useless.
	     However, we have to parse it and take it into account for
	     the sake of the file format.  */

	  std::string vertices_keyword;
	  unsigned nvertices, vertices_size;
	  istr >> vertices_keyword >> nvertices >> vertices_size;
	  internal::accept(vertices_keyword, "VERTICES", me, filename);

	  /*	   Each vertex requires two numbers: the cardinal of
		   its ends (which is always 1) and the indices of the
		   point among the POINTS section.  Hence the total
		   number of values in the VERTEX section is
		   nvertices * 2.  */
	  // FIXME: This test is not really robust: we should involve
	  // the VERTICES_SIZE value in the parsing process.
	  if (vertices_size != nvertices * 2)
	    internal::error(me, filename, "ill-formed `VERTICES' section.");

	  // Vertices built on points.
	  std::vector<unsigned> vertices;
	  vertices.reserve(npoints);
	  for (unsigned i = 0; i < nvertices; ++i)
	    {
	      unsigned numpoints, p;
	      istr >> numpoints >> p;
	      if (numpoints != 1)
		internal::error(me, filename, "ill-formed vertex item.");
	      if (p > npoints)
		internal::error(me, filename,
				"point id out of bounds in vertex item.");
	      vertices.push_back(p);
	    }

	  // ------- //
	  // Edges.  //
	  // ------- //

	  std::string lines_keyword;
	  unsigned nedges, edges_size;
	  istr >> lines_keyword >> nedges >> edges_size;
	  internal::accept(lines_keyword, "LINES", me, filename);

	  /*	   Each edge requires three numbers: the cardinal of
		   its ends (which is always 2) and the indices of
		   these ends among the POINTS section.  Hence the
		   total number of values in the LINES section is
		   nedges * 3.  */
	  // FIXME: This test is not really robust: we should involve
	  // the EDGES_SIZE value in the parsing process.
	  if (edges_size != nedges * 3)
	    internal::error(me, filename, "ill-formed `LINES' section.");

	  // An adjacenty matrix recording the edges seen so far.
	  typedef std::vector< std::vector<bool> > complex_edges_t;
	  complex_edges_t complex_edges (npoints,
					 std::vector<bool>(npoints, false));

	  // Populate the complex and the adjacency matrix
	  for (unsigned i = 0; i < nedges; ++i)
	    {
	      // Create a 1-face.	      
	      unsigned numpoints, p1, p2;
	      istr >> numpoints >> p1 >> p2;
	      if (numpoints != 2)
		internal::error(me, filename, "ill-formed line item.");
	      if (p1 > npoints || p2 > npoints)
		internal::error(me, filename,
				"point id out of bounds in line item.");
	      topo::n_face<0, D> v1(c, p1);
	      topo::n_face<0, D> v2(c, p2);
	      c.add_face(v1 - v2);
	      // Tag this edged (and its opposite) in the adjacency matrix.
	      complex_edges[p1][p2] = true;
	      complex_edges[p2][p1] = true;
	    }

	  // ---------- //
	  // Polygons.  //
	  // ---------- //

	  std::string polygons_keyword;
	  unsigned npolygons, polygons_size;
	  istr >> polygons_keyword >> npolygons >> polygons_size;
	  internal::accept(polygons_keyword, "POLYGONS", me, filename);

	  // Read polygon values.
	  unsigned npolygons_values = 0;

	  for (unsigned i = 0; i < npolygons; ++i)
	    {
	      unsigned numpoints;
	      istr >> numpoints;
	      ++npolygons_values;
	      if (numpoints <= 2)
		{
		  internal::error(me, filename,
				  std::string("ill-formed face (having ")
				  + internal::str(numpoints)
				  + (numpoints < 2 ? "vertex" : "vertices")
				  + ").");
		}

	      /* FIXME: This part (computing the set of edges on which
	         a polygon is built) is also shared by
	         mln::io::off::load; we can probably factor it.  */

	      // The edges of the face.
	      topo::n_faces_set<1, D> face_edges_set;
	      face_edges_set.reserve(numpoints);

	      // Remember the first point id of the face.
	      unsigned first_point_id;
	      istr >> first_point_id;
	      ++npolygons_values;
	      // The current point id, initialized with the first id.
	      unsigned point_id = first_point_id;
	      if (point_id >= npoints)
		internal::error(me, filename,
				std::string("invalid point id: `")
				+ internal::str(point_id) + "'.");
	      for (unsigned p = 0; p < numpoints; ++p)
		{
		  /* The next point id.  The pair (point_id,
		     next_point_id) is an edge of the
		     mesh/complex.  */
		  unsigned next_point_id;
		  /* When P is the id of the last point of the face,
		     set NEXT_POINT_ID to FIRST_VERTEX_ID; otherwise,
		     read it from the input.  */
		  if (p == numpoints - 1)
		    next_point_id = first_point_id;
		  else
		    {
		      istr >> next_point_id;
		      ++npolygons_values;
		      if (next_point_id >= npoints)
			internal::error(me, filename,
					std::string("invalid point id: `")
					// In-line ``itoa'' of NEXT_POINT_ID.
					+ internal::str(next_point_id) + "'.");
		    }
		  // The ends of the current edge.
		  topo::n_face<0, D> vertex(c, point_id);
		  topo::n_face<0, D> next_vertex(c, next_point_id);
		  // The current edge.
		  topo::algebraic_n_face<1, D> edge;
		  /* The edge (POINT_ID, NEXT_POINT_ID) (or its
		     opposite (NEXT_POINT_ID, POINT_ID) must have been
		     inserted previously.  In other words, the sides
		     of POLYGON items must belong to the list of
		     LINES.  */
		  if (!complex_edges[point_id][next_point_id])
		    internal::error(me, filename,
				    "ill-formed polygon, having a side "
				    "(edge) not part of the list of LINES.");
		  edge = topo::edge(vertex, next_vertex);
		  mln_assertion(edge.is_valid());
		  // Add this edge a side of the polygon.
		  face_edges_set += edge;
		  // Next vertex.
		  point_id = next_point_id;
		}

	      // Add face.
	      c.add_face(face_edges_set);
	    }

	  // FIXME: This test is not really robust: we should involve
	  // the POLYGONS_SIZE value in the parsing process.
	  if (polygons_size != npolygons_values)
	    internal::error(me, filename, "ill-formed `LINES' section.");
  
	  // -------------------- //
	  // Dataset attributes.  //
	  // -------------------- //

	  /* ``5. The final part describes the dataset attributes.
		  This part begins with the keywords POINT_DATA or
		  CELL_DATA, followed by an integer number specifying
		  the number of points or cells, respectively.  (It
		  doesn't matter whether POINT_DATA or CELL_DATA comes
		  first.)  Other keyword/data combinations then define
		  the actual dataset attribute values (i.e., scalars,
		  vectors, tensors, normals, texture coordinates, or
		  field data).'' */
	  std::string dataset_kind;
	  istr >> dataset_kind;
	  if (dataset_kind == "POINT_DATA")
	    internal::error(me, filename,
			    "`POINT_DATA' datasets are not supported "
			    "(only `CELL_DATA' datasets are supported yet).");
	  else if (dataset_kind != "CELL_DATA")
	    internal::error(me, filename,
			    std::string("invalid dataset kind: `")
			    + dataset_kind + "'.");
	  unsigned nfaces = nvertices + nedges + npolygons;
	  unsigned ncell_data;
	  istr >> ncell_data;
	  if (ncell_data != nfaces)
	    internal::error(me, filename,
			    std::string("wrong number of dataset attributes ")
			    + "(expected " + internal::str(nfaces)
			    + ", got " + internal::str(ncell_data) + ").");

	  /*    ``Dataset Attribute Format.  The Visualization Toolkit
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

	  std::string scalars_keyword, data_name, data_type;
	  istr >> scalars_keyword >> data_name >> data_type;
	  internal::accept(scalars_keyword, "SCALARS", me, filename);
	  // FIXME: As in the rest of this routine's code, we only
	  // handle the case of binary values (for the moment).
	  if (data_type != "bit")
	    internal::error(me, filename,
			    std::string("unsupported data (value) type : `")
			    + data_type + "' (only `bit' is supported yet).");

	  std::string lookup_table_keyword, lookup_table_name;
	  istr >> lookup_table_keyword >> lookup_table_name;
	  internal::accept(lookup_table_keyword, "LOOKUP_TABLE", me, filename);

	  // Values.
	  typedef mln_value_(I) V;
	  typedef metal::vec<D + 1, std::vector<V> > values;
	  values vs;

	  // Populate values associated to 0-faces.
	  /* FIXME: The default value used (here 0) depends on the
	     images's value type.  */
	  vs[0].resize(npoints, 0);
	  // Iterate on VERTICES (not ``points'').  We cannot copy
	  // VERTICES into VS[0] as-is, since VS[0] corresponds to
	  // points.
	  for (std::vector<unsigned>::const_iterator v = vertices.begin();
	       v != vertices.end(); ++ v)
	    {
	      V value;
	      istr >> value;
	      // The point id on which the vertex V is built.
	      unsigned p = *v;
	      vs[0][p] = value;
	    }

	  // Populate values associated to 1-faces.
	  for(unsigned e = 0; e < nedges; ++e)
	    {
	      V value;
	      istr >> value;
	      vs[1].push_back(value);
	    }

	  // Populate values associated to 2-faces.
	  for(unsigned f = 0; f < npolygons; ++f)
	    {
	      V value;
	      istr >> value;
	      vs[2].push_back(value);
	    }

	  /*--------.
	  | Image.  |
	  `--------*/
	  
	  // Site set.
	  typedef mln_domain_(I) domain;
	  domain s(c, geom);

	  // Image.
	  ima.init_(s, vs);

	  /*--------------.
	  | End of file.  |
	  `--------------*/

	  // FIXME: Eat comments and whitespace, and check the end of
	  // the file has been reached.
	  // ...
	  istr.close();
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::io::vtk

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_VTK_LOAD_HH
