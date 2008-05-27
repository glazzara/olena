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
// $Id: View.cc,v 1.4 2001/01/20 02:38:09 min Exp $
//

#include "binvox/ui/View.h"


using namespace binvox;


View::View()
{
  enabled = 1;
  colour_ptr = colour;
  set_colour((float) 1.0, 1.0, 1.0);
  
}  // constructor



View::~View()
{

}  // destructor



void
View::redraw()
{

}  // View::redraw



void
View::update()
{
  if (win_ptr) win_ptr->redraw();

}  // View::update



void
View::set_colour(float red, float green, float blue)
{
  colour[0] = red;
  colour[1] = green;
  colour[2] = blue;
  colour_ptr = colour;
  
}  // View::set_colour, float arguments



void
View::set_colour(byte red, byte green, byte blue)
{
  colour[0] = red / 255.0;
  colour[1] = green / 255.0;
  colour[2] = blue / 255.0;
  colour_ptr = colour;
  
}  // View::set_colour, byte arguments



