// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_HISTO_HISTO_MIN_HH
# define OLN_HISTO_HISTO_MIN_HH

# include <oln/histo/histogram.hh>
# include <oln/core/internal/max_value.hh>

namespace oln
{

  namespace histo_min
  {

    template <typename T>
    struct histo_min : public Histogram< histo_min<T> >
    {
      histo_min() : imin(this.ival) {}

      template <typename L>
      void init(L line)
      {
	int i;
	for (i = 0; i < line.norme; i++)
	  take(oln_max(T));
	imin = i/2;
      }

      void slide(T i)
      {
	int last_i = take(i);
	untake();
	if (imin == -1)
	  compute_min();
	else if (i < this.v[imin])
	  imin = last_i;
      }

      void compute_min()
      {
	imin = 0;
	for (int i = 1; i < this.v.size; i++)
	  if (this.v[i] < this.v[imin])
	    imin = i;
      }

      T min() { return this.v[imin]; }
    private:
      int& imin;
    };


# ifndef OLN_INCLUDE_ONLY

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::histo_min

} // end of namespace oln

#endif // ! OLN_HISTO_HISTO_MIN_HH




