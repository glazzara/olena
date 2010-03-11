#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/util/array.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/fill.hh>
#include <mln/data/wrap.hh>

#include <mln/util/timer.hh>



namespace scribo
{

  namespace text
  {

    namespace internal
    {

      using namespace mln;
      using value::int_u8;



      template <typename T, typename T2>
      void draw_box(image2d<T>& input, const box2d& b, T2 l)
      {
//     data::fill((input | b).rw(), l);
	const unsigned
	  delta = input.delta_index(dpoint2d(1,0)),
	  nrows = b.nrows(),
	  ncols = b.ncols();
	T* p_start = & input(b.pmin());
	T* ptr = p_start;
	for (unsigned r = 0; r < nrows; ++r)
	{
	  ptr = p_start;
	  for (unsigned c = 0; c < ncols; ++c)
	    *ptr++ = l;
	  p_start += delta;
	}
      }



      template <typename T, typename T2>
      void draw_box(image2d<T>& input,
		    int pmin_row, int pmin_col,
		    int pmax_row, int pmax_col,
		    T2 l)
      {
	if (pmax_row < pmin_row || pmax_col < pmin_col)
	  return;

	const unsigned
	  input_nrows_1 = input.nrows() - 1,
	  input_ncols_1 = input.ncols() - 1;
	if (pmin_row < 0) pmin_row = 0;
	if (pmin_col < 0) pmin_col = 0;
	if (pmax_row > input_nrows_1) pmax_row = input_nrows_1;
	if (pmax_col > input_ncols_1) pmax_col = input_ncols_1;

	const unsigned
	  delta = input.delta_index(dpoint2d(1,0)),
	  nrows = pmax_row - pmin_row + 1,
	  ncols = pmax_col - pmin_col + 1;
	T* p_start = & input.at_(pmin_row, pmin_col);
	T* ptr = p_start;
	for (unsigned r = 0; r < nrows; ++r)
	{
	  ptr = p_start;
	  for (unsigned c = 0; c < ncols; ++c)
	    *ptr++ = l;
	  p_start += delta;
	}
      }



      unsigned my_find_root(util::array<unsigned>& parent, unsigned x)
      {
	if (parent[x] == x)
	  return x;
	return parent[x] = my_find_root(parent, parent[x]);
      }

      void swap_ordering(unsigned l1, unsigned l2)
      {
	if (l2 > l1)
	  return;
	unsigned l1_ = l1;
	l1 = l2;
	l2 = l1_;
      }


      box2d union_of(const box2d& b1, const box2d& b2)
      {
	box2d b(point2d(std::min(b1.pmin().row(), b2.pmin().row()),
			std::min(b1.pmin().col(), b2.pmin().col())),
		point2d(std::max(b1.pmax().row(), b2.pmax().row()),
			std::max(b1.pmax().col(), b2.pmax().col())));
	return b;
      }


      template <typename L>
      unsigned do_union(scribo::line_set<L>& lines,
			unsigned l1,
			unsigned l2,
			util::array<unsigned>& parent)
      {
	unsigned
	  l1r = my_find_root(parent, l1),
	  l2r = my_find_root(parent, l2);
	if (l1r == l2r)
	{
	  std::cout << '.' << std::endl;
	  return l1r;
	}

//    swap_ordering(l1, l2);

//     parent[l2] = l1; // The smallest label value is root.
//     lines(l1).fast_merge(lines(l2));

	if (lines(l1).card() < lines(l2).card())
	  std::swap(l1, l2);

	parent[l2] = l1; // The text line with the most component count is root.
	lines(l1).fast_merge(lines(l2));

//	looks_like_a_text_line_[l1] = looks_like_a_text_line(lines(l1));


	// l1's tag is automatically set to line::Needs_Precise_Stats_Update
	// l2's tag is automatically set to line::Merged

	// looks_like_txt_line[l1] = looks_like_txt_line[l1] || looks_like_txt_line[l2];
	// merge of "approximate stats": (n, (n x sum))  =>  useful for computing the enlarging delta

	return l1;
      }


      box2d enlarge(const box2d& b, int delta)
      {
	box2d b_(point2d(b.pmin().row(), b.pmin().col() - delta),
		 point2d(b.pmax().row(), b.pmax().col() + delta));
	return b_;
      }


      void compute_data(// in
	unsigned tl, unsigned tr, unsigned bl, unsigned br,
	// out
	unsigned& l1, unsigned& l2, unsigned& l3, unsigned& l4,
	unsigned& count_non_zero,
	unsigned& count_different_labels)
      {
	l1 = l2 = l3 = l4 = 0;
	count_non_zero = 0;
	if (tl)
	{
	  l1 = tl;
	  ++count_non_zero;
	}
	if (tr)
	{
	  if (! l1) l1 = tr;
	  else if (tr != l1) l2 = tr;
	  ++count_non_zero;
	}
	if (bl)
	{
	  if (! l1) l1 = bl;
	  else if (! l2) { if (bl != l1) l2 = bl; }
	  else if (bl != l1 && bl != l2) l3 = bl;
	  ++count_non_zero;
	}
	if (br)
	{
	  if (! l1) l1 = br;
	  else if (! l2) { if (br != l1) l2 = br; }
	  else if (! l3) { if (br != l1 && br != l2) l3 = br; }
	  else if (br != l1 && br != l2 && br != l3) l4 = br;
	  ++count_non_zero;
	}

	if (l1 == 0)
	  count_different_labels = 0;
	else if (l2 == 0)
	  count_different_labels = 1;
	else if (l3 == 0)
	  count_different_labels = 2;
	else if (l4 == 0)
	  count_different_labels = 3;
	else
	  count_different_labels = 4;

	if (count_different_labels == 0)
	  std::cerr << "bug 0" << std::endl;
      }


      template <typename L>
      bool looks_like_a_text_line(const scribo::line_info<L>& l)
      {
	return
	  /*l.card() > 2                                 //     suffisient cardinality
	  && */l.bbox().height() > 20                    // and suffisient height
	  && l.bbox().width() > 50                     // and suffisient width
	  && l.bbox().width() > 2 * l.bbox().height(); // and horizontal-like.

	// FIXME:
	// if (n_comps > 2)
	//   return true;
	// FIXME: Refine?
      }


      template <typename L>
      int delta_of_line(const scribo::line_info<L>& l)
      {
	return l.char_width() /* / 2 */ + l.char_space();
      }


      template <typename L>
      void draw_enlarged_box(image2d<unsigned>& output,
			     const scribo::line_info<L>& l)
      {
	if (looks_like_a_text_line(l))
	  draw_box(output, enlarge(l.bbox(), delta_of_line(l)), l.id());
	else
	  draw_box(output, l.bbox(), l.id());
      }


      template <typename L>
      bool lines_can_merge(const scribo::line_info<L>& l1, const scribo::line_info<L>& l2)
      {
	float hratio = float(l1.bbox().height()) / float(l2.bbox().height());
	float xhratio = float(l1.x_height()) / float(l2.x_height());
	if ((hratio < 0.5 || hratio > 2.) && (xhratio < 0.8 || xhratio > 1.25)) // too different heights
	  return false;

	unsigned
	  col1 = l1.bbox().pcenter().col(),
	  col2 = l2.bbox().pcenter().col();
	if (col1 < col2)
	  return col1 + l1.bbox().width() / 4  <  col2 - l2.bbox().width() / 4;
	else
	  return col2 + l2.bbox().width() / 4  <  col1 - l1.bbox().width() / 4;
      }



//   struct line_info<L>
//   {
//     bool looks_like_a_text_line; // pre-computed
//     int delta_of_line; // pre-computed before each merging pass

//     // FIXME:
//     //
//     // Important note: after merging two lines, we draw the
//     // merged line over the existing one; we have to ensure that we
//     // cover the previous rectangle (otherwise we have a label in
//     // 'output' that is not used anymore! and it can mix up the
//     // detection of upcoming merges...)  so this delta has to remain
//     // the same during one pass.  Another solution (yet more costly)
//     // could be of erasing the previous rectangle before re-drawing...
//   };


      template <typename L>
      image2d<unsigned>
      one_merge_pass(unsigned ith_pass,
		     const box2d& domain,
		     std::vector<scribo::line_id_t>& v,
		     scribo::line_set<L>& lines,
		     util::array<unsigned>& parent,
		     util::array<bool>& looks_like_a_text_line_,
		     bool last_pass = false)
      {
	image2d<unsigned> canvas(domain);
	data::fill(canvas, 0);

	image2d<value::int_u8> log(domain);
	data::fill(log, 0);

	unsigned
	  count_new = 0,
	  count_include = 0,
	  count_merge = 0,
	  count_grow = 0,
	  count_bad_3 = 0,
	  count_bad_4 = 0;

	unsigned

	  count_1_grow_ok = 0,
	  count_1_grow_KO = 0,
	  count_1_merge_ok = 0,
	  count_1_merge_KO = 0,
	  count_1_other = 0,

	  count_2_merge_hyphen_ok = 0,
	  count_2_merge_hyphen_KO = 0,
	  count_2_merge_all_ok = 0,
	  count_2_merge_all_KO = 0,
	  count_2_merge_other = 0;

	const unsigned n = lines.nelements();
	unsigned l_;

	for (int i = n - 1; i >= 0; --i)
	{
	  unsigned l = v[i];

	  if (parent[l] != l) // not a root, so has already merged, thus ignore it
	    continue;

	  box2d b = lines(l).bbox();

	  unsigned tl, tr, bl, br;

	  {
	    box2d b_ =
	      looks_like_a_text_line_(lines(l).id()) ?
	      enlarge(b, delta_of_line(lines(l))) :
	      b;
	    tl = canvas(b_.pmin());
	    tr = canvas.at_(b_.pmin().row(), b_.pmax().col());
	    bl = canvas.at_(b_.pmax().row(), b_.pmin().col());
	    br = canvas(b_.pmax());
	  }

// 	{
// 	  tl = canvas(b.pmin());
// 	  tr = canvas.at_(b.pmin().row(), b.pmax().col());
// 	  bl = canvas.at_(b.pmax().row(), b.pmin().col());
// 	  br = canvas(b.pmax());
// 	}

	  if (tl == tr && bl == br && tl == bl) // Same behavior for all corners.
	  {
	    if (tl != 0)
	    {
	      // Main case: it is an "included" box (falling in an already drawn box)
	      ++count_include;

	      // Merge lines #tl and #l.
	      l_ = do_union(lines, tl, l, parent);
	      // We have to re-draw the original largest line since
	      // it may change of label (take the one of the included line).
	      draw_enlarged_box(canvas, lines(l_));

	      // Log:
	      draw_box(log, b, 128);
	    }
	    else
	    {
	      // Main case: it is a "new" box, to be drawn in the background.
	      ++count_new;

	      // Extra test:
	      if (canvas(b.pcenter()) == 0)
		// confirmation that we are not in this rare pathological case:
		//
		//        o---o
		//   +----|---|---------+
		//   |    | ? |         |
		//   +----|---|---------+
		//        o---o
	      {
		draw_enlarged_box(canvas, lines(l));
		// Log:
		draw_box(log, b, 127);
	      }
	      else
	      {
		// FIXME:
		// We have to tag as pathological the line l.
		if (last_pass)
		  lines(l).update_tag(scribo::line::Pathological);
		std::cout << "pathological ''new box''" << std::endl;
	      }
	    }
	  }
	  else
	  {
	    // Particular cases.
	    unsigned
	      l1, l2, l3, l4,
	      count_non_zero, count_different_labels;

	    compute_data(// in
	      tl, tr, bl, br,
	      // out
	      l1, l2, l3, l4,
	      count_non_zero, count_different_labels);


	    if (count_different_labels == 0)
	      std::cout << "bug 0000" << std::endl;


	    if (count_different_labels == 1 && count_non_zero == 3)
	    {
	      //  +----------+
	      //  |  x--x    |
	      //  |  |  |    |
	      //  |  x--0 !! |
	      //  +----------+
	      std::cout << "bug 1-3" << std::endl;
	    }

	    // FIXME: Handle the case where count_different_labels ==
	    // 2 && count_non_zero == 3?


	    if (count_different_labels == 4)
	    {
	      ++count_bad_4;

	      // FIXME:
	      // We have to tag as pathological the line l.
	      if (last_pass)
		lines(l).update_tag(scribo::line::Pathological);

	      // Log:
	      draw_box(log, b, 254);
	    }


	    if (count_different_labels == 3)
	    {
	      ++count_bad_3;

	      // FIXME:
	      // We have to tag as pathological the line l.
	      if (last_pass)
		lines(l).update_tag(scribo::line::Pathological);

	      // Log:
	      draw_box(log, b, 253);
	    }


	    if (count_different_labels == 1)  // Usually a "grow" case thanks to comma, hyphen, etc.
	    {
	      ++count_grow;

	      if (count_non_zero == 0 || count_non_zero == 4)
		std::cerr << "bug 1-04" << std::endl;

	      // We have to test if we shall merge!

	      bool
		l_is_line  = looks_like_a_text_line_(lines(l).id()),
		l1_is_line = looks_like_a_text_line_(lines(l1).id());

	      if (! l_is_line && l1_is_line)
	      {
		// This is the classical "grow" case: a small component
		// makes a larger one grow.

		// l should be a comma, hyphen, etc. of the line l1
		// so we try to make line l1 grow.
		if (lines(l).bbox().height() < lines(l1).bbox().height()
		    && lines(l).bbox().width() < lines(l1).bbox().width())
		{
		  // line l height and width are resp. smaller than line l1 height and width.
		  ++count_1_grow_ok;
		  l_ = do_union(lines, l, l1, parent);
		  draw_enlarged_box(canvas, lines(l_));
		  // Log:
		  draw_box(log, b, 100);
		}
		else
		{
		  ++count_1_grow_KO;

		  //    ,-.
		  //    | | l etait une fois...
		  //    | |
		  //    | | bla bla bla.
		  //    | |
		  //    | | bla bla bla
		  //    `-'
		  //
		  // It is usually the case of a dropped initial...
		  //
		  // FIXME: Do we want to set this line as pathological?

		  if (last_pass)
		    lines(l).update_tag(scribo::line::Pathological);

		  draw_box(canvas, b, l);

		  // Log:
		  draw_box(log, b, 101);
		}
		// It seems better to distinguish between the
		// different cases : if the height of line l is
		// larger than the width, I can be a comma so I
		// only merge if my location is at the bottom of
		// line l1, and so on.
	      }

	      else if (l_is_line && l1_is_line)
	      {
		// Less common case than the above "grow" one:
		// two lines merge.

		if (lines_can_merge(lines(l), lines(l1)))
		{
		  ++count_1_merge_ok;
		  l_ = do_union(lines, l, l1,  parent);
		  draw_enlarged_box(canvas, lines(l_));
		  // Log:
		  draw_box(log, b, 110);
		}
		else
		{
		  ++count_1_merge_KO;
		  if (last_pass)
		    lines(l).update_tag(scribo::line::Pathological);

		  // Log:
		  draw_box(log, b, 111);
		}
	      }
	      else
	      {
		++count_1_other;
		// Log:
		draw_box(log, b, 120);
	      }
	    }


	    if (count_different_labels == 2)
	    {

	      bool
		l_is_line  = looks_like_a_text_line_(lines(l).id()),
		l1_is_line = looks_like_a_text_line_(lines(l1).id()),
		l2_is_line = looks_like_a_text_line_(lines(l2).id());

	      if (! l_is_line && l1_is_line && l2_is_line)
	      {
		// Typically an hyphen joins two tines (two chunks of the same line).
		if (lines_can_merge(lines(l1), lines(l2)))
		{
		  ++count_2_merge_hyphen_ok;
		  do_union(lines, l1, l2, parent);
		  l_ = do_union(lines, l, l2,  parent);
		  draw_enlarged_box(canvas, lines(l_));
		  // Log:
		  draw_box(log, b, 80);
		}
		else
		{
		  ++count_2_merge_hyphen_KO;
		  if (last_pass)
		    lines(l).update_tag(scribo::line::Pathological);
		  // Log:
		  draw_box(log, b, 81);
		}
	      }
	      else if (l_is_line && l1_is_line && l2_is_line)
	      {
		// Typically three chunks of the same line.
		if (lines_can_merge(lines(l1), lines(l2))
		    && lines_can_merge(lines(l), lines(l1))
		    && lines_can_merge(lines(l), lines(l2)))
		{
		  ++count_2_merge_all_ok;
		  do_union(lines, l1, l2, parent);
		  l_ = do_union(lines, l, l2,  parent);
		  draw_enlarged_box(canvas, lines(l_));
		  // Log:
		  draw_box(log, b, 90);
		}
		else
		{
		  ++count_2_merge_all_KO;
		  if (last_pass)
		    lines(l).update_tag(scribo::line::Pathological);
		  // Log:
		  draw_box(log, b, 91);
		}
	      }
	      else
	      {
		++count_2_merge_other;
		if (last_pass)
		  lines(l).update_tag(scribo::line::Pathological);
		// Log:
		draw_box(log, b, 255);
	      }

	      // FIXME: Perhaps there is only one merge...

	    }

	  }
	}

	std::cout
	  << "new = " << count_new << std::endl
	  << "inc = " << count_include << std::endl
	  << "mrg = " << count_merge << std::endl
	  << "grw = " << count_grow << std::endl
	  << "bd3 = " << count_bad_3 << std::endl
	  << "bd4 = " << count_bad_4 << std::endl;

	std::cout
	  << std::endl
	  << "#1 grow  ok = " << count_1_grow_ok << std::endl
	  << "#1 grow  KO = " << count_1_grow_KO << std::endl
	  << "#1 merge ok = " << count_1_merge_ok << std::endl
	  << "#1 merge KO = " << count_1_merge_KO << std::endl
	  << "#1 other    = " << count_1_other << std::endl;

	std::cout
	  << std::endl
	  << "#2 merge hyphen ok = " << count_2_merge_hyphen_ok << std::endl
	  << "#2 merge hyphen KO = " << count_2_merge_hyphen_KO << std::endl
	  << "#2 merge all ok    = " << count_2_merge_all_ok << std::endl
	  << "#2 merge all KO    = " << count_2_merge_all_KO << std::endl
	  << "#2 merge other     = " << count_2_merge_other << std::endl;

	if (ith_pass == 1)
	  mln::io::pgm::save(log, "log_1.pgm");
	else if (ith_pass == 2)
	  mln::io::pgm::save(log, "log_2.pgm");
	else if (ith_pass == 3)
	  mln::io::pgm::save(log, "log_3.pgm");

	return canvas;
      }



      template <typename L>
      struct order_lines_id
      {
	order_lines_id(const scribo::line_set<L>& lines)
	  : lines_(lines)
	{
	}

	bool operator()(const scribo::line_id_t& l1, const scribo::line_id_t& l2) const
	{
	  if (lines_(l1).bbox().nsites() == lines_(l2).bbox().nsites())
	    return l1 < l2;
	  return lines_(l1).bbox().nsites() < lines_(l2).bbox().nsites();
	}

	scribo::line_set<L> lines_;
      };

      template <typename L>
      scribo::line_set<L>
      draw_boxes(const box2d& input_domain,
		 const scribo::line_set<L>& lines_)
      {
	scribo::line_set<L> lines = lines_.duplicate();
	const unsigned n = lines.nelements();

	order_lines_id<L> func(lines);
	std::vector<scribo::line_id_t> v;
	v.reserve(n);

	// Union-find parent data, used to merge lines.
	util::array<unsigned> parent(n + 1);

	// Initialize data
	parent(0) = 0;
	for (unsigned l = 1; l < parent.nelements(); ++l)
	{
	  v.push_back(l);
	  parent[l] = l;
	}

	// Sort lines by bbox.nelements() and ids.
	std::sort(v.begin(), v.end(), func);

	image2d<unsigned> canvas;
	util::timer t;


	// Caching whether a line looks like a text line.
	util::array<bool>
	  looks_like_a_text_line_(unsigned(lines.nelements()) + 1);
	for_all_lines(l, lines)
	  looks_like_a_text_line_[l] = looks_like_a_text_line(lines(l));


	// First pass
	t.start();
	one_merge_pass(1, input_domain, v, lines,
		       parent, looks_like_a_text_line_);
	float ts = t.stop();
	std::cout << "time " << ts << std::endl;

	// Second pass
	t.start();
	canvas = one_merge_pass(2, input_domain, v, lines,
				parent, looks_like_a_text_line_,
				true); // <- last pass
	ts = t.stop();
	std::cout << "time " << ts << std::endl;


	using value::int_u8;
	//io::pgm::save(data::wrap(int_u8(), canvas), "merge_result.pgm");

	mln::io::ppm::save(labeling::colorize(value::rgb8(), canvas),
		      "merge_result.ppm");


	return lines;
      }

    } // end of namespace scribo::text::internal



    // Facade

    template <typename L>
    line_set<L>
    merging(const box2d& input_domain,
	    const scribo::line_set<L>& lines)
    {
      using namespace mln;

      util::timer t;
      t.start();

      scribo::line_set<L> output = internal::draw_boxes(input_domain, lines);
      float ts = t.stop();
      std::cout << ts << std::endl;

      return output;
    }


  } // end of namespace scribo::text

} // end of namespace scribo
