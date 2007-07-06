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

#ifndef OLN_HISTO_HISTOGRAM_HH
# define OLN_HISTO_HISTOGRAM_HH

namespace oln
{

  /// Concept-class "Histogram".

  template <typename T>
  struct Histogram
  {
    void take(T t)
    {
      v.insert(v.begin(), t);
      if (ival > 0)
	ival++;
      return 0;
    }

    void untake()
    {
      if (this.ival == v.size)
	ival = -1;
      T ret = v.back();
      v.pop_back();
      return ret;
    }

    T operator()(int i) { return v[i]; }
    void clean()        { v.clear(); }

  protected:
    Histogram() : ival(-1) {}

    std::vector<unsigned> v;
    int ival;
  }; // end of oln::Histogram<Exact>


} // end of namespace oln

#endif // ! OLN_HISTO_HISTOGRAM_HH
