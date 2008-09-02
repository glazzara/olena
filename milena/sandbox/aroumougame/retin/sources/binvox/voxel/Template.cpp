//
// thinvox, a binary voxel thinning program
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
// $Id: Template.cc,v 1.4 2000/12/21 23:25:35 min Exp $
//

#include "binvox/voxel/Template.h"

using namespace std;

using namespace binvox;



Template::Template()
{
  for(int i=0; i < 27; i++) t[i] = 0;

}  // constructor



int
Template::match(VoxelType *nb)
{
//    cout << "Template::match, for template: " << *this << endl;
//    char k;
//    cin >> k;
  
  int needs_potential_black = 0;
  int has_potential_black = 0;

  for(int i=0; i < 27; i++) {
    if (t[i] == DONT_CARE) continue;
    if (t[i] == POTENTIAL_BLACK) {
      needs_potential_black = 1;
      if (nb[i] == BLACK) has_potential_black = 1;
    }
    else {
      if (t[i] != nb[i]) return 0;
    }
  }

  if (needs_potential_black && !has_potential_black) return 0;

  return 1;

}  // Template::match



int
Template::operator==(Template& other)
{
  for(int i=0; i < 27; i++) {
    if (t[i] != other[i]) return 0;
  }
  return 1;

}  // Template::operator==



void
Template::swap(int i1, int i2)
{
  VoxelType temp = t[i1];
  t[i1] = t[i2];
  t[i2] = temp;

}  // Template::swap



void
Template::reflect_sw_ne()
{
  // bottom (D) plane
  swap(0, 8);
  swap(1, 5);
  swap(3, 7);

  // middle plane
  swap(9, 17);
  swap(10, 14);
  swap(12, 16);

  // top (U) plane
  swap(18, 26);
  swap(19, 23);
  swap(21, 25);
  
}  // Template::reflect_sw_ne



void
Template::reflect_us_dn()
{
  // left (W) plane
  swap(6, 18);
  swap(3, 9);
  swap(15, 21);

  // middle plane
  swap(7, 19);
  swap(4, 10);
  swap(16, 22);

  // right (E) plane
  swap(8, 20);
  swap(5, 11);
  swap(17, 23);
  
}  // Template::reflect_us_dn



void
Template::reflect_uw_de()
{
  // front (S) plane
  swap(6, 26);
  swap(7, 17);
  swap(15, 25);

  // middle plane
  swap(3, 23);
  swap(4, 14);
  swap(12, 22);

  // back (N) plane
  swap(0, 20);
  swap(1, 11);
  swap(9, 19);
  
}  // Template::reflect_uw_de



static int order[3][9] = {
  {18, 19, 20, 21, 22, 23, 24, 25, 26},
  {9, 10, 11, 12, 13, 14, 15, 16, 17},
  {0, 1, 2, 3, 4, 5, 6, 7, 8}
};

ostream& operator <<(ostream& out_stream, const Template& T)
{
  for(int i=0; i < 3; i++) {
    out_stream << "layer " << i << ":" << endl;
    for(int j=0; j < 9; j++) {
      if ((j == 3) || (j == 6)) out_stream << endl;
      switch(T[order[i][j]]) {
	case WHITE: out_stream << "o "; break;
	case BLACK: out_stream << "* "; break;
	case DONT_CARE: out_stream << ". "; break;
	case POTENTIAL_BLACK: out_stream << "x "; break;
	default:
	  out_stream << "operator<< for Template error: unknown value "
		     << T[order[i][j]] << endl;
      }  // switch
    }
    out_stream << endl;
  }

  return out_stream;
  
}  // operator<< for Template



void
Template::flip_ud()
{
  for(int i=0; i < 9; i++)
    swap(i, i + 18);

}  // Template::flip_ud



static int north[9] = {0, 1, 2, 9, 10, 11, 18, 19, 20};

void
Template::flip_ns()
{
  for(int i=0; i < 9; i++)
    swap(north[i], north[i] + 6);
  
}  // Template::flip_ns



static int west[9] = {0, 3, 6, 9, 12, 15, 18, 21, 24};

void
Template::flip_we()
{
  for(int i=0; i < 9; i++)
    swap(west[i], west[i] + 2);
  
}  // Template::flip_we


