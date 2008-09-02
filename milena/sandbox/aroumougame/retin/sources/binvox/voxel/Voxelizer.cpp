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
// $Id: Voxelizer.cc,v 1.10 2004/10/19 11:43:26 min Exp min $
//

#include <assert.h>
#include "binvox/voxel/Voxelizer.h"
#include "binvox/time/WallTimer.h"
#include "binvox/math/common.h"
using namespace binvox;
using namespace std;

static int carve_x = 1;
static int carve_y = 1;
static int carve_z = 1;





Voxelizer::Voxelizer(Voxels& voxels_ref, Mesh& mesh_ref, GLwindow *win_p) :
  VoxelFilter(voxels_ref),
  mesh(mesh_ref)
{
  dilated = 0;
  step_by_step = 0;

  this->win_p = win_p;
  
   mesh_view_p = new MeshView(&mesh);
  
}  // constructor



Voxelizer::~Voxelizer()
{
   if (mesh_view_p) delete mesh_view_p;
  
}  // destructor



void
Voxelizer::parity_vote_voxelize(int skip_carve)
{
  WallTimer timer("voxelize", 0);
  timer.start();

  //
  // use _both_ methods, every voxel set in old method gets vote of 3
  //
  if (!skip_carve) {
    carve_voxelize();
    int size = voxels.get_size();
    for(int i=0; i < size; i++)
      if (voxels[i]) voxels[i] = 3;  // 3 votes for every remaining voxel
  }

  cout << "Voxelizer::parity_vote_voxelize" << endl;

  //
  // new method starts here
  //
  
  blue_buffer = new GLubyte[wxh];
  assert(blue_buffer);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glDisable(GL_LIGHTING);
  glDisable(GL_DITHER);
  glColor3ub(255, 128, 1);  // go for a more orangy look
   mesh_view_p->set_colour(255, 128, (byte) 1);
  //  mesh_view_p->set_use_no_colour(1);
  glDisable(GL_CULL_FACE);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_BACK);
  glDrawBuffer(GL_BACK);

  glDisable(GL_DEPTH_TEST);

  //
  // x
  //
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(1, 0.5, 0.5,
	    0.5, 0.5, 0.5,
	    0, 0, 1);

  for(int i=0; i < depth; i++) {
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, (i + 0.5) / depth, 1.0);
  
    win_p->clear();
    mesh_view_p->draw_faces();
    glFlush();
//     glFinish();

    read_screen_buffer(blue_buffer);
    voxels.vote(blue_buffer, X, 1, depth - i - 1);

    win_p->swap_buffers();

  }  // for, x from 1 to 0

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0.5, 0.5,
	    1, 0.5, 0.5,
	    0, 0, 1);

  for(int i=0; i < depth; i++) {
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, (i + 0.5) / depth, 1.0);
  
    win_p->clear();
    mesh_view_p->draw_faces();
    glFlush();
//     glFinish();

    read_screen_buffer(blue_buffer);
    voxels.vote(blue_buffer, X, -1, i);

    win_p->swap_buffers();
  
  }  // for, x from 0 to 1

  //
  // y
  //
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 1, 0.5,
	    0.5, 0, 0.5,
	    0, 0, 1);

  for(int i=0; i < width; i++) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, (i + 0.5) / width, 1.0);
  
    win_p->clear();
    mesh_view_p->draw_faces();
    glFlush();
//     glFinish();

    // -Y because bottom left corner of loop in vote is at (100, 100, 0)
    read_screen_buffer(blue_buffer);
    voxels.vote(blue_buffer, Y, -1, width - i - 1);  

    win_p->swap_buffers();
  
  }  // for, y from 1 to 0

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0, 0.5,
	    0.5, 1, 0.5,
	    0, 0, 1);

  for(int i=0; i < width; i++) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, (i + 0.5) / width, 1.0);
  
    win_p->clear();
    mesh_view_p->draw_faces();
    glFlush();
//     glFinish();

    read_screen_buffer(blue_buffer);
    voxels.vote(blue_buffer, Y, 1, i);

    win_p->swap_buffers();
    
  }  // for, y from 0 to 1
  

  //
  // z
  //
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0.5, 1,
	    0.5, 0.5, 0,
	    0, 1, 0);

  for(int i=0; i < height; i++) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, (i + 0.5) / height, 1.0);
  
    win_p->clear();
    mesh_view_p->draw_faces();
    glFlush();
//     glFinish();

    read_screen_buffer(blue_buffer);
    voxels.vote(blue_buffer, Z, 1, height - i - 1);

    win_p->swap_buffers();

  }  // for, z from 1 to 0
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0.5, 0,
	    0.5, 0.5, 1,
	    0, 1, 0);

  for(int i=0; i < height; i++) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 0.5, -0.5, 0.5, (i + 0.5) / height, 1.0);
  
    win_p->clear();
    mesh_view_p->draw_faces();
    glFlush();
//     glFinish();

    read_screen_buffer(blue_buffer);
    voxels.vote(blue_buffer, Z, -1, i);

    win_p->swap_buffers();

  }  // for, z from 0 to 1
  
  //
  // fill voxel array from votes
  //
  voxels.process_votes();
  voxels.init_types();
  
  timer.stop();
//  delete[] blue_buffer;//NEW
  cout << "Voxelizer::voxelize took " << timer.elapsed() << " seconds" << endl;
  
}  // Voxelizer::voxelize



void
Voxelizer::carve_voxelize()
{
  cout << "Voxelizer::carve_voxelize";
  if (dilated) cout << "  (dilated)";
  cout << endl;
  
  char k;
  win_p->make_current();
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDisable(GL_BLEND);
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, 0.5, -0.5, 0.5, 0.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  win_p->clear();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(1, 0.5, 0.5,
	    0.5, 0.5, 0.5,
	    0, 0, 1);
  mesh_view_p->draw_faces();
  glFlush();
//   glFinish();
  read_z_buffer(z_buffer_x_front);

  win_p->swap_buffers();
  if (step_by_step) cin >> k;
    
  win_p->clear();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0.5, 0.5,
	    1, 0.5, 0.5,
	    0, 0, 1);
  mesh_view_p->draw_faces();
  glFlush();
//   glFinish();
  read_z_buffer(z_buffer_x_back);

  win_p->swap_buffers();
  if (step_by_step) cin >> k;
  
  //
  // y
  //
  win_p->clear();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 1, 0.5,
	    0.5, 0, 0.5,
	    0, 0, 1);
  mesh_view_p->draw_faces();
  glFlush();
//   glFinish();
  read_z_buffer(z_buffer_y_front);

  win_p->swap_buffers();
  if (step_by_step) cin >> k;
  
  win_p->clear();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0, 0.5,
	    0.5, 1, 0.5,
	    0, 0, 1);
  mesh_view_p->draw_faces();
  glFlush();
//   glFinish();
  read_z_buffer(z_buffer_y_back);

  win_p->swap_buffers();
  if (step_by_step) cin >> k;

  //
  // z
  //
  win_p->clear();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0.5, 1,
	    0.5, 0.5, 0,
	    0, 1, 0);
  mesh_view_p->draw_faces();
  glFlush();
//   glFinish();
  read_z_buffer(z_buffer_z_front);

  win_p->swap_buffers();
  if (step_by_step) cin >> k;

  win_p->clear();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.5, 0.5, 0,
	    0.5, 0.5, 1,
	    0, 1, 0);
  mesh_view_p->draw_faces();
  glFlush();
//   glFinish();
  read_z_buffer(z_buffer_z_back);

  win_p->swap_buffers();
  if (step_by_step) cin >> k;

  //
  // fill voxel array from z buffers
  //
   fill_voxels();

}  // Voxelizer::carve_voxelize



void
Voxelizer::fill_voxels()
{
  voxels.clear(1);  // fill voxels, then carve away
  //
  // x
  //
  if (carve_x) {
    for(int i=0; i < width; i++) {
      for(int j=0; j < height; j++) {
	float z_front_float = z_buffer_x_front[j * width + i] * depth;
	int z_front = depth - my_round(z_front_float);
	// z_buffer_x_back looked from the other side!!
	float z_back_float = z_buffer_x_back[j * width + (width - i - 1)] * depth;
	int z_back = my_round(z_back_float);
	if(z_front_float != 0.)
//  	cout << i << ", " << j << ": " << z_front_float << " -> " << z_front
//  	     << ", " << z_back_float << " -> " << z_back << endl;
	  
	if (dilated) {
	  z_back--;
	  z_front++;
	}
 	int index = j * width + i;
 	for(int k=0; k < z_back; k++) {
 	  voxels[index] = 0;
 	  index += wxh;
 	}
 	index = j * width + i + z_front * wxh;
 	for(int k = z_front; k < depth; k++) {
 	  voxels[index] = 0;
 	  index += wxh;
 	}

      }  // for, rows
    }  // for, columns
  }
  
  //
  // y
  //
  if (carve_y) {
    for(int i=0; i < depth; i++) {
      for(int j=0; j < height; j++) {
	float z_front_float = z_buffer_y_front[j * depth + i] * width;
	int z_front = my_round(width - z_front_float);
	float z_back_float = z_buffer_y_back[j * depth + (depth - i - 1)] * width;
	int z_back = my_round(z_back_float);
	if(z_front_float != 0.)
// 	cout << i << ", " << j << ": " << z_front_float << " -> " << z_front
// 			<< ", " << z_back_float << " -> " << z_back << endl;
	
	if (dilated) {
	  z_back--;
	  z_front++;
	}
 	int index = (depth - i - 1) * wxh + j * depth;
 	for(int k=0; k < z_back; k++) {
 	  voxels[index] = 0;
 	  index++;
 	}
 	index = (depth - i - 1) * wxh + j * depth + z_front;
 	for(int k = z_front; k < width; k++) {
 	  voxels[index] = 0;
 	  index++;
 	}
      }
    }
  }
  
  //
  // z
  //
  if (carve_z) {
    for(int i=0; i < depth; i++) {
      for(int j=0; j < width; j++) {
	float z_front_float = z_buffer_z_front[j * depth + i] * height;
	int z_front = my_round(height - z_front_float);
	float z_back_float = z_buffer_z_back[j * depth + (depth - i - 1)] * height;
	int z_back = my_round(z_back_float);
	if(z_front_float != 0.)
// 	cout << i << ", " << j << ": " << z_front_float << " -> " << z_front
// 			<< ", " << z_back_float << " -> " << z_back << endl;

	if (dilated) {
	  z_back--;
	  z_front++;
	}
 	int index = i * wxh + j;
  	for(int k=0; k < z_back; k++) {
  	  voxels[index] = 0;
  	  index += width;
  	}
 	index = i * wxh + j + z_front * width;
   	for(int k = z_front; k < height; k++) {
   	  voxels[index] = 0;
   	  index += width;
   	}
      }
    }
  }  // if (carve_z)

  voxels.init_types();
  
}  // Voxelizer::fill_voxels



void
Voxelizer::read_z_buffer(GLfloat *buffer_p)
{
  // note: having VOXEL_WIDTH and VOXEL_HEIGHT only implies same dimensions
  glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT,GL_FLOAT, buffer_p);

}  // Voxelizer::read_z_buffer




void
Voxelizer::read_screen_buffer(GLubyte *buffer_p)
{
  // note: having VOXEL_WIDTH and VOXEL_HEIGHT only, implies same dimensions
  glReadPixels(0, 0, width, height, GL_BLUE, GL_UNSIGNED_BYTE, buffer_p);
  
}  // Voxelizer::read_screen_buffer



