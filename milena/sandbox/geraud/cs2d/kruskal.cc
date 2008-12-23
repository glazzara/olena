# include <vector>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image_if_value.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/alias/window2d.hh>
# include <mln/convert/to_window.hh>

# include <mln/debug/println.hh>
# include <mln/debug/iota.hh>
# include <mln/fun/p2v/iota.hh>
# include <mln/data/paste.hh>
# include <mln/data/fill.hh>
# include <mln/morpho/gradient.hh>
# include <mln/morpho/meyer_wst.hh>

# include <mln/level/sort_points.hh>

# include <mln/io/pgm/load.hh>
# include <mln/io/ppm/save.hh>
# include <mln/value/int_u8.hh>
# include <mln/pw/all.hh>
# include <mln/win/rectangle2d.hh>

# include <mln/value/rgb8.hh>
# include <mln/literal/black.hh>
# include <mln/literal/white.hh>
# include <mln/literal/colors.hh>

# include <mln/labeling/regional_minima.hh>

# include "dbl_neighb.hh"



// KRUSKAL-MST(G, w) 
//   T := Ø 
//   for each vertex u in V 
//     MAKE-SET(DS, u) 
//   end for
//   for each edge (u,v) in E in order of nondecreasing weight 
//     if FIND-SET(DS, u) != FIND-SET(DS, v) 
//       UNION-SET(DS, u, v) 
//       T := T U {(u,v)} 
//   end for
//   return T



struct is_cell_t :  mln::Function_p2b<is_cell_t>
{
  typedef bool result;
  bool operator()(const mln::point2d& p) const
  {
    return p.row() % 2 == 0 && p.col() % 2 == 0;
  }
}
  is_cell;

struct is_edge_t :  mln::Function_p2b<is_edge_t>
{
  typedef bool result;
  bool operator()(const mln::point2d& p) const
  {
    return p.row() % 2 + p.col() % 2 == 1;
  }
}
  is_edge;

struct is_point_t :  mln::Function_p2b<is_point_t>
{
  typedef bool result;
  bool operator()(const mln::point2d& p) const
  {
    return p.row() % 2 && p.col() % 2;
  }
}
  is_point;


struct is_row_odd_t
  {
    bool operator()(const mln::point2d& p) const
    {
      return p.row() % 2;
    }
  } is_row_odd;


template<typename T_t>
struct is_tree_t :  mln::Function_p2b< is_tree_t<T_t> >
{
  typedef bool result;
  bool operator()(const mln::point2d& p) const
  {
    return p.row() % 2 + p.col() % 2 == 1 && T(p) == true;
  }
  T_t T;
};

namespace mln
{

  template <typename E>
  image2d<value::rgb8> show_edge(const E& edge,
				 unsigned nrows,unsigned ncols,
				 unsigned clen)
  {
    mln_precondition(clen % 2); // oddity

    typedef win::hline2d H;
    H hline(clen);

    typedef win::vline2d V;
    V vline(clen);

    box2d b(nrows * clen + 3 * (nrows - 1),
	    ncols * clen + 3 * (ncols - 1));
    image2d<value::rgb8> output(b);
    data::fill(output, literal::black);


    //   0 1 2 3 4
    // 0 c e c e c
    // 1 e x e x e
    // 2 c e c e c

    //     0 1 2 3 4 5 6 7 8 9 10

    //  0  c c c . e . c c c . e . c c c
    //  1  c c c . e . c c c . e . c c c
    //  2  c c c . e . c c c . e . c c c
    //  3  . . . . . . . . . . . . . . .
    //  4  e e e . x . e e e . x . e e e
    //  5  . . . . . . . . . . . . . . .
    //  6  c c c . e . c c c . e . c c c
    //  7  c c c . e . c c c . e . c c c
    //  8  c c c . e . c c c . e . c c c

    // Edges.
    mln_piter(E) e(edge.domain());
    for_all(e)
      {
	value::rgb8 v;
	if (edge(e))
	  v = literal::red;
	else
	  v = literal::black;
	point2d e_ = e, p_;
	if (e_.row() % 2) // Odd => horizontal.
	  {
	    p_.row() = clen + 1 + (clen + 3) * (e_.row() / 2); // vertex-like
	    p_.col() = clen / 2 + (clen + 3) * (e_.col() / 2); // cell-like
	    mln_qiter(V) q(hline, p_);
	    for_all(q) if (output.has(q))
	      output(q) = v;
	  }
	else // Even => vertical.
	  {
	    p_.row() = clen / 2 + (clen + 3) * (e_.row() / 2); // cell-like
	    p_.col() = clen + 1 + (clen + 3) * (e_.col() / 2); // vertex-like
	    mln_qiter(H) q(vline, p_);
	    for_all(q) if (output.has(q))
	      output(q) = v;
	  }
      }

    return output;
  }

} // mln


#define mln_VAR(Var, Expr)  typeof(Expr) Var = Expr;


template <typename DS_t, typename U>
void make_set(DS_t& DS, const U& u)
{
  DS(u) = u;
}


template <typename DS_t, typename U>
U find_set(DS_t& DS, U u)
{
  if (DS(u) == u) // is root
    return u;
  return DS(u) = find_set(DS, DS(u));
}


template <typename DS_t, typename U>
void union_set(DS_t& DS, const U& u, const U& v)
{
  U ru = find_set(DS, u),
    rv = find_set(DS, v);
  mln_precondition(ru != rv);
  if (rv > ru)
    DS(ru) = rv;
  else
    DS(rv) = ru;
}



int main()
{
  using namespace mln;
  using value::int_u8;

  window2d c4 = convert::to_window(mln::c4());


  typedef dbl_neighb_<dpoint2d, is_row_odd_t> nbh_t;
  nbh_t nbh_e2c, win_p_e2c, nbh_e2e;

  {
    bool e2c_h[] = { 0, 1, 0,
		     0, 0, 0,
		     0, 1, 0 };

    bool e2c_v[] = { 0, 0, 0,
		     1, 0, 1,
		     0, 0, 0 };
    nbh_e2c
      .when_true (make::neighb2d(e2c_h))
      .when_false(make::neighb2d(e2c_v));

    win_p_e2c = nbh_e2c;
    win_p_e2c
      .insert_true(dpoint2d(0,0))
      .insert_false(dpoint2d(0,0));

    bool e2e_h[] = { 0, 0, 1, 0, 0,
		     0, 1, 0, 1, 0,
		     0, 0, 0, 0, 0,
		     0, 1, 0, 1, 0,
		     0, 0, 1, 0, 0 };

    bool e2e_v[] = { 0, 0, 0, 0, 0,
		     0, 1, 0, 1, 0,
		     1, 0, 0, 0, 1,
		     0, 1, 0, 1, 0,
		     0, 0, 0, 0, 0 };
    nbh_e2e
      .when_true (make::neighb2d(e2e_h))
      .when_false(make::neighb2d(e2e_v));
  }

  typedef image2d<int_u8> I;

  image2d<int_u8> input;
  io::pgm::load(input, "fly.pgm");

  image2d<int_u8> ima(input.nrows() * 2 - 1,
		      input.ncols() * 2 - 1, 0);

  mln_VAR(cell, ima | is_cell);
  typedef typeof(cell) cell_t;
  debug::println(cell);

  {
    mln_fwd_piter_(I) pi(input.domain());
    mln_fwd_piter_(cell_t) pc(cell.domain());
    for_all_2(pi, pc)
      cell(pc) = input(pi);
  }

  mln_VAR(edge, ima | is_edge);
  data::paste(morpho::gradient(edge, nbh_e2c), edge);
  //                                  ^^^^^^^
  //                         edge -> neighboring cells

  debug::println(edge);

  typedef typeof(edge) edge_t;

  typedef p_array<point2d> Arr;
  Arr E = level::sort_points_increasing(edge);

  // Aux data.
  mln_ch_value_(edge_t, bool) T;
  initialize(T, edge);
  data::fill(T, false);

  mln_ch_value_(cell_t, point2d) DS;
  initialize(DS, cell);

  {
    mln_piter_(cell_t) u(cell.domain());
    for_all(u) // in V
      make_set(DS, u); 
  }

  {
    mln_fwd_piter_(Arr) uv(E);
    for_all(uv)
      {
	point2d
	  uv_ = uv.to_point(),
	  u, v;
	if (uv_.row() % 2)
	  u = uv_ + up, v = uv_ + down;
	else
	  u = uv_ + left, v = uv_ + right;
  	if (find_set(DS, u) != find_set(DS, v))
	  {
	    union_set(DS, u, v);
	    T(uv) = true;
	  }
      }
  }

  debug::println(T);

  mln_VAR(is_T, pw::value(T) == pw::cst(true));

  mln_VAR(dom, (T | is_T).domain());
  typedef typeof(dom) dom_t;

  io::ppm::save(show_edge(T, input.nrows(), input.ncols(), 7),
		"edge.ppm");


  typedef typeof(T) T_t;
  is_tree_t<T_t> is_tree;
  is_tree.T = T;
  // [*] See EOF.

  unsigned nbasins;
  mln_VAR(wst, morpho::meyer_wst(ima | is_tree, nbh_e2e, nbasins) );
  std::cout << "nbasins : " << nbasins << std::endl;

  {
    mln_piter_(dom_t) e(dom);
    mln_niter_(nbh_t) n(nbh_e2c, e);
    for_all(e)
      if (wst(e) != 0)
	for_all(n)
	  cell(n) = wst(e);
    debug::println(cell);
  }

}

// [*] FIXME: "edge | is_T" does not properly work because of
// initialization failure of the predicate function (pw expr do not
// have ctor without arg so the init cannot be delayed).  As a
// consequence, we cannot have the following code:
//
//   mln_VAR(tree, edge | is_T);
//   typedef typeof(tree) tree_t;
//   morpho::meyer_wst(tree, nbh_e2e, nbasins);
