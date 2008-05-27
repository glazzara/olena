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
// $Id: CameraList.cc,v 1.1 2002/10/30 19:20:06 min Exp min $
//

#include "binvox/ui/CameraList.h"



using namespace binvox;

CameraList::CameraList()
{
  Camera *default_cam = new Camera();
  cameras.push_back(default_cam);
  cur_index = 0;
  
}  // constructor



CameraList::~CameraList()
{

}  // destructor



void
CameraList::clear()
{
  //  cout << "CameraList::clear" << endl;
  for(int i=0; i < cameras.size(); i++) delete cameras[i];
  cameras.clear();
  cur_index = -1;
  
}  // CameraList::clear



void
CameraList::add_camera(Camera *new_cam)
{
  //  cout << "CameraList::add_camera: " << *new_cam << endl;
  cameras.push_back(new_cam);
  if (cameras.size() == 1) cur_index = 0;

}  // CameraList::add_camera



void
CameraList::set_cur_camera(int index)
{
  cout << "CameraList::set_cur_camera(" << index << ")" << endl;
  cur_index = index;
  //  cameras[cur_index]->print();
  cameras[cur_index]->load_matrix();
  
}  // CameraList::set_cur_camera



Camera *
CameraList::set_next_cam()
{
  cur_index = (cur_index + 1) % cameras.size();
  cout << "CameraList::set_next_cam, cur_index now is " << cur_index << "/" << cameras.size() << endl;
  //  cameras[cur_index]->print();
  cameras[cur_index]->load_matrix();
  
  return cameras[cur_index];
  
}  // CameraList::*set_next_cam



Camera *
CameraList::set_prev_cam()
{
  cur_index = (cur_index - 1) % cameras.size();
  cout << "CameraList::set_prev_cam, cur_index now is " << cur_index << "/" << cameras.size() << endl;
  //  cameras[cur_index]->print();
  cameras[cur_index]->load_matrix();
  
  return cameras[cur_index];

}  // CameraList::*set_prev_cam



void
CameraList::transform(Matrix T)
{
  // for now only called with the normalization transform
  // (which is translate, scale, translate)
  
  //  cout << "CameraList::transform(" << T << ")" << endl;

  for(int i=0; i < cameras.size(); i++) {
    Vector pos = cameras[i]->get_position();
    Vector lookat = cameras[i]->get_lookat();

    pos = T * pos;
    lookat = T * lookat;

    cameras[i]->set_position(pos);
    cameras[i]->set_lookat(lookat);
  }  // for
  
}  // CameraList::transform


