
namespace mln
{


  // From parent image to children image.

  template <typename I, typename E, typename L>
  mln_ch_value(I, std::vector<mln_psite(I)>)
    compute_children(const I& epar, const util::array<E>& edge, L l_max, std::vector<E>& roots)
  {
    typedef std::vector<mln_psite(I)> C; // Children.
    mln_ch_value(I,C) chl;
    initialize(chl, epar);

    mln_ch_value(I,bool) deja_vu;
    initialize(deja_vu, epar);
    data::fill(deja_vu, false);

    for (L l = 1; l <= l_max; ++l)
      {
	E e = edge[l];
	while (deja_vu(e) == false)
	  {
	    deja_vu(e) = true;
	    if (epar(e) != e)
	      {
		chl(epar(e)).push_back(e);
		e = epar(e);
	      }
	    else
	      roots.push_back(e);
	  }
      }

    return chl;
  }


  /// Compute an LCA
  /// Reference article: The LCA Problem Revisited, M. A. Bender and M.
  /// Farach-Colton, May 2000
  ///
  /// See also Topological Watershed, Alexandre Abraham, LRDE CSI report
  /// 0821, June 2008
  ///
  template <typename L, typename I, typename E>
  class lca_t
  {

  public:

    /// Constructor
    lca_t(const L& l_max_,
	const I& edge_children,
	const std::vector<E>& roots)
    {
      unsigned l_max = l_max_.next();
      euler_tour_edge_.resize(2 * l_max + 1);
      euler_tour_depth_.resize(2 * l_max + 1);
      initialize (euler_position_, edge_children);

      // BUILD EULER TOUR
      std::stack<E> to_treat;
      for (unsigned i = 0; i < roots.size(); ++i)
	to_treat.push(roots[i]);

      unsigned euler_edge = 0;
      mln_ch_value(I, bool) deja_vu;
      initialize(deja_vu, edge_children);
      data::fill(deja_vu, false);

      while (!to_treat.empty())
      {
	E e = to_treat.top();
	to_treat.pop();

	euler_tour_edge_[euler_edge] = e;
	if (deja_vu(e))
	  euler_tour_depth_[euler_edge] = euler_tour_depth_[euler_edge - 1] - 1;
	else
	  euler_tour_depth_[euler_edge] = euler_tour_depth_[euler_edge - 1] + 1;

	if (!deja_vu(e))
	{
	  for (int j = edge_children(e).size() - 1; j >= 0; --j)
	  {
	    to_treat.push(e);
	    to_treat.push(edge_children(e)[j]);
	  }
	  deja_vu(e) = true;
	  euler_position_(e) = euler_edge;
	}

	++euler_edge;
      }

      // BUILD MINIMAS
      int size = 2 * l_max - 1;
      int logn = (int)(ceil(log((double)(size))/log(2.0)));

      minim_ = new int [logn * size]; // FIXME : Think about freeing this
      Minim_ = new int* [logn];

      Minim_[0] = minim_;

      // Init
      for (unsigned i = 0; i < size - 1; ++i)
	if (euler_tour_depth_[i] < euler_tour_depth_[i+1])
	{
	  Minim_[0][i] = i;
	} else {
	  Minim_[0][i] = i+1;
	}

      int k;
      for (int j = 1; j < logn; j++)
      {
	k = 1 << (j - 1);
	Minim_[j] = &minim_[j * size];
	for (int i = 0; i < size; i++)
	{
	  if ((i + (k << 1)) < size)
	  {
	    if (euler_tour_depth_[Minim_[j - 1][i]] <= euler_tour_depth_[Minim_[j - 1][i + k]])
	      Minim_[j][i] = Minim_[j - 1][i];
	    else
	      Minim_[j][i] = Minim_[j - 1][i + k];
	  }
	}
      }
    }


    /// Destructor
    ~lca_t()
    {
      delete[] Minim_;
      delete[] minim_;
    }


    const E& operator() (const E& a, const E& b)
    {
      if (a == b)
	return a;

      int m = euler_position_(a),
	  n = euler_position_(b),
	  k;

      if (m > n)
	std::swap(m, n);

      k = (int)(log((double)(n - m))/log(2.));

      if (euler_tour_depth_[Minim_[k][m]]
	    < euler_tour_depth_[Minim_[k][n - (1 << k)]])
	return euler_tour_edge_[Minim_[k][m]];
      else
	return euler_tour_edge_[Minim_[k][n - (1 << k)]];
    }

  private:
    int *minim_;
    int **Minim_;
    std::vector<E> euler_tour_edge_;
    std::vector<unsigned> euler_tour_depth_;
    mln_ch_value(I, unsigned) euler_position_;
  };


} // mln
