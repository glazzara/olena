//
// binvox, a binary 3D mesh voxelizer
// Copyright (c) 2004-2007 by Patrick Min, patrick.n.min "at" gmail "dot" com
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// 
//
// $Id: Voxels.cc,v 1.31 2004/10/19 11:43:26 min Exp min $
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "binvox/voxel/Voxels.h"
#include "binvox/voxel/VoxelRef.h"
#include "binvox/math/MyRandom.h"
using namespace binvox;

#define DEBUG(x)
#define DEBUG_SIMPLE(x)
#define DEBUG_FIND_LINE(x)

static Voxels *cur_voxels = 0;

using namespace std;





Voxels::Voxels()
{
  voxels = 0;
  voxels_copy = 0;
  types = 0;
  types_copy = 0;
  
  cur_voxels = this;  // for static function "compute_coordinates"

  voxels_changed = 0;

}  // default constructor


  
Voxels::Voxels(int width, int height, int depth, int with_types)
{
  voxels = 0;
  voxels_copy = 0;
  types = 0;
  types_copy = 0;
  
  //  cout << "Voxels constructor(" << width << ", " << height << ", " << depth << ")" << endl;

  init(width, height, depth, with_types);

  cur_voxels = this;  // for static function "compute_coordinates"

  voxels_changed = 0;

}  // constructor



void
Voxels::init(int width, int height, int depth, int with_types)
{
  cout << "Voxels::init(" << width << ", " << height << ", " << depth << ")" << endl;
  
  this->width = width;
  this->height = height;
  this->depth = depth;
  wxh = width * height;
  size = width * height * depth;

  //
  // allocate memory for voxels and types
  //
  if (voxels) delete[] voxels;
  voxels = new VoxelType[size];
  assert(voxels);
  if (voxels_copy) delete[] voxels_copy;
  voxels_copy = 0;
  if (types_copy) delete[] types_copy;
  types_copy = 0;
  if (types) delete[] types;
  if (with_types) {
    types = new VoxelType[size];
    assert(types);
  }
  else
    types = 0;
  
  clear();

  init_deltas();

  update_voxel_refs();
  
}  // Voxels::init



Voxels::~Voxels()
{
  if (voxels) delete[] voxels;
  if (voxels_copy) delete[] voxels_copy;
  if (types_copy) delete[] types_copy;
  if (types) delete[] types;
  
}  // destructor



void
Voxels::clear(VoxelType value)
{
  memset(voxels, value, size * sizeof(VoxelType));
  if (types) memset(types, 0, size * sizeof(VoxelType));

}  // Voxels::clear(value)



void
Voxels::clear()
{
  clear(0);

}  // Voxels::clear



void
Voxels::get_dimensions(int *width_ptr, int *height_ptr, int *depth_ptr)
{
  *width_ptr = width;
  *height_ptr = height;
  *depth_ptr = depth;

}  // Voxels::get_dimensions



void
Voxels::compute_bounding_box()
{
  cout << "Voxels::compute_bounding_box" << endl;

  bmin.set(1e6, 1e6, 1e6);
  bmax.set(-1e6, -1e6, -1e6);
  
  int index = 0;
  for(int i=0; i < depth; i++) {
    float x = (float)(i + 0.5) / depth;
    for(int j = 0; j < height; j++) {
      float z = (float)(j + 0.5) / height;
      //      index = i * voxels.wxh + j * voxels.width;
      for(int k=0; k < width; k++) {
	float y = (float)(k + 0.5) / width;
	if (voxels[index]) {
	  if (x < bmin[X]) bmin[X] = x;
	  if (x > bmax[X]) bmax[X] = x;
	  if (y < bmin[Y]) bmin[Y] = y;
	  if (y > bmax[Y]) bmax[Y] = y;
	  if (z < bmin[Z]) bmin[Z] = z;
	  if (z > bmax[Z]) bmax[Z] = z;
	}
	index++;
      }  // for
    }  // for
  }  // for

  cout << "  bounding box: " << bmin << " - " << bmax << endl;
  
}  // Voxels::compute_bounding_box



int
Voxels::get_index(int x, int y, int z)
{
  int index = x * wxh + z * width + y;
  //    cout << "Voxels::get_index(" << x << ", " << y << ", " << z << ")"
  //         << " = " << index << endl;

  return index;

}  // Voxels::get_index



void
Voxels::get_neighbours(int index, VoxelType *nb)
{
  for(int i=0; i < 27; i++) {
    nb[i] = get_neighbour(index, i);
  }  // for

}  // Voxels::get_neighbours



void
Voxels::init_deltas()
{
  init_face_edge_vertex_deltas();
  init_nb_deltas();
  init_plane_deltas();

}  // Voxels::init_deltas



void
Voxels::update_voxel_refs()
{
  voxels_changed = 1;
  VoxelRef::check_update();
  voxels_changed = 0;

}  // Voxels::update_voxel_refs



void
Voxels::vote(byte *screen_buffer, int axis, int direction, int slice)
{
  // code assumes all dimensions are equal
  
  int i, j;
  int i_reversed;
  int *i_ptr;
  int *x_ptr, *y_ptr, *z_ptr;

  if (axis == X)
      x_ptr = &slice, y_ptr = &i, z_ptr = &j;
  else {
    if (axis == Y)
      x_ptr = &i, y_ptr = &slice, z_ptr = &j;
    else {
      if (axis == Z)
	x_ptr = &i, y_ptr = &j, z_ptr = &slice;
      else {
	cout << "Voxels::vote error: unknown axis " << axis << endl;
	return;
      }
    }
  }  // else

  int i_start, i_end, i_delta;
  if (direction > 0) {
    i_start = 0, i_end = width, i_delta = 1;
    i_ptr = &i;
  }
  else {
    //    cout << "reversing direction" << endl;
    i_start = width - 1, i_end = -1, i_delta = -1;
    i_ptr = &i_reversed;
  }

  for(i=i_start; i != i_end; i += i_delta) {
    i_reversed = width - 1 - i;
    for(j=0; j < height; j++) {
      int index = j * width + (*i_ptr);
      if (screen_buffer[index] % 2) {
	//	cout << (int) screen_buffer[index] << " ";
	// increase vote for appropriate voxel
	int voxel_index = (*x_ptr) * wxh + (*z_ptr) * width + (*y_ptr);
	voxels[voxel_index]++;
      }
      
    }  // for
  }  // for
  //  cout << endl;
  
}  // Voxels::vote



void
Voxels::process_votes()
{
  cout << "Voxels::process_votes" << endl;
  
  for(int i=0; i < size; i++) {
    if (voxels[i] > 2) voxels[i] = 1;
    else voxels[i] = 0;
  }

}  // Voxels::process_votes



void
Voxels::store_copy()
{
  //  cout << "Voxels::store_copy" << endl;
  
  if (voxels_copy) delete[] voxels_copy;
  voxels_copy = new VoxelType[size];
  assert(voxels_copy);
  if (types_copy) delete[] types_copy;
  types_copy = new VoxelType[size];
  assert(types_copy);

  for(int i=0; i < size; i++) {
    voxels_copy[i] = voxels[i];
    types_copy[i] = types[i];
  }  // for
  
}  // Voxels::store_copy



void
Voxels::swap_copy()
{
  //  cout << "Voxels::swap_copy" << endl;

  if (!voxels_copy || !types_copy) {
    cout << "Voxels::swap_copy error: no copy available" << endl;
    return;
  }

  VoxelType *temp = voxels;
  voxels = voxels_copy;
  voxels_copy = temp;

  temp = types;
  types = types_copy;
  types_copy = temp;
  
}  // Voxels::swap_copy

  

void
Voxels::and_with_copy()
{
  if (!voxels_copy) {
    cout << "Voxels::and_with_copy error: no copy available" << endl;
    return;
  }

  for(int i=0; i < size; i++) {
    voxels[i] = voxels[i] & voxels_copy[i];
  }

}  // Voxels::and_with_copy



int
Voxels::voxels_in_xoy_slice(int height_index, int front, int back, int left, int right)
{
  //   cout << "voxels_in_xoy_slice(" << height_index << ", " << front << ", " << back
  //        << ", " << left << ", " << right << ")" << endl;
  
  int nr_voxels = 0;
  
  for(int i=back; i < front; i++) {
    int index = i * wxh + height_index * width + left;
    for(int j=left; j < right; j++) {
      nr_voxels += voxels[index];
      index++;
    }
  }

  return nr_voxels;
      
}  // Voxels::voxels_in_xoy_slice



int
Voxels::voxels_in_xoz_slice(int width_index, int top, int bottom, int front, int back)
{
  int nr_voxels = 0;

  for(int i=back; i < front; i++) {
    int index = i * wxh + bottom * depth + width_index;
    for(int j=bottom; j < top; j++) {
      nr_voxels += voxels[index];
      index += depth;
    }
  }

  return nr_voxels;
  
}  // Voxels::voxels_in_xoz_slice



int
Voxels::voxels_in_yoz_slice(int depth_index, int top, int bottom, int left, int right)
{
  int nr_voxels = 0;

  for(int i=left; i < right; i++) {
    int index = depth_index * wxh + bottom * width + i;
    for(int j=bottom; j < top; j++) {
      nr_voxels += voxels[index];
      index += width;
    }
  }

  return nr_voxels;

}  // Voxels::voxels_in_yoz_slice



//
// initialize delta values for face_delta, edge_delta, vertex_delta arrays
//
void
Voxels::init_face_edge_vertex_deltas()
{
  face_delta[0] = -wxh;
  face_delta[1] = wxh;
  face_delta[2] = -width;
  face_delta[3] = width;
  face_delta[4] = -1;
  face_delta[5] = 1;

  edge_delta[0] = -wxh - width;
  edge_delta[1] = -wxh + width;
  edge_delta[2] = wxh - width;
  edge_delta[3] = wxh + width;
  edge_delta[4] = -wxh - 1;
  edge_delta[5] = -wxh + 1;
  edge_delta[6] = wxh - 1;
  edge_delta[7] = wxh + 1;
  edge_delta[8] = -width - 1;
  edge_delta[9] = -width + 1;
  edge_delta[10] = width - 1;
  edge_delta[11] = width + 1;

  vertex_delta[0] = -wxh - width - 1;
  vertex_delta[1] = -wxh - width + 1;
  vertex_delta[2] = -wxh + width - 1;
  vertex_delta[3] = -wxh + width + 1;
  vertex_delta[4] = wxh - width - 1;
  vertex_delta[5] = wxh - width + 1;
  vertex_delta[6] = wxh + width - 1;
  vertex_delta[7] = wxh + width + 1;

}  // Voxels::init_deltas

  
  
void
Voxels::get_face_neighbours(int index, VoxelType *nb)
{
  // one step in x, y or z
  for(int i=0; i < 6; i++) {
    int nb_index = index + face_delta[i];
    if ((nb_index >= 0) && (nb_index < size))
      nb[i] = voxels[nb_index];
    else
      nb[i] = 0;
  }

}  // Voxels::get_face_neighbours(index, nb)

		

void
Voxels::get_edge_neighbours(int index, VoxelType *nb)
{
  // one step in (x and z) or (x and y) or (y and z) = 3 * 4 = 12 combinations
  // x and z
  for(int i=0; i < 12; i++) {
    int nb_index = index + edge_delta[i];
    if ((nb_index >= 0) && (nb_index < size))
      nb[i] = voxels[nb_index];
    else
      nb[i] = 0;
  }
    
}  // Voxels::get_edge_neighbours(index, nb)



void
Voxels::get_vertex_neighbours(int index, VoxelType *nb)
{
  // one step in (x and y and z) = 8 combinations
  for(int i=0; i < 8; i++) {
    int nb_index = index + vertex_delta[i];
    if ((nb_index >= 0) && (nb_index < size))
      nb[i] = voxels[nb_index];
    else
      nb[i] = 0;
  }
  
}  // Voxels::get_vertex_neighbours



//
// returns true if there is at least one empty voxel in nb array
//
int
Voxels::has_empty(VoxelType *nb, int size)
{
  for(int i=0; i < size; i++) {
    if (!nb[i]) return 1;
  }

  return 0;

}  // Voxels::has_empty

  

int
Voxels::are_neighbours(int index1, int index2)
{
  int z1 = index1 / 9;
  int z2 = index2 / 9;
  int x1 = (index1 - 9 * z1) / 3;
  int x2 = (index2 - 9 * z2) / 3;
  int y1 = index1 - 3 * x1 - 9 * z1;
  int y2 = index2 - 3 * x2 - 9 * z2;

  int dx = x2 - x1;
  int dy = y2 - y1;
  int dz = z2 - z1;
  if (dx < 0) dx = -dx;
  if (dy < 0) dy = -dy;
  if (dz < 0) dz = -dz;

  int neighbours = ((dx <= 1) && (dy <= 1) && (dz <= 1));

  return neighbours;

}  // Voxels::are_neighbours


  
void
Voxels::init_nb_deltas()
{
  nb_delta[0] = -wxh - width - 1;
  nb_delta[1] = -wxh - width;
  nb_delta[2] = -wxh - width + 1;
  nb_delta[3] = -width - 1;
  nb_delta[4] = -width;
  nb_delta[5] = -width + 1;
  nb_delta[6] = wxh - width - 1;
  nb_delta[7] = wxh - width;
  nb_delta[8] = wxh - width + 1;

  nb_delta[9] = -wxh - 1;
  nb_delta[10] = -wxh;
  nb_delta[11] = -wxh + 1;
  nb_delta[12] = -1;
  nb_delta[13] = 0;
  nb_delta[14] = 1;
  nb_delta[15] = wxh - 1;
  nb_delta[16] = wxh;
  nb_delta[17] = wxh + 1;

  nb_delta[18] = -wxh + width - 1;
  nb_delta[19] = -wxh + width;
  nb_delta[20] = -wxh + width + 1;
  nb_delta[21] = width - 1;
  nb_delta[22] = width;
  nb_delta[23] = width + 1;
  nb_delta[24] = wxh + width - 1;
  nb_delta[25] = wxh + width;
  nb_delta[26] = wxh + width + 1;
    
}  // Voxels::init_nb_deltas



int plane_delta[3][8];

void
Voxels::init_plane_deltas()
{
  plane_delta[0][0] = -width - 1;
  plane_delta[0][1] = -1;
  plane_delta[0][2] = width - 1;
  plane_delta[0][3] = width;
  plane_delta[0][4] = width + 1;
  plane_delta[0][5] = 1;
  plane_delta[0][6] = -width + 1;
  plane_delta[0][7] = -width;

  plane_delta[1][0] = -wxh - width;
  plane_delta[1][1] = -wxh;
  plane_delta[1][2] = -wxh + width;
  plane_delta[1][3] = width;
  plane_delta[1][4] = wxh + width; 
  plane_delta[1][5] = wxh;
  plane_delta[1][6] = wxh - width;
  plane_delta[1][7] = -width;

  plane_delta[2][0] = -wxh - 1;
  plane_delta[2][1] = -wxh;
  plane_delta[2][2] = -wxh + 1;
  plane_delta[2][3] = 1;
  plane_delta[2][4] = wxh + 1;
  plane_delta[2][5] = wxh;
  plane_delta[2][6] = wxh - 1;
  plane_delta[2][7] = -1;

}  // Voxels::init_plane_deltas



int
Voxels::get_nr_plane_neighbours(int plane_index, int index)
{
  int nr_neighbours = 0;
  for(int i=0; i < 8; i++) {
    int nb_index = index + plane_delta[plane_index][i];
    if (voxels[nb_index]) nr_neighbours++;
  }

  return nr_neighbours;

}  // Voxels::get_nr_plane_neighbours



void
Voxels::clear_visited()
{
  // clear bit 7 of types array
  for(int i=0; i < size; i++) {
    types[i] &= 0x7f;
  }

}  // Voxels::clear_visited



static const int DILATE = 3;

void
Voxels::dilate()
{
  cout << "Voxels::dilate" << endl;
  
  init_deltas();

  for(int i=1; i < (depth - 1); i++) {

    for(int j=1; j < (height - 1); j++) {
      int index = i * wxh + j * width + 1;
      for(int k=1; k < (width - 1); k++) {
	if (voxels[index] == 1) {
	  for(int m=0; m < 6; m++) {
	    int nb_index = index + face_delta[m];
	    if ((nb_index < 0) || (nb_index >= size)) {
	      cout << "Voxels::dilate error: nb_index out of range" << endl;
	      return;
	    }
	    if (!voxels[nb_index]) voxels[nb_index] = DILATE;
	  }
	  for(int m=0; m < 12; m++) {
	    int nb_index = index + edge_delta[m];
	    if ((nb_index < 0) || (nb_index >= size)) {
	      cout << "Voxels::dilate error: nb_index out of range" << endl;
	      return;
	    }
	    if (!voxels[nb_index]) voxels[nb_index] = DILATE;
	  }
	  for(int m=0; m < 8; m++) {
	    int nb_index = index + vertex_delta[m];
	    if ((nb_index < 0) || (nb_index >= size)) {
	      cout << "Voxels::dilate error: nb_index out of range" << endl;
	      return;
	    }
	    if (!voxels[nb_index]) voxels[nb_index] = DILATE;
	  }
	}  // if voxel == 1
	index++;
      }
    }
  }  // for

  for(int i=0; i < size; i++) if (voxels[i] == DILATE) {
    voxels[i] = 1;
    types[i] = NORMAL;
  }

}  // Voxels::dilate



void
Voxels::init_types(VoxelType value)
{
  if (!types) return;
  
  for(int i=0; i < size; i++) {
    if (voxels[i])
      types[i] = value;
    else
      types[i] = 0;
  }

}  // Voxels::init_types with parameter



void
Voxels::init_types()
{
  init_types(NORMAL);
  
}  // Voxels::init_types



int
Voxels::get_nr_neighbours(int index)
{
  int nr_neighbours = 0;
  for(int j=0; j < 27; j++) {
    if (j == 13) continue;
    if (get_neighbour(index, j)) nr_neighbours++;
  }

  return nr_neighbours;

}  // Voxels::get_nr_neighbours


  
void
Voxels::compute_int_coordinates(int voxel_index, int *coords)
{
  int wxh = cur_voxels->get_wxh();
  int width, height, depth;
  cur_voxels->get_dimensions(&width, &height, &depth);
  
  coords[0] = (voxel_index / wxh);
  coords[2] = (voxel_index - wxh * coords[0]) / width;
  coords[1] = voxel_index - wxh * coords[0] - width * coords[2];

}  // Voxels::compute_int_coordinates



int
Voxels::squared_distance(int index1, int index2)
{
  int v[3];
  int w[3];
  compute_int_coordinates(index1, v);
  compute_int_coordinates(index2, w);

  int dx = v[0] - w[0];
  int dy = v[1] - w[1];
  int dz = v[2] - w[2];
  
  return (dx * dx + dy * dy + dz * dz);

}  // Voxels::squared_distance (static)

  
  
Vector
Voxels::compute_coordinates(int voxel_index)
{
  int width, height, depth;
  cur_voxels->get_dimensions(&width, &height, &depth);
  
  // first compute coordinates as integers
  int int_coords[3];
  compute_int_coordinates(voxel_index, int_coords);
  
  float x = (float) int_coords[0] / depth;
  float y = (float) int_coords[1] / width;
  float z = (float) int_coords[2] / height;

  Vector p;
  p.set(x, y, z);
  
  //  cout << "Voxels::compute_coordinates, xyz: " << x << ", " << y << ", " << z << endl;

  return p;
  
}  // Voxels::compute_coordinates

namespace binvox {

ostream& operator<<(ostream& out_stream, Voxels& voxels)
{
  cout << "Voxels operator<<" << endl;
  int index = 0;
  for(int i=0; i < voxels.depth; i++) {
    for(int j = voxels.height - 1; j >= 0; j--) {
      out_stream << setw(2) << i << ", " << setw(2) << j << ": ";
      index = i * voxels.wxh + j * voxels.width;
      for(int k=0; k < voxels.width; k++) {
	// because VoxelType is unsigned char, 2nd comparison is always true
	if (voxels[index]) // && (voxels[index] != MaxInt))
	  out_stream << (voxels[index] & 0x7f) << " ";
	else
	  out_stream << ". ";
	index++;
      }
      out_stream << endl;
    }
  }

  cout << endl << "-----" << endl << endl;
  
  index = 0;
  for(int i=0; i < voxels.depth; i++) {
    for(int j = voxels.height - 1; j >= 0; j--) {
      out_stream << setw(2) << i << ", " << setw(2) << j << ": ";
      index = i * voxels.wxh + j * voxels.width;
      for(int k=0; k < voxels.width; k++) {
	// because VoxelType is unsigned char, 2nd comparison is always true
	if (voxels[index]) // && (voxels[index] != MaxInt))
	  out_stream << voxels.get_type(index) << " ";
	else
	  out_stream << ". ";
	index++;
      }
      out_stream << endl;
    }
  }

  return out_stream;
  
}  // operator<<

}

static const int octants[8][7] = {
  {4, 10, 12, 1, 3, 9, 0},
  {1, 2, 4, 5, 10, 11, 14},
  {4, 5, 7, 8, 14, 16, 17},
  {3, 4, 6, 7, 12, 15, 16},
  {9, 10, 12, 18, 19, 21, 22},
  {10, 11, 14, 19, 20, 22, 23},
  {14, 16, 17, 22, 23, 25, 26},
  {12, 15, 16, 21, 22, 24, 25}
};

static const int face_octants[8][3] = {
  {0, 2, 4},
  {0, 2, 5},
  {1, 2, 5},
  {1, 2, 4},
  {0, 3, 4},
  {0, 3, 5},
  {1, 3, 5},
  {1, 3, 4}
};


int
Voxels::get_nr_6adj_octant_neighbours(int octant_index, int index)
{
  VoxelType nb[6];
  get_face_neighbours(index, nb);

  int nr_neighbours = 0;
  for(int i=0; i < 3; i++) {  // 3 because checking face neighbours only
    if (voxels[nb[face_octants[octant_index][i]]]) nr_neighbours++;
  }

  return nr_neighbours;

}  // Voxels::get_nr_6adj_octant_neighbours



static int local_6_nb_18_nbh[27][4] = {
  {-1, -1, -1, -1},  // 0
  {4, 10, -1, -1},
  {-1, -1, -1, -1},
  {4, 12, -1, -1},
  {1, 3, 5, 7},   // 4
  {4, 14, -1, -1},
  {-1, -1, -1, -1},
  {4, 16, -1, -1},
  {-1, -1, -1, -1},  // 8
  {10, 12, -1, -1},
  {1, 9, 11, 19},
  {10, 14, -1, -1},
  {3, 9, 15, 21},  // 12
  {-1, -1, -1, -1},
  {5, 11, 17, 23},
  {12, 16, -1, -1},
  {7, 15, 17, 25}, // 16
  {14, 16, -1, -1},
  {-1, -1, -1, -1},
  {10, 22, -1, -1},
  {-1, -1, -1, -1}, // 20
  {12, 22, -1, -1},
  {19, 21, 23, 25},
  {14, 22, -1, -1},
  {-1, -1, -1, -1}, // 24
  {16, 22, -1, -1},
  {-1, -1, -1, -1}
};



int
Voxels::get_nr_6adj_bg_cc(int index)
{
  VoxelType nb[27];
  get_neighbours(index, nb);

  int visited[27];
  for(int i=0; i < 27; i++) visited[i] = 0;

  int cur_index;
  
  static int face_indices[6] = {4, 10, 12, 14, 16, 22};
  int found = 0;
  for(int i=0; (i < 6) && !found; i++) {
    int fi = face_indices[i];
    if (!nb[fi]) {
      found = 1;
      cur_index = fi;
    }
  }  // for

  if (!found) return 0;  // no 6 connected background components

  // from there, travel to all other voxels in 18 neighbourhood, and mark
  vector<int> to_visit;
  to_visit.push_back(cur_index);
  
  while (!to_visit.empty()) {
    cur_index = to_visit[to_visit.size() - 1];
    visited[cur_index] = 1;
    to_visit.pop_back();
    
    // get neighbours of cur_index
    for(int i=0; (i < 4) && (local_6_nb_18_nbh[cur_index][i] != -1); i++) {
      int nb_index = local_6_nb_18_nbh[cur_index][i];
      if (!visited[nb_index] && !nb[nb_index]) {
	to_visit.push_back(nb_index);
      }
    }  // visit neighbours of cur_index
  }  // while

  // if unmarked unvisited background voxel: more than 1 6-connected component
  for(int i=0; i < 6; i++) {
    int fi = face_indices[i];
    if (!nb[fi] && !visited[fi]) {
      DEBUG_SIMPLE(cout << "> 1 bg comp" << endl);
      return 2;
    }
  }
    
  return 1;

}  // Voxels::get_nr_6_adj_bg_cc



void
Voxels::invert()
{
  cout << "Voxels::invert" << endl;
  for(int i=0; i < size; i++) voxels[i] = 1 - voxels[i];
  
}  // Voxels::invert



void
Voxels::remove_internal_voxels()
{
  cout << "Voxels::remove_internal_voxels" << endl;

  vector<int> to_be_deleted;
  VoxelType nb[6];
  for(int i=0; i < size; i++) {
    if (voxels[i]) {
      get_face_neighbours(i, nb);
      int boundary = 0;
      for(int j=0; (j < 6) && !boundary; j++) {
	if (nb[j] == 0) boundary = 1;
      }
      if (!boundary) to_be_deleted.push_back(i);
    }  // if
  }  // for

  cout << "  removing " << to_be_deleted.size() << " voxels" << endl;
  for(int i=0; i < to_be_deleted.size(); i++) {
    voxels[to_be_deleted[i]] = 0;
  }
  
}  // Voxels::remove_internal_voxels


