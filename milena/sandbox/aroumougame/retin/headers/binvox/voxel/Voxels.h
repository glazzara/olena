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
// $Id: Voxels.h,v 1.28 2004/10/19 11:43:27 min Exp min $
//

#ifndef __VOXELS_H
#define __VOXELS_H

#include <fstream>
#include "binvox/voxel/typedefs.h"
#include "binvox/math/common.h"
#include "binvox/math/Vector.h"

static const VoxelType NORMAL = 1;
static const VoxelType EXTREMAL = 2;
static const VoxelType JUNCTION = 3;
static const VoxelType SKELETON = 4;
static const VoxelType MID = 5;
static const VoxelType SIX_INTERIOR = 6;
static const VoxelType WITNESS = 7;
static const VoxelType NON_WITNESS = 8;
static const VoxelType BOUNDARY = 9;
static const VoxelType SIMPLE = 10;
static const VoxelType REMOVE = 11;
static const VoxelType LOCAL_MAX = 12;
static const VoxelType INTERIOR = 13;
static const VoxelType LEFT_SIDE = 14;
static const VoxelType RIGHT_SIDE = 15;
static const VoxelType SEGMENT = 16;

static const VoxelType ANY_TYPE = 127;

static const VoxelType VISITED = 0x80;


namespace binvox {


class Voxels
{

public:

  Voxels();
  Voxels(int width, int height, int depth, int with_types = 1);
  ~Voxels();

  void init(int width, int height, int depth, int with_types = 1);
  void clear();
  void clear(VoxelType value);

  void invert();
  
  int needs_update() { return voxels_changed; }
  
  void get_dimensions(int *width_ptr, int *height_ptr, int *depth_ptr);
  int get_wxh() { return wxh; }
  int get_size() { return size; }

  void compute_bounding_box();
  void get_bounding_box(Vector *bmin_p, Vector *bmax_p) { *bmin_p = bmin, *bmax_p = bmax; }

  // for voxel to mesh coordinates correspondence
  Vector& get_norm_translate() { return norm_translate; }
  void set_norm_translate(Vector new_translate) { norm_translate = new_translate; }
  Float get_norm_scale() { return norm_scale; }
  void set_norm_scale(Float new_factor) { norm_scale = new_factor; }
  
  void remove_internal_voxels();

  
  inline VoxelType operator[](int index) const { return voxels[index]; }
  inline VoxelType& operator[](int index) { return voxels[index]; }

  inline VoxelType get_neighbour(int index, int nb_index) {
    return voxels[index + nb_delta[nb_index]];
  }
  void get_neighbours(int index, VoxelType *nb);
  inline int get_neighbour_index(int index, int nb_index) {
    return index + nb_delta[nb_index];
  }
  int get_nr_neighbours(int index);
  int get_nr_plane_neighbours(int plane_index, int index);
  int get_nr_6adj_octant_neighbours(int octant_index, int index);
  int get_nr_6adj_bg_cc(int index);
  
  
  void init_deltas();
  
  void get_face_neighbours(int index, VoxelType *nb);
  inline int get_face_neighbour_index(int voxel_index, int index) {
    return voxel_index + face_delta[index]; }
  void get_edge_neighbours(int index, VoxelType *nb);
  inline int get_edge_neighbour_index(int voxel_index, int index) {
    return voxel_index + edge_delta[index]; }
  void get_vertex_neighbours(int index, VoxelType *nb);
  inline int get_vertex_neighbour_index(int voxel_index, int index) {
    return voxel_index + vertex_delta[index]; }
  int has_empty(VoxelType *nb, int size);

  static Vector compute_coordinates(int voxel_index);
  static void compute_int_coordinates(int voxel_index, int *coords);
  static int squared_distance(int index1, int index2);
  
  // for voxelization
  void vote(byte *screen_buffer, int axis, int direction, int slice);
  void process_votes();

  int get_index(int x, int y, int z);
  int get_index(Float x, Float y, Float z) { return get_index((int) x, (int) y, (int) z); }
  
  void store_copy();
  void swap_copy();
  void and_with_copy();
  
  void dilate();
  
  void init_types();
  void init_types(VoxelType value);
  inline int get_type(int index) { return types[index] & 0x7f; }
  void set_type(int index, int new_value) {
    types[index] &= 0x80;
    types[index] += new_value;
  }
  void print_types();
  void clear_visited();
  int get_visited(int index) { return (types[index] & 0x80); }
  void set_visited(int index) { types[index] |= 0x80; }
  void reset_visited(int index) { types[index] &= 0x7f; }


  void update_voxel_refs();

  friend ostream& operator<<(ostream& out_stream, Voxels& voxels);

  VoxelType *get_voxels_p() { return voxels; }

  
private:

  int voxels_changed;
  
  // dimensions
  int width, height, depth, wxh;
  int size;
  Vector bmin, bmax;

  // both used for voxel-mesh correspondence transform
  Vector norm_translate;
  Float norm_scale;  
  
  int clear_value;
  VoxelType *types;
  VoxelType *voxels;
  VoxelType *voxels_copy;
  VoxelType *types_copy;

  int voxels_in_xoy_slice(int height_index, int front, int back, int left, int right);
  int voxels_in_xoz_slice(int width_index, int top, int bottom, int front, int back);
  int voxels_in_yoz_slice(int depth_index, int top, int bottom, int left, int right);

  
  //
  // neighbour support
  //
  int face_delta[6];
  int edge_delta[12];
  int vertex_delta[8];
  void init_face_edge_vertex_deltas();
  int nb_delta[27];
  void init_nb_deltas();
  int are_neighbours(int index1, int index2);

  int plane_delta[3][8];
  void init_plane_deltas();

  
};  // Voxels class
}

#endif

