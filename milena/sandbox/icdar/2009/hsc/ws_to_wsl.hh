#include <cmath>

#include <mln/core/var.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/pw/all.hh>

#include <mln/value/int_u8.hh>

#include <mln/util/array.hh>
#include <mln/data/transform.hh>

#include <mln/accu/internal/base.hh>

#include <mln/algebra/vec.hh>
#include <mln/algebra/mat.hh>

#include <mln/make/region_adjacency_graph.hh>
#include <mln/make/p_vertices_with_mass_centers.hh>
#include <mln/debug/draw_graph.hh>

#include <mln/core/site_set/p_edges.hh>
#include <mln/core/site_set/p_vertices.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>

#include <mln/morpho/elementary/dilation.hh>

#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/literal/colors.hh>
#include <mln/data/convert.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/fun/i2v/array.hh>
#include <mln/data/transform.hh>
#include <mln/labeling/colorize.hh>




namespace mln
{


  struct mahalanobis : public mln::accu::internal::base< unsigned , mahalanobis >
  {
    typedef point2d argument;
    typedef unsigned result;

    mahalanobis()
    {
    }

    /// Manipulators.
    /// \{
    void init()
    {
      valid_ = false;
      n_ = 0;
      mean_.set_all(0);
      cov_.set_all(0);
    }

    void take(const argument& p)
    {
      mln_invariant(0);
    }

    void take(const mahalanobis& other)
    {
      mln_invariant(0);
    }

    void take(unsigned n_times, const argument& p)
    {
      n_ += n_times;
      float
	r = p.row(),
	c = p.col();
      mean_[0] += r * n_times;
      mean_[1] += c * n_times;
      cov_(0, 0) += r * r * n_times;
      cov_(0, 1) += r * c * n_times;
      cov_(1, 0) += c * r * n_times;
      cov_(1, 1) += c * c * n_times;
    }

    void finalize()
    {
      mln_invariant(! valid_);
      valid_ = true;
      if (n_ == 0)
	return;
      mean_ /= n_;
      cov_ = cov_ / n_ - mean_ * mean_.t();

      float
	a = cov_(0, 0), b = cov_(0, 1),
	c = cov_(1, 0), d = cov_(1, 1),
	r = a * d - b * c;

      float D = std::sqrt((a - d) * (a - d) + 4 * b * c);
      lambda1_ = 0.5 * (a + d + D);
      lambda2_ = 0.5 * (a + d - D);

      if (r > -0.001f && r < 0.001f) // too close to 0
	{
	  n_ = 0; // invalidation
	  return;
	}
      cov_1_(0, 0) = + d / r;
      cov_1_(0, 1) = - b / r;
      cov_1_(1, 0) = - c / r;
      cov_1_(1, 1) = + a / r;
    }

    /// Get the accumulator result; here DUMMY !!!
    unsigned to_result() const
    {
      mln_invariant(0);
      return n_;
    }

    /// Get the mean vector.
    const algebra::vec<2,float>& mean() const
    {
      mln_invariant(valid_);
      return mean_;
    }

    /// Get the center column.
    def::coord col() const
    {
      mln_invariant(valid_);
      return static_cast<def::coord>(0.49999f + mean_[1]); // col = coord #1
    }

    /// Get the covariance matrix.
    const algebra::mat<2,2,float>& cov() const
    {
      mln_invariant(valid_);
      return cov_;
    }

    /// Get the inverse of the covariance matrix.
    const algebra::mat<2,2,float>& cov_1() const
    {
      mln_invariant(valid_);
      return cov_1_;
    }

    float dist(const algebra::vec<2,float>& v) const
    {
      mln_invariant(valid_);
      algebra::vec<2,float> v_ = v - mean_;
      return v_.t() * cov_1_ * v_;
    }

    /// Get the number of items.
    unsigned n() const
    {
      return n_;
    }

    /// Check whether this accu is able to return a result.
    /// Always true here.
    bool is_valid() const
    {
      return valid_;
    }

    float lambda1() const { return lambda1_; }
    float lambda2() const { return lambda2_; }

  protected:

    bool valid_;
    unsigned n_;
    algebra::vec<2,float>   mean_;
    algebra::mat<2,2,float> cov_, cov_1_;
    float lambda1_, lambda2_;
  };



  template <typename L>
  util::array<mahalanobis>
  mahalanobis_from_images(const image2d<L>& ws,
			  const image2d<value::int_u8>& small,
			  L n_labels)
  {
    util::array<mahalanobis> arr(n_labels + 1);
    for (unsigned l = 0; l <= n_labels; ++l)
      arr[l].init();
    mln_piter(box2d) p(ws.domain());
    for_all(p)
    {
      if (ws(p) == 0 || small(p) == 255)
	continue;
      arr[ws(p)].take(255 - small(p), p);
    }

    for (unsigned l = 0; l <= n_labels; ++l)
      arr[l].finalize();

    return arr;
  }


  float dist(const mahalanobis& m1, const mahalanobis& m2)
  {
    return std::min(m1.dist(m2.mean()), m2.dist(m1.mean()));
  }


  template <typename L>
  inline
  L find_root_(util::array<L>& parent, L l)
  {
    if (parent[l] == l)
      return l;
    return parent[l] = find_root_(parent, parent[l]);
  }



  bool
  pass(const util::array<mahalanobis>& m,
       unsigned l, unsigned r)
  {
    float
      // vertical std deviations
      vs_l = std::sqrt(m[l].cov()(0, 0)),
      vs_r = std::sqrt(m[r].cov()(0, 0)),
      // vertical distance
      vd   = std::abs(m[l].mean()[0] - m[r].mean()[0]),
      // horizontal std deviations
      hs_l = std::sqrt(m[l].cov()(1, 1)),
      hs_r = std::sqrt(m[r].cov()(1, 1)),
      // horizontal means (column coordinates)
      hm_l = m[l].mean()[1],
      hm_r = m[r].mean()[1];

    bool
      v_criterion = (vd < 5 * std::sqrt(vs_l * vs_r)),   // FIXME: say 4?
      h_criterion = (hm_r - 1 * hs_r > hm_l + 1 * hs_l); // FIXME: say 1.5?

    return v_criterion && h_criterion;
  }



  template <typename L>
  image2d<L>
  ws_to_wslines(const image2d<bool>& input,
		const image2d<value::int_u8>& small,
		const image2d<L>& ws,
		L n_basins)
  {
    typedef util::graph G;

    G gr = make::region_adjacency_graph(ws, c8(), n_basins);

    util::array<mahalanobis> m;
    m = mahalanobis_from_images(ws, small, n_basins);

    typedef pw::cst_<int> P;
    P no_f(0);
    p_edges<G,P> pe(gr, no_f);


    mln_VAR(d, fun::i2v::array<float>(gr.e_nmax()) | pe);


    // Computing 'd'.

    {
      mln_piter_(d_t) e(pe);
      for_all(e)
      {
	unsigned
	  i1 = e.element().v1(),
	  i2 = e.element().v2();
	const mahalanobis
	  & m1 = m[i1],
	  & m2 = m[i2];

	if (m1.n() != 0 && m2.n() != 0)
	  d(e) = dist(m1, m2);
	else
	  d(e) = 666.f;

// 	if (m1.n() > 1000 && m2.n() > 1000) // FIXME: was 1000
// 	  d(e) = dist(m1, m2);
// 	else
// 	  d(e) = 666.f;

      }
    }


#ifdef LOG
    {
      image2d<value::rgb8> cool(small.domain());
      data::fill(cool, literal::white);
      data::fill((cool | (pw::value(ws) == pw::cst(0))).rw(),
		 literal::red);

      typedef p_vertices< G, fun::i2v::array<point2d> > pv_t;
      pv_t pv = make::p_vertices_with_mass_centers(ws, gr);

      debug::draw_graph(cool, pv,
			literal::green,
			literal::black);

      io::ppm::save(cool, "temp_cool.ppm");
    }
#endif // LOG


    util::array<unsigned>
      v_left (n_basins + 1),
      v_right(n_basins + 1);


    // Fill in v_left and v_right.

    {

      mln_vertex_iter_(G) v(gr);
      mln_vertex_nbh_edge_iter_(G) e(v);
      for_all(v)
      {
	int v_col = m[v.id()].col();
	float
	  d_left_min  = 666.f,
	  d_right_min = 666.f;
	unsigned
	  v_left_best  = 0,
	  v_right_best = 0;
	for_all(e)
	{
	  unsigned v2 = e.v_other(v);
	  float d_ = d.function()(e);
	  if (m[v2].col() < v_col)
	    {
	      // v2 <-- v
	      if (pass(m, v2, v.id())
		  && d_ < d_left_min)
		{
		  d_left_min  = d_;
		  v_left_best = v2;
		}
	    }
	  else
	    {
	      // v --> v2
	      if (pass(m, v.id(), v2)
		  && d_ < d_right_min)
		{
		  d_right_min  = d_;
		  v_right_best = v2;
		}
	    }
	}
	v_left [v.id()] = v_left_best;
	v_right[v.id()] = v_right_best;
      }

    }


    util::array<L> parent(n_basins + 1);

    // Computing parent.

    {
      for (L l = 0; l <= n_basins; ++l)
	parent[l] = l;

      for (L l = 1; l <= n_basins; ++l)
	{
	  if (v_right[l] == 0)
	    continue;
	  unsigned r = v_right[l];
	  if (v_left[r] != l)
	    continue;
	  parent[l] = r;
	}

      for (L l = 1; l <= n_basins; ++l)
	parent[l] = find_root_(parent, l);
    }


#ifdef LOG
    {
      using value::rgb8;

      fun::i2v::array<L> f_relab(n_basins + 1);
      f_relab(0) = 0;
      for (L l = 1; l <= n_basins; ++l)
	f_relab(l) = parent[l];

      image2d<L> ws_ = data::transform(ws, f_relab);
      image2d<rgb8> cool = labeling::colorize(rgb8(), ws_, n_basins);
      data::fill((cool | (pw::value(ws) == pw::cst(0))).rw(),
		 literal::white);
      mln_piter(box2d) p(small.domain());
      for_all(p)
      {
	if (small(p) == 255)
	  continue;
	float f = float(small(p)) / 255;
	cool(p).red()   = cool(p).red()   * f;
	cool(p).green() = cool(p).green() * f;
	cool(p).blue()  = cool(p).blue()  * f;
      }
      io::ppm::save(cool, "temp_wsl.ppm");

    }
#endif // LOG

    // Outputing.

    image2d<L> output(input.domain());

    {
      unsigned s = input.nrows() / small.nrows();

      image2d<L> ws_nol = morpho::elementary::dilation(ws, c8());

      mln_piter(box2d) p(input.domain());
      for_all(p)
	if (input(p))
	  output(p) = 0;
	else
	  {
	    L l = ws_nol.at_(p.row() / s, p.col() / s);
	    output(p) = parent[l];
	  }
    }

    return output;
  }

} // ! mln
