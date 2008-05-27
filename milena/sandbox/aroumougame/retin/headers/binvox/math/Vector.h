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
// $Id: Vector.h,v 1.3 2001/03/19 19:30:09 min Exp min $
//

#ifndef __VECTOR_H
#define __VECTOR_H

#include <iostream>
#include "binvox/math/common.h"
#include "binvox/math/Matrix.h"

const int VECTOR_SIZE = 3;
const int MAX_SIZE = 4;
const int X = 0;
const int Y = 1;
const int Z = 2;
const int W = 3;

using namespace std;


namespace binvox {


class Vector {

public:

  Vector(Float a = 0.0, Float b = 0.0, Float c = 0.0);
  Vector(Float a, Float b, Float c, Float d);
  ~Vector() {};

  void set(Float a, Float b, Float c = 0.0);
  void set(Float a, Float b, Float c, Float d);


  //
  // operators
  //
  
  // dot product
  Float operator *(const Vector& A);
  
  // scalar multiply
  friend Vector operator *(const Vector& A, const Float scalar);
  friend Vector operator *(const Float scalar, const Vector& A);

  // cross product
  Vector operator ^(const Vector& A);

  // scalar divide
  friend Vector operator /(const Vector& A, const Float scalar);

  // vector add
  Vector operator +(const Vector& A);

  // vector subtract
  Vector operator -(Vector& A);

  // vector negation
  Vector operator -();

  // test for equality
  int operator ==(const Vector& B);

  // test for inequality
  int operator !=(const Vector& B);

  // output operator
  friend ostream& operator<<(ostream& out_stream, const Vector& T);

  // input
  friend istream& operator>>(istream& in_stream, Vector& T);

  // getting value at index X, Y or Z
  inline Float operator[](int index) const { return elm[index]; };

  // setting value at index X, Y or Z
  inline Float& operator[](int index) { return elm[index]; };

  //
  // support functions
  //

  void rotate(Vector& axis, Float angle);
  void scale(Vector& scale);
  
  Float sum();
  Float squared_length();
  Float length();
  Float squared_distance(Vector& V);
  Float distance(Vector& V);
  
  void normalize();
  void clamp(Float min, Float max);
  Vector vec_abs();
  Vector round();
  
  friend int ccw(Vector& A, Vector& B, Vector& C);
  friend int intersect(Vector& p1, Vector& p2, Vector& q1, Vector& q2);
  Matrix star();
  Matrix outer_product();
  int max_dimension();
  
  void random_in_sphere();
  void random_on_sphere();
  void uniform(Float min, Float max);
  
  void project(Vector normal);

  void flip_y_z();

  
private:

  Float elm[MAX_SIZE];

  
};  // Vector
}


#endif



