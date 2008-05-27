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

#ifndef __WALLTIMER_H
#define __WALLTIMER_H

#ifdef IRIX
#include <locale>
#endif
#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace binvox {



class WallTimer {

public:

  WallTimer(string name = "", int verbose = 1);
  ~WallTimer();

  void start();
  void lap();
  int get_nr_laps() { return lap_times.size() - 1; }
  float get_lap_time(int index);
  void stop();
  float elapsed() { return elapsed_seconds; };
  friend ostream& operator <<(ostream& out_stream, WallTimer& T);

  

private:

  string my_name;
  int verbose;
  
  void get_cur_time();
  
  long cur_sec, cur_microsec;
  long start_sec, start_microsec;
  long stop_sec, stop_microsec;
  
  float elapsed_seconds;
  vector<float> lap_times;
  

};  // class WallTimer
}

#endif
