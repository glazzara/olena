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
// $Id$
//

#if (IRIX || LINUX)
#include <sys/time.h>
#else
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#include "binvox/time/WallTimer.h"

#define DEBUG(x)

using namespace binvox;



WallTimer::WallTimer(string name, int verbose) :
  my_name(name)
{
  elapsed_seconds = 0;
  lap_times.clear();
  this->verbose = verbose;
  if (verbose) cout << "Timer [" << my_name << "] started" << endl;
  start();
  
}  // constructor



WallTimer::~WallTimer()
{
  stop();
  if (verbose) cout << "Timer [" << my_name << "] stopped at " << elapsed_seconds << " seconds" << endl;

}  // destructor



void
WallTimer::get_cur_time()
{
#if (IRIX || LINUX)
  struct timeval t;
#endif
#if IRIX
  gettimeofday(&t);
#endif
#if LINUX
  struct timezone tz;
  gettimeofday(&t, &tz);
#endif

#if (IRIX || LINUX)
  cur_sec = t.tv_sec;
  cur_microsec = t.tv_usec;
#endif

#if WINDOWS_NT
  struct _timeb t;
  _ftime(&t);

  cur_sec = t.time;
  cur_microsec = 1000 * t.millitm;
#endif

}  // WallTimer::get_cur_time



void
WallTimer::start()
{
  get_cur_time();

  start_sec = cur_sec;
  start_microsec = cur_microsec;

  lap_times.clear();
  lap_times.push_back(0);
  
}  // WallTimer::start



void
WallTimer::lap()
{
  get_cur_time();

  float time = (cur_sec + cur_microsec/1000000.0);
  lap_times.push_back(time);

}  // WallTimer::lap



float
WallTimer::get_lap_time(int index)
{
  if ((index < 1) || (index >= lap_times.size())) {
    cout << "WallTimer::get_lap_time(" << index << ") error: index out of range [1 - "
	 << lap_times.size() - 1 << "]" << endl;
    return 0;
  }

  return(lap_times[index] - lap_times[index - 1]);
  
}  // WallTimer::get_lap_time



void
WallTimer::stop()
{
  get_cur_time();

  stop_sec = cur_sec;
  stop_microsec = cur_microsec;
  
  elapsed_seconds = (float)(stop_sec - start_sec) + (float)(stop_microsec - start_microsec)/1000000.0;
  
  DEBUG(cout << "elapsed: " << elapsed_seconds << endl);
  
}  // WallTimer::stop

namespace binvox {

ostream&
operator <<(ostream& out_stream, WallTimer& T)
{
  out_stream << T.elapsed_seconds;
  return out_stream;
  
}  // WallTimer operator <<

}

