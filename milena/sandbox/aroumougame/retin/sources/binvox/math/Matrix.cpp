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
// $Id: Matrix.cc,v 1.3 2001/03/19 19:30:07 min Exp min $
//

#include <math.h>
#include <assert.h>
#include "binvox/math/Matrix.h"
#include "binvox/math/Vector.h"

using namespace std;

using namespace binvox;



Matrix::Matrix(int dim)
{
  this->dim = dim;
  size = dim * dim;
  identity();

}  // default constructor



Matrix::Matrix(Float a1, Float a2, Float a3,
               Float b1, Float b2, Float b3,
               Float c1, Float c2, Float c3)
{
  set(a1, a2, a3, b1, b2, b3, c1, c2, c3);

}  // constructor with initial values



Matrix::Matrix(Float a1, Float a2, Float a3, Float a4,
	       Float b1, Float b2, Float b3, Float b4,
	       Float c1, Float c2, Float c3, Float c4,
	       Float d1, Float d2, Float d3, Float d4)
{
  set(a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4);
  
}  // constructor with 16 initial values



Matrix::Matrix(Vector u, Vector v, Vector n)
{
  set(u, v, n);

}  // constructor with three vectors



void
Matrix::set(Float a1, Float a2, Float a3,
	    Float b1, Float b2, Float b3,
	    Float c1, Float c2, Float c3)
{
  set(a1, a2, a3, 0.0,
      b1, b2, b3, 0.0,
      c1, c2, c3, 0.0,
      0.0, 0.0, 0.0, 1.0);
  
}  // Matrix::set




void
Matrix::set(Float a1, Float a2, Float a3, Float a4,
	    Float b1, Float b2, Float b3, Float b4,
	    Float c1, Float c2, Float c3, Float c4,
	    Float d1, Float d2, Float d3, Float d4)
{
  elm[0] = a1;
  elm[1] = a2;
  elm[2] = a3;
  elm[3] = a4;
  
  elm[4] = b1;
  elm[5] = b2;
  elm[6] = b3;
  elm[7] = b4;
  
  elm[8] = c1;
  elm[9] = c2;
  elm[10] = c3;
  elm[11] = c4;

  elm[12] = d1;
  elm[13] = d2;
  elm[14] = d3;
  elm[15] = d4;
  
}  // Matrix::set




void
Matrix::set(Vector u, Vector v, Vector n)
{
  set(u[X], v[X], n[X], u[Y], v[Y], n[Y], u[Z], v[Z], n[Z]);
  
}  // Matrix::set



Float
Matrix::get(int index1, int index2)
{
  return elm[index1 * dim + index2];

}  // Matrix::get



void
Matrix::identity()
{
  clear();
  elm[0] = elm[5] = elm[10] = elm[15] = 1.0;

}  // Matrix::identity



void
Matrix::clear()
{
  for(int i=0; i < 16; i++) elm[i] = 0;

}  // Matrix::clear

namespace binvox {


Vector
operator *(Vector& T, Matrix& M)
{
  Vector result;

  result[0] =
    T[0] * M.elm[0] +
    T[1] * M.elm[4] +
    T[2] * M.elm[8] +
    T[3] * M.elm[12];

  result[1] =
    T[0] * M.elm[1] +
    T[1] * M.elm[5] +
    T[2] * M.elm[9] +
    T[3] * M.elm[13];

  result[2] =
    T[0] * M.elm[2] +
    T[1] * M.elm[6] +
    T[2] * M.elm[10] +
    T[3] * M.elm[14];

  result[3] =
    T[0] * M.elm[3] +
    T[1] * M.elm[7] +
    T[2] * M.elm[11] +
    T[3] * M.elm[15];

  return result;
  
}  // operator * for Vector * Matrix



Vector
operator *(const Matrix& M, const Vector& T)
{
  Vector result;

  result[0] =
    T[0] * M.elm[0] +
    T[1] * M.elm[1] +
    T[2] * M.elm[2] +
    T[3] * M.elm[3];

  result[1] =
    T[0] * M.elm[4] +
    T[1] * M.elm[5] +
    T[2] * M.elm[6] +
    T[3] * M.elm[7];

  result[2] =
    T[0] * M.elm[8] +
    T[1] * M.elm[9] +
    T[2] * M.elm[10] +
    T[3] * M.elm[11];

  result[3] =
    T[0] * M.elm[12] +
    T[1] * M.elm[13] +
    T[2] * M.elm[14] +
    T[3] * M.elm[15];

  return result;
  
}  // operator * for Matrix * Vector



Matrix
operator *(const Matrix& M1, const Matrix& M2)
{
  Matrix result;

  //
  // row 1
  //
  result.elm[0] = M1.elm[0] * M2.elm[0] +
    M1.elm[1] * M2.elm[4] +
    M1.elm[2] * M2.elm[8] +
    M1.elm[3] * M2.elm[12];

  result.elm[1] = M1.elm[0] * M2.elm[1] +
    M1.elm[1] * M2.elm[5] +
    M1.elm[2] * M2.elm[9] +
    M1.elm[3] * M2.elm[13];

  result.elm[2] = M1.elm[0] * M2.elm[2] +
    M1.elm[1] * M2.elm[6] +
    M1.elm[2] * M2.elm[10] +
    M1.elm[3] * M2.elm[14];

  result.elm[3] = M1.elm[0] * M2.elm[3] +
    M1.elm[1] * M2.elm[7] +
    M1.elm[2] * M2.elm[11] +
    M1.elm[3] * M2.elm[15];

  //
  // row 2
  //
  result.elm[4] = M1.elm[4] * M2.elm[0] +
    M1.elm[5] * M2.elm[4] +
    M1.elm[6] * M2.elm[8] +
    M1.elm[7] * M2.elm[12];

  result.elm[5] = M1.elm[4] * M2.elm[1] +
    M1.elm[5] * M2.elm[5] +
    M1.elm[6] * M2.elm[9] +
    M1.elm[7] * M2.elm[13];

  result.elm[6] = M1.elm[4] * M2.elm[2] +
    M1.elm[5] * M2.elm[6] +
    M1.elm[6] * M2.elm[10] +
    M1.elm[7] * M2.elm[14];

  result.elm[7] = M1.elm[4] * M2.elm[3] +
    M1.elm[5] * M2.elm[7] +
    M1.elm[6] * M2.elm[11] +
    M1.elm[7] * M2.elm[15];

  //
  // row 3
  //
  result.elm[8] = M1.elm[8] * M2.elm[0] +
    M1.elm[9] * M2.elm[4] +
    M1.elm[10] * M2.elm[8] +
    M1.elm[11] * M2.elm[12];

  result.elm[9] = M1.elm[8] * M2.elm[1] +
    M1.elm[9] * M2.elm[5] +
    M1.elm[10] * M2.elm[9] +
    M1.elm[11] * M2.elm[13];

  result.elm[10] = M1.elm[8] * M2.elm[2] +
    M1.elm[9] * M2.elm[6] +
    M1.elm[10] * M2.elm[10] +
    M1.elm[11] * M2.elm[14];

  result.elm[11] = M1.elm[8] * M2.elm[3] +
    M1.elm[9] * M2.elm[7] +
    M1.elm[10] * M2.elm[11] +
    M1.elm[11] * M2.elm[15];

  //
  // row 4
  //
  result.elm[12] = M1.elm[12] * M2.elm[0] +
    M1.elm[13] * M2.elm[4] +
    M1.elm[14] * M2.elm[8] +
    M1.elm[15] * M2.elm[12];

  result.elm[13] = M1.elm[12] * M2.elm[1] +
    M1.elm[13] * M2.elm[5] +
    M1.elm[14] * M2.elm[9] +
    M1.elm[15] * M2.elm[13];

  result.elm[14] = M1.elm[12] * M2.elm[2] +
    M1.elm[13] * M2.elm[6] +
    M1.elm[14] * M2.elm[10] +
    M1.elm[15] * M2.elm[14];

  result.elm[15] = M1.elm[12] * M2.elm[3] +
    M1.elm[13] * M2.elm[7] +
    M1.elm[14] * M2.elm[11] +
    M1.elm[15] * M2.elm[15];
  
  return result;

}  // operator * for Matrix * Matrix



Matrix operator+(Matrix& M1, Matrix& M2)
{
  Matrix result;

  for(int i=0; i < 16; i++)
    result.elm[i] = M1.elm[i] + M2.elm[i];

  return result;
  
}  // operator +, for Matrix + Matrix



Matrix operator-(Matrix& M1, Matrix& M2)
{
  Matrix result;

  for(int i=0; i < 16; i++)
    result.elm[i] = M1.elm[i] - M2.elm[i];

  return result;
  
}  // operator -, for Matrix - Matrix



Matrix
operator *(Matrix& M, Float& f)
{
  Matrix result;

  for(int i=0; i < 16; i++)
    result.elm[i] = M.elm[i] * f;

  return result;

}  // operator *, for Matrix * scalar

}

void
Matrix::transpose()
{
  Matrix M;

  // note: elm 0, 5, 10 and 15 remain the same (diagonal)

  M.elm[0] = elm[0];
  M.elm[5] = elm[5];
  M.elm[10] = elm[10];
  M.elm[15] = elm[15];

  M.elm[1] = elm[4];
  M.elm[2] = elm[8];
  M.elm[3] = elm[12];
  M.elm[4] = elm[1];
  M.elm[6] = elm[9];
  M.elm[7] = elm[13];
  M.elm[8] = elm[2];
  M.elm[9] = elm[6];
  M.elm[11] = elm[14];
  M.elm[12] = elm[3];
  M.elm[13] = elm[7];
  M.elm[14] = elm[11];

  *this = M;
  
}  // transpose



void
Matrix::translate(Float tx, Float ty, Float tz)
{
  identity();
  elm[3] = tx;
  elm[7] = ty;
  elm[11] = tz;
  
}  // Matrix::translate



void
Matrix::translate(Vector T)
{
  translate(T[X], T[Y], T[Z]);
  
}  // Matrix::translate



void
Matrix::rotate(Float ax, Float ay, Float az)
{
  Matrix RX(1.0, 0.0, 0.0,
	    0.0, cos(ax), -sin(ax),
	    0.0, sin(ax), cos(ax));

  Matrix RY(cos(ay), 0.0, -sin(ay),
	    0.0, 1.0, 0.0,
	    sin(ay), 0.0, cos(ay));

  Matrix RZ(cos(az), -sin(az), 0.0,
	    sin(az), cos(az), 0.0,
	    0.0, 0.0, 1.0);

  *this = RX * RY * RZ;
  
}  // Matrix::rotate



//
// from Graphics Gems I, page 466
//
void
Matrix::rotate(Vector axis, Float angle)
{
  Float x = axis[X];
  Float y = axis[Y];
  Float z = axis[Z];

  Float c = cos(angle);
  Float s = sin(angle);
  Float t = 1 - c;

  set(t * x * x + c,     t * x * y + s * z,  t * x * z - s * y,
      t * x * y - s * z, t * y * y + c,      t * y * z + s * x,
      t * x * z + s * y, t * y * z - s * x,  t * z * z + c);
  
}  // Matrix, rotation around arbitrary axis


    
void
Matrix::scale(Float sx, Float sy, Float sz)
{
  set(sx, 0, 0,
      0, sy, 0,
      0, 0, sz);

}  // Matrix::scale

      

void
Matrix::scale(Vector S)
{
  scale(S[X], S[Y], S[Z]);
  
}  // Matrix::scale


namespace binvox {

ostream& operator<<(ostream& out_stream, const Matrix& M)
{
  out_stream << M.elm[0] << " " << M.elm[1] << " " << M.elm[2] << " " << M.elm[3] << endl;
  out_stream << M.elm[4] << " " << M.elm[5] << " " << M.elm[6] << " " << M.elm[7] << endl;
  out_stream << M.elm[8] << " " << M.elm[9] << " " << M.elm[10] << " " << M.elm[11] << endl;
  out_stream << M.elm[12] << " " << M.elm[13] << " " << M.elm[14] << " " << M.elm[15];

  return out_stream;
  
}  // << output operator
}


void
Matrix::mirror(int xy, int xz, int yz)
{
  identity();

  if (xy) elm[10] = -1;
  if (xz) elm[5] = -1;
  if (yz) elm[0] = -1;

}  // Matrix::mirror


