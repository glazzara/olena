// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_MESH_SEGM_SKEL_IO_HH
# define APPS_MESH_SEGM_SKEL_IO_HH

/// \file apps/mesh-segm-skel/io.hh
/// \brief I/O routines adapted from TriMesh's ones.

#include <cstdio>

#include <algorithm>

#include <TriMesh.h>

#include <mln/value/rgb8.hh>


/// Taken from TriMesh_io.cc
/// \{
// Convert colors float -> uchar
inline unsigned char color2uchar(float p)
{
	return min(max(int(255.0f * p + 0.5f), 0), 255);
}

// Write a bunch of vertices to an ASCII file
inline void write_verts_asc(TriMesh *mesh, FILE *f,
			    const char *before_vert,
			    const char *before_norm,
			    const char *before_color,
			    bool float_color,
			    const char *before_conf,
			    const char *after_line)
{
    for (unsigned i = 0; i < mesh->vertices.size(); i++) {
		fprintf(f, "%s%.7g %.7g %.7g", before_vert,
				mesh->vertices[i][0],
				mesh->vertices[i][1],
				mesh->vertices[i][2]);
		if (!mesh->normals.empty() && before_norm)
			fprintf(f, "%s%.7g %.7g %.7g", before_norm,
				mesh->normals[i][0],
				mesh->normals[i][1],
				mesh->normals[i][2]);
		if (!mesh->colors.empty() && before_color && float_color)
			fprintf(f, "%s%.7g %.7g %.7g", before_color,
				mesh->colors[i][0],
				mesh->colors[i][1],
				mesh->colors[i][2]);
		if (!mesh->colors.empty() && before_color && !float_color)
			fprintf(f, "%s%d %d %d", before_color,
				color2uchar(mesh->colors[i][0]),
				color2uchar(mesh->colors[i][1]),
				color2uchar(mesh->colors[i][2]));
		if (!mesh->confidences.empty() && before_conf)
			fprintf(f, "%s%.7g", before_conf, mesh->confidences[i]);
		fprintf(f, "%s\n", after_line);
	}
}
/// \}


/// Taken and adapted from TriMesh_io.cc
/// \{

/*----------------------.
| OFF with color data.  |
`----------------------*/

/// Write a bunch of faces to an ASCII file with colors.
inline void write_faces_asc_colored(TriMesh *mesh,
				    const std::vector<mln::value::rgb8>& colors,
				    FILE *f,
				    const char *before_face,
				    const char *after_line)
{
  mesh->need_faces();
  for (unsigned i = 0; i < mesh->faces.size(); i++)
    {
      fprintf(f, "%s%d %d %d %d %d %d%s\n",
	      before_face,
	      mesh->faces[i][0], mesh->faces[i][1], mesh->faces[i][2],
	      int(colors[i].red()),
	      int(colors[i].green()),
	      int(colors[i].blue()),
	      after_line);
    }
}

/// Write an off file with colors.
inline void write_off_colored(TriMesh *mesh,
			      const std::vector<mln::value::rgb8>& colors,
			      FILE *f)
{
  fprintf(f, "OFF\n");
  mesh->need_faces();
  fprintf(f, "%lu %lu 0\n", (unsigned long) mesh->vertices.size(),
	  (unsigned long) mesh->faces.size());
  write_verts_asc(mesh, f, "", 0, 0, false, 0, "");
  write_faces_asc_colored(mesh, colors, f, "3 ", "");
}

/*----------------------.
| OFF with float data.  |
`----------------------*/

/// Write a bunch of faces to an ASCII file with colors.
inline void write_faces_asc_float(TriMesh *mesh,
				  const std::vector<float>& values,
				  FILE *f,
				  const char *before_face,
				  const char *after_line)
{
  mesh->need_faces();
  for (unsigned i = 0; i < mesh->faces.size(); i++)
    {
      //            Vertices    Color
      //            -------- ------------
      //            V0 V1 V2  R  G  B  A
      fprintf(f, "%s%d %d %d %f %f %f 1.0%s\n",
	      before_face,
	      mesh->faces[i][0], mesh->faces[i][1], mesh->faces[i][2],
	      values[i], values[i], values[i],
	      after_line);
    }
}

/// Write an off file with floating-point values.
inline void write_off_float(TriMesh *mesh, const std::vector<float>& values,
			    FILE *f)
{
  fprintf(f, "OFF\n");
  mesh->need_faces();
  fprintf(f, "%lu %lu 0\n", (unsigned long) mesh->vertices.size(),
	  (unsigned long) mesh->faces.size());
  write_verts_asc(mesh, f, "", 0, 0, false, 0, "");
  write_faces_asc_float(mesh, values, f, "3 ", "");
}
/// \}


/*---------------------------------------.
| OFF without data (``binary values'').  |
`---------------------------------------*/

/// Taken and adapted from TriMesh_io.cc
/// \{
/// Write a bunch of faces to an ASCII file, removing the ones tagged
/// as ``false'' in \a face_value.
inline void write_faces_asc_binary(TriMesh *mesh,
				   const std::vector<bool>& face_value,
				   FILE *f,
				   const char *before_face,
				   const char *after_line)
{
  mesh->need_faces();
  for (unsigned i = 0; i < mesh->faces.size(); i++)
    if (face_value[i])
    {
      fprintf(f, "%s%d %d %d%s\n",
	      before_face,
	      mesh->faces[i][0], mesh->faces[i][1], mesh->faces[i][2],
	      after_line);
    }
}

/// Write an off file, removing faces tagged as ``false'' in \a face_value.
inline void write_off_binary(TriMesh *mesh,
			     const std::vector<bool>& face_value,
			     FILE *f)
{
  fprintf(f, "OFF\n");
  mesh->need_faces();
  unsigned long nfaces =
    std::count(face_value.begin(), face_value.end(), true);
  fprintf(f, "%lu %lu 0\n", (unsigned long) mesh->vertices.size(), nfaces);
  write_verts_asc(mesh, f, "", 0, 0, false, 0, "");
  write_faces_asc_binary(mesh, face_value, f, "3 ", "");
}
/// \}

#endif // ! APPS_MESH_SEGM_SKEL_IO_HH
