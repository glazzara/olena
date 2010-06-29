#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/core/box_runend_piter.hh>

#include <mln/extension/adjust.hh>
#include <mln/extension/fill.hh>
#include <mln/data/fill.hh>
#include <mln/accu/shape/bbox.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>
#include <mln/debug/println.hh>

#include <mln/util/timer.hh>
#include <mln/labeling/foreground.hh>
#include <mln/labeling/wrap.hh>
#include <mln/extension/fill.hh>
#include <mln/data/compare.hh>


namespace mln
{

  template <typename I>
  unsigned my_find_root(image2d<I>& data, unsigned x)
  {
    if (data.element(x).parent == x)
      return x;
    else
      return data.element(x).parent = my_find_root(data,
						   data.element(x).parent);
  }



  struct info
  {
    unsigned parent;
    unsigned card;
    float row_sum, col_sum;
    point2d p_min, p_max;

    int width() const
    {
      return p_max.col() - p_min.col();
    }

    int height() const
    {
      return p_max.row() - p_min.row();
    }

    void init(unsigned p, int row, int col)
    {
      parent = p;
      card = 1;
      row_sum = row;
      col_sum = col;
      p_min.row() = row;
      p_max.row() = row;
      p_min.col() = col;
      p_max.col() = col;
    }

    void update(info& r)
    {
      r.parent = this->parent;
      card += r.card;
      row_sum += r.row_sum;
      col_sum += r.col_sum;

      // bkd browsing => p is always higher (lower row) than r
      mln_invariant(p_min.row() <= r.p_min.row());

      if (r.p_min.col() < p_min.col())
	p_min.col() = r.p_min.col();
      if (r.p_max.row() > p_max.row())
	p_max.row() = r.p_max.row();
      if (r.p_max.col() > p_max.col())
	p_max.col() = r.p_max.col();
    }
  };



  template <typename V>
  image2d<V>
  extract_components(const image2d<bool>& input,
		     V& nlabels,
		     util::array<box2d>& bboxes,
		     util::array<point2d>& mass_centers)
  {
    typedef image2d<bool> I;

    neighb2d nbh = c8();
    const int
      nrows = input.nrows(),
      ncols = input.ncols();

    bboxes.resize(1);
    mass_centers.resize(1);

    image2d<info>     data;
    image2d<V> label;
    V current_label = 0;
    int N, dp_border;

    util::timer time;
    time.start();

    // init
    {
      extension::adjust(input, nbh);
      N = input.nelements();
      dp_border = 2 * input.border();
      extension::fill(input, false);
      initialize(data, input);
    }

    float t = time;
    std::cout << "init = " << t << std::endl;
    time.restart();

    // 1st pass
    {
      util::array<int> dp = positive_offsets_wrt(input, nbh);
      const unsigned n_nbhs = dp.nelements();

      // Backward.
      unsigned p = input.index_of_point(point2d(nrows - 1, ncols - 1));
      for (int row = nrows - 1; row >= 0; --row, p -= dp_border)
	for (int col = ncols - 1; col >= 0; --col, --p)
	{
	  if (! input.element(p))
	    continue;

	  data.element(p).init(p, row, col); // init

	  for (unsigned i = 0; i < n_nbhs; ++i)
	  {
	    unsigned n = p + dp[i];
	    if (! input.element(n))
	      continue;
	    unsigned r = my_find_root(data, n);
	    if (r != p)
	    {
	      data.element(p).update( data.element(r) ); // update
	    }
	  }
	}
    }

    t = time;
    std::cout << "1st pass = " << t << std::endl;
    time.restart();

    // 2nd pass
    {
      initialize(label, input);
      data::fill(label, 0);

      // Forward.
      unsigned p = input.index_of_point(point2d(0, 0));
      for (int row = 0; row < nrows; ++row, p += dp_border)
	for (int col = 0; col < ncols; ++col, ++p)
	{
	  if (! input.element(p))
	    continue;
	  if (data.element(p).parent == p)
	  {
	    const info& dta = data.element(p);
	    if (dta.card > 5
		&& (dta.width() >= 1
		    && dta.height() >= 1))
	    {
	      label.element(p) = ++current_label;

	      bboxes.append(box2d(dta.p_min, dta.p_max));
	      mass_centers.append(point2d(dta.row_sum / dta.card,
					  dta.col_sum / dta.card));
	    }
	  }
	  else
	    label.element(p) = label.element(data.element(p).parent);
	}
    }
    t = time;
    std::cout << "2nd pass = " << t << std::endl;

    nlabels = current_label;
    return label;
  }



  template <typename V>
  image2d<V>
  extract_components_2(const image2d<bool>& input,
		       V& nlabels,
		       util::array<box2d>& bboxes,
		       util::array<point2d>& mass_centers)
  {
    typedef image2d<bool> I;

    neighb2d nbh = c8();
    const int
      nrows = input.nrows(),
      ncols = input.ncols();

    bboxes.resize(1);
    mass_centers.resize(1);

    image2d<info>     data;
    image2d<V> label;
    V current_label = 0;
    int N, dp_border;

    util::timer time;
    time.start();

    // init
    {
      extension::adjust(input, nbh);
      N = input.nelements();
      dp_border = 2 * input.border();
      extension::fill(input, false);
      initialize(data, input);
    }

    float t = time;
    std::cout << "init = " << t << std::endl;
    time.restart();



    // 1st pass
    {
      util::array<int> dp = positive_offsets_wrt(input, nbh);
      const unsigned n_nbhs = dp.nelements();

      mln_box_runend_piter_(I) sp(input.domain()); // Backward.
      unsigned ncols = sp.run_length();
      for_all(sp)
      {
	unsigned p = input.index_of_point(sp);  // Offset
	point2d site = sp;

	for (unsigned col = 0; col < ncols; ++col, --site[1], --p)
	{
	  if (! input.element(p))
	    continue;

	  data.element(p).init(p, site.row(), site.col()); // init

	  for (unsigned i = 0; i < n_nbhs; ++i)
	  {
	    unsigned n = p + dp[i];
	    if (! input.element(n))
	      continue;
	    unsigned r = my_find_root(data, n);
	    if (r != p)
	    {
	      data.element(p).update( data.element(r) ); // update
	    }
	  } // for_all(n)
	}
      } // for_all(p)

    } // end of 1st pass

    t = time;
    std::cout << "1st pass = " << t << std::endl;
    time.restart();

    // 2nd pass
    {
      initialize(label, input);
      data::fill(label, 0);

      // Forward.
      unsigned p = input.index_of_point(point2d(0, 0));
      for (int row = 0; row < nrows; ++row, p += dp_border)
	for (int col = 0; col < ncols; ++col, ++p)
	{
	  if (! input.element(p))
	    continue;
	  if (data.element(p).parent == p)
	  {
	    const info& dta = data.element(p);
	    if (dta.card > 5
		&& (dta.width() >= 1
		    && dta.height() >= 1))
	    {
	      label.element(p) = ++current_label;

	      bboxes.append(box2d(dta.p_min, dta.p_max));
	      mass_centers.append(point2d(dta.row_sum / dta.card,
					  dta.col_sum / dta.card));
	    }
	  }
	  else
	    label.element(p) = label.element(data.element(p).parent);
	}
    }
    t = time;
    std::cout << "2nd pass = " << t << std::endl;

    nlabels = current_label;
    return label;
  }





} // mln



void usage(char* argv[])
{
  std::cerr << argv[0] << " input.pbm out.pgm out2.pgm" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  if (argc != 4)
    usage(argv);

  using namespace mln;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);


  image2d<unsigned> ref;

//   {
//     util::timer t;
//     t.start();

//     unsigned nlabels;
//     ref = labeling::foreground(input, c4(), nlabels);

//     float ts = t.stop();
//     std::cout << "tufa: " << ts << "  " << nlabels << std::endl;
//   }

  util::array<box2d> bboxes(1, box2d(1,1));
  util::array<point2d> mass_centers(1, point2d(0,0));
  {
    util::timer t;
    t.start();

//     util::array<std::pair<box2d, point2d> > data_out(1);
    unsigned nlabels;
    image2d<unsigned> comps = extract_components(input, nlabels, bboxes, mass_centers);

    float ts = t.stop();
    std::cout << ts <<  "  " << nlabels << std::endl;

//     std::cout << bboxes << std::endl;
//     std::cout << mass_centers << std::endl;

//     if (comps != ref)
//       std::cout << "diff" << std::endl;

    io::pgm::save(labeling::wrap(value::int_u8(), comps),
		  argv[2]);
  }

  util::array<box2d> bboxes2(1, box2d(1,1));
  util::array<point2d> mass_centers2(1, point2d(0,0));

  {
    util::timer t;
    t.start();

//     util::array<std::pair<box2d, point2d> > data_out(1);
    unsigned nlabels;
    image2d<unsigned> comps = extract_components_2(input, nlabels, bboxes2, mass_centers2);

    float ts = t.stop();
    std::cout << ts <<  "  " << nlabels << std::endl;

//     std::cout << bboxes << std::endl;
//     std::cout << mass_centers << std::endl;

//     if (comps != ref)
//       std::cout << "diff" << std::endl;

    io::pgm::save(labeling::wrap(value::int_u8(), comps),
		  argv[3]);
  }

  mln_assertion(bboxes == bboxes2);
  mln_assertion(mass_centers == mass_centers2);
}
