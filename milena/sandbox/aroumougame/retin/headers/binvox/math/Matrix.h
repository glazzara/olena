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
// $Id: Matrix.h,v 1.3 2001/03/19 19:30:09 min Exp min $
//

#ifndef __MATRIX_H
#define __MATRIX_H

#include <iostream>
#include "binvox/math/common.h"



using namespace std;


namespace binvox {
class Vector;

class Matrix {

public:

  Matrix(int dim = 3);
  Matrix(Float a1, Float a2, Float a3,
	 Float b1, Float b2, Float b3,
	 Float c1, Float c2, Float c3);
  Matrix(Float a1, Float a2, Float a3, Float a4,
	 Float b1, Float b2, Float b3, Float b4,
	 Float c1, Float c2, Float c3, Float c4,
	 Float d1, Float d2, Float d3, Float d4);
  Matrix(Vector u, Vector v, Vector n);
  ~Matrix() {};


  friend Vector operator *(Vector& T, Matrix& M);
  friend Vector operator *(const Matrix& M, const Vector& T);
  friend Matrix operator *(const Matrix& M1, const Matrix& M2);
  friend Matrix operator *(Matrix& M, Float& f);
  friend Matrix operator +(Matrix& M1, Matrix& M2);
  friend Matrix operator -(Matrix& M1, Matrix& M2);
  
  void transpose();
  void set(Float a1, Float a2, Float a3,
	   Float b1, Float b2, Float b3,
	   Float c1, Float c2, Float c3);
  void set(Vector u, Vector v, Vector n);
  void set(Float a1, Float a2, Float a3, Float a4,
	   Float b1, Float b2, Float b3, Float b4,
	   Float c1, Float c2, Float c3, Float c4,
	   Float d1, Float d2, Float d3, Float d4);
  Float get(int index1, int index2);
  Float *get_elm_p() { return elm; }
  
  // getting value
  Float operator[](int index) const { return elm[index]; };
  // setting value
  Float& operator[](int index) { return elm[index]; };

  void identity();
  void clear();

  void translate(Float tx, Float ty, Float tz);
  void translate(Vector T);
  void rotate(Float ax, Float ay, Float az);
  void rotate(Vector axis, Float angle);
  void scale(Float sx, Float sy, Float sz);
  void scale(Vector S);
  void mirror(int xy, int xz, int yz);

  // output operator
  friend ostream& operator <<(ostream& out_stream, const Matrix& M);

  
private:

  int dim;
  int size;
  Float elm[16];

};  // class Matrix
}
#endif
