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

#ifndef MLN_LEVEL_FILL_HH
# define MLN_LEVEL_FILL_HH

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace level
  {

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I)& value);

    template <typename I>
    void fill(Image<I>& ima_,
	      mln_value(I) (*f)(const mln_point(I)& p));

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I) array[]);

    template <typename I, typename J>
    void fill(Image<I>& ima_,
	      const Image<J>& data);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I)& value)
    {
      I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = value;
    }

    template <typename I>
    void fill(Image<I>& ima_,
	      mln_value(I) (*f)(const mln_point(I)& p))
    {
      I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = f(p);
    }

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I) array[])
    {
      I& ima = exact(ima_);
      mln_precondition(ima.has_data());
      mln_piter(I) p(ima.domain());
      unsigned i = 0;
      for_all(p)
	ima(p) = array[i++];
    }

    template <typename I, typename J>
    void fill(Image<I>& ima_,
	      const Image<J>& data_)
    {
      I&        ima = exact(ima_);
      const J& data = exact(data_);
      mln_precondition(ima.has_data() && data.has_data());

      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = data(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_HH
