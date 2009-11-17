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

#include <queue>
#include <mln/core/routine/duplicate.hh>
#include <mln/data/fill.hh>
#include <mln/extension/fill.hh>

#include <mln/transform/influence_zone_geodesic.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/util/timer.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>



# define loop(n) for (unsigned i = 0; i < n; ++i)



namespace mln
{


  template <typename I, typename N>
  mln_concrete(I) do_iz(const I& input, const N& nbh)
  {
    mln_ch_value(I, unsigned) dmap;
    std::queue<unsigned> q;
    mln_concrete(I) output;

    // Initialization.
    {
      output = duplicate(input); // <-- init
      extension::fill(output, 1); // For the extension to be ignored.

      mln_pixter(const I)    p(input);
      mln_nixter(const I, N) n(p, nbh);
      for_all(p)
	if (p.val() != 0) // <-- inqueue_p_wrt_input_p
	  {
	    ; // <-- init_p
	    // FIXME: dmap.element(p.offset()) = 0;
	    for_all(n)
	      if (n.val() == 0) // <-- inqueue_p_wrt_input_n
		{
		  q.push(p.offset());
		  break;
		}
	  }
    }

    // Propagation.
    {
      unsigned p;

      util::array<int> dp = offsets_wrt(input, nbh);
      const unsigned n_nbhs = dp.nelements();

      while (! q.empty())
	{
	  p = q.front();
	  q.pop();
	  // pas de saturation
	  if (output.element(p) == 0)
	    std::cerr << "oops!" << std::endl;
	  for (unsigned i = 0; i < n_nbhs; ++i)
	    {
	      unsigned n = p + dp[i];
	      if (output.element(n) == 0)
		{
		  output.element(n) = output.element(p); // <- process
		  q.push(n);
		}
	    }
	}
    }

    return output;
  }



  template <typename I, typename N>
  mln_concrete(I) do_iz__bis(const I& input, const N& nbh)
  {
    std::queue<unsigned> q;
    mln_concrete(I) output;

    util::array<int> dp = offsets_wrt(input, nbh);
    const unsigned n_nbhs = dp.nelements();

    // Initialization.
    {
      output = duplicate(input); // <-- init
      extension::fill(output, 1); // For the extension to be ignored.

      mln_box_runstart_piter(I) s(input.domain());
      const unsigned len = s.run_length();
      for_all(s)
      {
	unsigned p = input.index_of_point(s);
	for (unsigned i = 0; i < len; ++i, ++p)
	  if (input.element(p) != 0)
	    {
	      for (unsigned j = 0; j < n_nbhs; ++j)
		{
		  unsigned n = p + dp[j];
		  if (input.element(n) == 0)
		    {
		      q.push(p);
		      break;
		    }
		}
	    }
      }
    }

    // Propagation.
    {
      unsigned p;

      while (! q.empty())
	{
	  p = q.front();
	  q.pop();
	  mln_invariant(output.element(p) != 0);
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];
	      if (output.element(n) == 0)
		{
		  output.element(n) = output.element(p); // <- process
		  q.push(n);
		}
	    }
	}
    }

    return output;
  }





  template <typename I, typename N>
  mln_concrete(I) do_iz__ter(const I& input, const N& nbh)
  {
    std::queue<unsigned> q;
    mln_concrete(I) output;

    util::array<int> dp = offsets_wrt(input, nbh);
    const unsigned n_nbhs = dp.nelements();
    const unsigned
      skip_border = 2 * input.border(),
      nrows = input.nrows(),
      ncols = input.ncols();

    // Initialization.
    {
      output = duplicate(input); // <-- init
      extension::fill(output, 1); // For the extension to be ignored.

      unsigned p = input.index_of_point(point2d(0,0));
      for (unsigned row = 0; row < nrows; ++row, p += skip_border)
	{
	  for (unsigned i = 0; i < ncols; ++i, ++p)
	    if (input.element(p) != 0)
	      for (unsigned j = 0; j < n_nbhs; ++j)
		{
		  unsigned n = p + dp[j];
		  if (input.element(n) == 0)
		    {
		      q.push(p);
		      break;
		    }
		}
	}
    }

    // Propagation.
    {
      unsigned p;

      while (! q.empty())
	{
	  p = q.front();
	  q.pop();
	  mln_invariant(output.element(p) != 0);
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];
	      if (output.element(n) == 0)
		{
		  output.element(n) = output.element(p); // <- process
		  q.push(n);
		}
	    }
	}
    }

    return output;
  }




  template <typename I, typename N>
  mln_concrete(I) do_iz__ptr(const I& input, const N& nbh)
  {
    std::queue<mln_value(I)*> q;
    mln_concrete(I) output;

    util::array<int> dp = offsets_wrt(input, nbh);
    const unsigned n_nbhs = dp.nelements();
    const unsigned
      skip_border = 2 * input.border(),
      nrows = input.nrows(),
      ncols = input.ncols();

    // Initialization.
    {
      output = duplicate(input);
      // For the extension to be ignored:
      extension::fill(input, 0);  // in initialization
      extension::fill(output, 1); // in propagation

      const unsigned nelts = input.nelements();
      const mln_value(I)* p_i = & input.at_(0, 0);
      mln_value(I)* p_o = & output.at_(0, 0);
      for (unsigned i = 0; i < nelts; ++i, ++p_i, ++p_o)
	{
	  if (*p_i == 0)
	    continue;
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      const mln_value(I)* n_i = p_i + dp[j];
	      if (*n_i == 0)
		{
		  q.push(p_o);
		  break;
		}
	    }
	}

    }

    // Propagation.
    {
      mln_value(I)* ptr;

      while (! q.empty())
	{
	  ptr = q.front();
	  q.pop();
	  mln_invariant(*ptr != 0);
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      mln_value(I)* ntr = ptr + dp[j];
	      if (*ntr == 0)
		{
		  *ntr = *ptr;
		  q.push(ntr);
		}
	    }
	}
    }

    return output;
  }



} // end of namespace mln





int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> in;
  io::pgm::load(in, "input_iz.pgm");

  util::timer t;
  neighb2d nbh = c4();

//   {
//     t.start();
//     image2d<int_u8> ref = transform::influence_zone_geodesic(in, nbh, mln_max(unsigned));
//     float ts = t.stop();
//     std::cout << ts << std::endl;
//     io::pgm::save(ref, "ref.pgm");
//   }

//   {
//     t.start();
//     image2d<int_u8> out = do_iz__bis(in, nbh);
//     float ts = t.stop();
//     std::cout << ts << std::endl;
//     io::pgm::save(out, "iz_bis.pgm");
//   }

//   {
//     t.start();
//     image2d<int_u8> out = do_iz__ter(in, nbh);
//     float ts = t.stop();
//     std::cout << ts << std::endl;
//     io::pgm::save(out, "iz_ter.pgm");
//   }

//   {
//     t.start();
//     image2d<int_u8> out = do_iz__ptr(in, nbh);
//     float ts = t.stop();
//     std::cout << ts << std::endl;
//     io::pgm::save(out, "iz_ptr.pgm");
//   }


  const unsigned n_times = 10;

  {
    t.start();
    loop(n_times) transform::influence_zone_geodesic(in, nbh, mln_max(unsigned));
    float ts = t.stop();
    std::cout << ts << std::endl;
  }

  {
    t.start();
    loop(n_times) do_iz(in, nbh);
    float ts = t.stop();
    std::cout << ts << std::endl;
  }

  {
    t.start();
    loop(n_times) do_iz__bis(in, nbh);
    float ts = t.stop();
    std::cout << ts << std::endl;
  }

  {
    t.start();
    loop(n_times) do_iz__ter(in, nbh);
    float ts = t.stop();
    std::cout << ts << std::endl;
  }

  {
    t.start();
    loop(n_times) do_iz__ptr(in, nbh);
    float ts = t.stop();
    std::cout << ts << std::endl;
  }

}
