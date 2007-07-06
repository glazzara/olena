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

#ifndef OLN_HISTO_MEDIAN_HH
# define OLN_HISTO_MEDIAN_HH

# include <oln/histo/histogram.hh>


namespace oln
{

  namespace histo_median
  {

    template <typename T>
    struct histo_median : public Histogram< histo_median<T> >
    {
      histo_median() : imed(this.ival) {}

      void slide(T i)
      {
	int last_i = take(i);
	untake();
	if (imed == -1)
	  compute_median();
	else
	  compute_median();
      }

      void compute_median()
      {
	T total = 0;
	for (int i = 0; i < this.v.size; i++)
	  total += this.v[i];

	imed = 0;
	T sum = this.v[imed];
	while (sum < total/2)
	{
	  ++imed;
	  sum += this.v[imed];
	}
      }

      T median() { return this.v[imed]; }
    private:
      int& imed;
      T rest;
    };


# ifndef OLN_INCLUDE_ONLY

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::histo_median

} // end of namespace oln

#endif // ! OLN_HISTO_MEDIAN_HH
