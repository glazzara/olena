// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef CHAIN_HH
# define CHAIN_HH

#include <mln/morpho/closing/area.hh>
#include <mln/morpho/watershed/flooding.hh>

template <typename I, typename N, typename L>
mln_ch_value(I, L)
chain(const mln::Image<I>& ima, const mln::Neighborhood<N>& nbh,
      unsigned lambda, L& nbasins)
{
  mln_concrete(I) c = mln::morpho::closing::area(ima, nbh, lambda);
  mln_ch_value(I, L) s = mln::morpho::watershed::flooding(c, nbh, nbasins);
  return s;
}

#endif // ! CHAIN_HH
