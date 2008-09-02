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
// $Id: Vector.cc,v 1.3 2001/03/19 19:30:07 min Exp min $
//

#include <math.h>
#include <stdio.h>

#include "binvox/math/Vector.h"
#include "binvox/math/MyRandom.h"

using namespace std;
using namespace binvox;
static const Float epsilon = 1e-6;  // for ccw





Vector::Vector(Float a, Float b, Float c)
{
  set(a, b, c);
  
}  // Vector constructor



Vector::Vector(Float a, Float b, Float c, Float d)
{
  set(a, b, c, d);
  
}  // Vector constructor



void
Vector::set(Float a, Float b, Float c)
{
  set(a, b, c, 1);

}  // set with 3 parameters



void
Vector::set(Float a, Float b, Float c, Float d)
{
  elm[X] = a;
  elm[Y] = b;
  elm[Z] = c;
  elm[W] = d;
  
}  // set with 4 parameters



Vector
Vector::operator +(const Vector& A)
{
  Vector result;

  for(int i = 0; i < VECTOR_SIZE; i++)
    result[i] = elm[i] + A[i];

  return result;
}  // Vector operator +



Vector
Vector::operator -(Vector& A)
{
  Vector result;

  for(int i = 0; i < VECTOR_SIZE; i++)
    result[i] = elm[i] - A[i];

  return result;
}  // Vector operator -



Vector
Vector::operator -()
{
  Vector result;

  for(int i = 0; i < VECTOR_SIZE; i++)
    result[i] = -elm[i];

  return result;
}  // Vector operator - (unary)



int 
Vector::operator ==(const Vector &B)
{
  for(int i = 0; i < VECTOR_SIZE; i++)
    if (elm[i] != B[i]) return 0;

  return 1;
}  // Vector::operator ==



int
Vector::operator !=(const Vector& B)
{
  for(int i = 0; i < VECTOR_SIZE; i++)
    if (elm[i] != B[i]) return 1;

  return 0;
}  // Vector::operator !=



Float
Vector::operator *(const Vector& A)
{
  Float result = 0;

  for(int i = 0; i < VECTOR_SIZE; i++)
    result += elm[i] * A[i];

  return result;
}  // Vector operator * (in product)


namespace binvox {

ostream&
operator <<(ostream& out_stream, const Vector& T)
{
  out_stream << "[";
  for(int i = 0; i < MAX_SIZE; i++) {
    out_stream << T[i];
    if (i < (MAX_SIZE - 1)) out_stream << ", ";
  }
  out_stream << "]";
  return out_stream;
  
}  // Vector operator <<



istream&
operator>>(istream& in_stream, Vector& T)
{
  cout << "Vector operator>>" << endl;
  char k;

  in_stream >> k;
  cout << "read char " << k << endl;
  
  for(int i=0; i < VECTOR_SIZE; i++) {
    in_stream >> T[i];
    cout << "read T[" << i << "]: " << T[i] << endl;
    if (i < (VECTOR_SIZE - 1)) in_stream >> k;
  }
  in_stream >> k;
  cout << "read char " << k << endl;

  return in_stream;

}  // Vector operator >>

}

Float
Vector::sum()
{
  return(elm[X] + elm[Y] + elm[Z]);

}  // Vector::sum



Float
Vector::squared_length()
{
  Float squared_length = 0;

  for(int i = 0; i < VECTOR_SIZE; i++)
    squared_length += elm[i] * elm[i];

  return squared_length;

}  // Vector::squared_length


  
Float
Vector::length()
{
  Float len = squared_length();
  
  return sqrt(len);

}  // Vector::length



Float
Vector::squared_distance(Vector& V)
{
  Vector diff;
  for(int i=0; i < VECTOR_SIZE; i++)
    diff[i] = elm[i] - V[i];
  
  return diff.squared_length();
    
}  // Vector::squared_distance



Float
Vector::distance(Vector& V)
{
  Vector diff;
  for(int i=0; i < VECTOR_SIZE; i++)
    diff[i] = elm[i] - V[i];
  
  return diff.length();

}  // Vector::distance



void
Vector::normalize()
{
  Float my_length = length();

  if (my_length) {
    for(int i = 0; i < VECTOR_SIZE; i++)
      elm[i] /= my_length;
  }

}  // normalize


namespace binvox {
Vector
operator *(const Vector& A, const Float scalar)
{
  Vector result;

  for(int i = 0; i < VECTOR_SIZE; i++)
    result[i] = scalar * A[i];

  return result;
}  // mult. operator for scalar * Vector



Vector
operator *(const Float scalar, const Vector& A)
{
  return (A * scalar);
}  // multiply for Vector * scalar



Vector
operator /(const Vector& A, const Float scalar)
{
  Vector result;

  for(int i = 0; i < VECTOR_SIZE; i++)
    result[i] = A[i] / scalar;

  return result;
}  // division operator for Vector / scalar

}

void
Vector::clamp(Float min, Float max)
{
  for(int i=0; i < VECTOR_SIZE; i++) {
    if (elm[i] > max) elm[i] = max;
    else if (elm[i] < min) elm[i] = min;
  }
  
}  // Vector::clamp



Vector
Vector::vec_abs()
{
  Vector result;
  
  for(int i=0; i < VECTOR_SIZE; i++) {
    result[i] = fabs(elm[i]);
  }
  return result;
  
}  // Vector::vec_abs



Vector
Vector::round()
{
  Vector result;
  
  for(int i=0; i < VECTOR_SIZE; i++) {
    result[i] = floor(elm[i] + 0.5);
  }
  return result;

}  // Vector::round



Vector 
Vector::operator ^(const Vector& A)
{
  Vector result;

  result[X] = elm[Y] * A[Z] - A[Y] * elm[Z];
  result[Y] = A[X] * elm[Z] - elm[X] * A[Z];
  result[Z] = elm[X] * A[Y] - A[X] * elm[Y];

  return result;
}  // cross product, operator ^

namespace binvox {

int
ccw(Vector& A, Vector& B, Vector& C)
{
  if (A == B) {
    if (A == C) return 0;
    else return 2;
  }

  if ((A == C) || (B == C)) return 0;

  Float ccw_value = (B[X] * C[Y] - C[X] * B[Y] -
		     A[X] * C[Y] + C[X] * A[Y] +
		     A[X] * B[Y] - B[X] * A[Y]);

  if (ccw_value > epsilon) return 1;
  else if (ccw_value < -epsilon) return -1;

  Vector L = B - A;
  Vector R = C - A;
  if ((L * R) < 0) return -2;

  L = A - B;
  R = C - B;
  if ((L * R) < 0) return 2;

  return 0;
    
}  // ccw

}

Matrix
Vector::star()
{
  Matrix *M = new Matrix(0, -elm[X], elm[Y],
			 elm[Z], 0, -elm[X],
			 -elm[Y], elm[X], 0);
  return *M;
}  // Vector::star, create matrix to mult by matrix to create crossprod



Matrix
Vector::outer_product()
{
  Matrix *M = new Matrix(elm[X] * elm[X], elm[X] * elm[Y], elm[X] * elm[Z],
			 elm[Y] * elm[X], elm[Y] * elm[Y], elm[Y] * elm[Z],
			 elm[Z] * elm[X], elm[Z] * elm[Y], elm[Z] * elm[Z]);
  return *M;

}  // Vector::outer_product



int
Vector::max_dimension()
{
  if ((elm[X] > elm[Y]) && (elm[X] > elm[Z]))
    return X;
  else {
    if ((elm[Y] > elm[X]) && (elm[Y] > elm[Z]))
      return Y;
    else
      return Z;
  }
}  // Vector::max_dimension



int intersect(Vector& p1, Vector& p2, Vector& q1, Vector& q2)
//
// for each pair of ccw's (the first pair being ccw1 and ccw2, the
// second ccw3 and ccw4) it should be true that either:
// - at least one of them is zero (implying that the third or fourth
//   point is inside the first line segment)
// - or their product is negative (implying that the first and second
//   point of the other segment lie on opposite sides)
//
{
  int ccw1 = ccw(p1, p2, q1);
  int ccw2 = ccw(p1, p2, q2);
  int ccw3 = ccw(q1, q2, p1);
  int ccw4 = ccw(q1, q2, p2);

  return (((ccw1 * ccw2) <= 0) || ((ccw3 * ccw4) <= 0));

}  // intersect



void
Vector::random_in_sphere()
{
  // get point on surface of 4D sphere and normalize...
  
  Float temp[4];
  Float length = 0;

  for(int i=0; i < 4; i++) {
    temp[i] = MyRandom::gaussian();
    length += temp[i] * temp[i];
  }
  length = sqrt(length);

  for(int i=0; i < 3; i++)
  {  // 3!
    temp[i] /= length;
    elm[i] = temp[i];
  }
  elm[W] = 1.0;
  
}  // Vector::random_in_sphere



void
Vector::random_on_sphere()
{
  for(int i=0; i < VECTOR_SIZE; i++) {
    elm[i] = MyRandom::gaussian();
  }  // for

  normalize();

}  // Vector::random_on_sphere



void
Vector::project(Vector normal)
{
  Vector temp = (*this * normal) * normal;
  *this = *this - temp;

}  // Vector::project



void
Vector::uniform(Float min, Float max)
{
  for(int i=0; i < VECTOR_SIZE; i++) {
    elm[i] = MyRandom::uniform(min, max);
  }

}  // Vector::uniform



void
Vector::rotate(Vector& axis, Float angle)
{
  //
  // Rotate vector counterclockwise around axis (looking at axis end-on) (rz(xaxis) = yaxis)
  // From Goldstein: v' = v cos t + a (v . a) [1 - cos t] - (v x a) sin t
  //
  
  Float cos_angle = cos(angle);
  Float dot = *this * axis;
  
  Vector cross = *this ^ axis;
  Vector temp = *this;
  *this = cos_angle * temp;
  *this = *this + dot * (1.0 - cos_angle) * axis;
  cross = sin(angle) * cross;
  *this = *this -  cross;

}  // Vector::rotate



//
// scale by another vector, X by X, etc.
//
void
Vector::scale(Vector& scale)
{
  elm[X] *= scale[X];
  elm[Y] *= scale[Y];
  elm[Z] *= scale[Z];
  
}  // Vector::scale



void
Vector::flip_y_z()
{
  Float temp = elm[Y];
  elm[Y] = elm[Z];
  elm[Z] = temp;
  
}  // Vector::flip_y_z


