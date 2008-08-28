# include <mln/core/image2d.hh>
# include <mln/core/neighb2d.hh>
# include <mln/value/int_u8.hh>

# include <mln/level/fill.hh>
# include <mln/level/paste.hh>
# include <mln/debug/println.hh>
# include <mln/util/timer.hh>
# include <mln/labeling/blobs.hh>

# include <mln/core/alias/p_runs2d.hh>
# include <mln/core/site_set/p_vaccess.hh>
# include <mln/core/image_if.hh>
# include <mln/pw/all.hh>
# include <mln/convert/from_to.hh>

# include <mln/core/var.hh>



template <typename I, typename S>
void fill(S& s, const I& ima)
{
  mln_piter(I) p(ima.domain());
  for_all(p)
    s.insert(ima(p), p);
  mln_postcondition(s.nsites() == ima.nsites());
}



int main()
{
  using namespace mln;
  using value::int_u8;

  bool vals[4][4] = {
    {1, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {1, 1, 0, 1}
  };
  image2d<bool> ima = make::image2d(vals);
  debug::println(ima);

  unsigned n;

  {
    typedef p_vaccess<bool, p_array<point2d> > Arr;
    mlc_equal(Arr::element, point2d)::check();
    Arr arr;
    fill(arr, ima);
    n = arr(true).nsites();
    mln_assertion(arr(true).nsites() + arr(false).nsites() == ima.nsites());
    
    std::cout << "{-} = " << arr(false) << std::endl
	      << "{|} = " << arr(true) << std::endl
	      << std::endl;
  }

  int_u8 nlabels;
  image2d<int_u8> lab = labeling::blobs(ima, c4(), nlabels);
  debug::println(lab);

  {
    typedef p_vaccess<int_u8, p_runs2d> Arr;
    mlc_equal(Arr::element, p_run2d)::check();
    Arr arr;

    mln_VAR(sub, lab | (pw::value(lab) != pw::cst(0u)));
    debug::println(sub);

    convert::from_to(sub, arr);
    mln_assertion(arr.nsites() == n);

    {
      std::cout << "arr = ";
      mln_piter_(Arr) p(arr);
      for_all(p)
	std::cout << p << ' ';
      std::cout << std::endl;
    }

    std::cout << std::endl;

    {
      for (unsigned l = 0; l <= nlabels; ++l)
	{
	  std::cout << "arr(" << l << ") = ";
	  mln_piter_(p_runs2d) p(arr(l));
	  for_all(p)
	    std::cout << p << ' ';
	  std::cout << std::endl;
	}
    }

    std::cout << std::endl;

    {
      for (unsigned l = 0; l <= nlabels; ++l)
	{
	  std::cout << "arr(" << l << ") = ";
	  mln_iter_(util::set<p_run2d>) r(arr(l).set_hook_());
	  for_all(r)
	    std::cout << r << ' ';
	  std::cout << std::endl;
	}
    }

    std::cout << std::endl;

    std::cout << "arr in " << arr.bbox() << std::endl;
    for (unsigned l = 0; l <= nlabels; ++l)
      {
	std::cout << l << ": #" << arr(l).nsites();
	if (! arr(l).is_empty())
	  std::cout << " in " << arr(l).bbox();
	std::cout << std::endl;
      }
    std::cout << std::endl;

    {
      image2d<int> ima_(arr.bbox());
      level::fill(ima_, 0);
      for (unsigned l = 1; l <= nlabels; ++l)
	for (unsigned r = 0; r < arr(l).nelements(); ++r)
	  level::paste(pw::cst(10 * l + r) | arr(l)[r], ima_);
      debug::println(ima_);
    }

    {
      image2d<int> ima_(arr.bbox());
      level::fill(ima_, 0);
      for (unsigned l = 1; l <= nlabels; ++l)
	if (arr(l).nsites() > 1)
	  level::paste(pw::cst(l) | arr(l), ima_);
      debug::println(ima_);
    }
  }

//   {
//     p_runs2d rs;
//     convert::from_to(ima, rs);
//     mln_assertion(rs.bbox() == ima.bbox());

//     image2d<int_u8> ima_(ima.domain());
//     level::fill(ima_, 0);
//     for (unsigned r = 0; r < rs.nelements(); ++r)
//       {
// 	mln_piter_(p_run2d) p(rs[r]);
// 	for_all(p)
// 	  ima_(p) = r + 1;
//       }
//     debug::println(ima_);
//   }

}
