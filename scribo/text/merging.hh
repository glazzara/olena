#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/util/array.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/fill.hh>
#include <mln/data/wrap.hh>

#include <mln/make/box2d.hh>

#include <mln/util/timer.hh>



namespace scribo
{

  namespace text
  {

    namespace internal
    {

      using namespace mln;
      using value::int_u8;


      template <typename L>
      inline
      int delta_of_line(const scribo::line_info<L>& l)
      {
	return l.char_width() + l.char_space();
      }


      template <typename L>
      inline
      bool looks_like_a_text_line(const scribo::line_info<L>& l)
      {
	return
	  l.card() >= 3                  // at least 3 components
	  && l.bbox().height() > 10      // and minimal height
	  && l.bbox().width() > l.bbox().height(); // and more horizontal-like than vertical
	// FIXME: Later on, add a criterion based on the number
	// of alignments (on top and bot).
      }


      /*!

       */
      template <typename L>
      struct group_data_t
      {
	group_data_t()
	  : info(0)
	{
	}

	group_data_t(const scribo::line_info<L>& info_)
	  : info(&info_)
	{
	  finalize();
	}

	const scribo::line_info<L>* info;

	// deduced:
	int meanline;
	bool looks_like_a_line;
	unsigned delta;
	box2d ebox;

	void finalize()
	{
	  if (info->x_height() == 0)
	    std::cerr << "oops" << std::endl;

	  meanline = info->baseline() - int(info->x_height()) + 1;

	  looks_like_a_line = looks_like_a_text_line(*info);

	  // delta = looks_like_a_line ? char_space + char_width : 0;
	  delta = delta_of_line(*info);
	  // FIXME: choose between:
	  //   char_width + char_space
	  //   2 * char_width
	  //   char_width + 2 * char_space

	  int A = info->a_height() - info->x_height(), D = - info->d_height();
	  if (A <= 2 && D > 2)
	    A = D;
	  if (D <= 2 && A > 2)
	    D = A;
	  ebox = mln::make::box2d(meanline - A, info->bbox().pmin().col() - delta,
				  info->baseline() + D, info->bbox().pmax().col() + delta);
	}

      };

      template <typename T, typename T2>
      void draw_box(image2d<T>& input, const box2d& b, T2 l)
      {
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
      unsigned do_union(util::array<group_data_t<L> >& dta,
			scribo::line_set<L>& lines,
			unsigned l1,
			unsigned l2,
			util::array<unsigned>& parent)
      {
	l1 = my_find_root(parent, l1);
	l2 = my_find_root(parent, l2);
	if (l1 == l2)
	  return l1;

	swap_ordering(l1, l2);
	parent[l2] = l1; // The smallest label value is root.

	if (lines(l2).card() > lines(l1).card())
	{
          // we transfer data from the largest item to the root one.
	  scribo::line_info<L> tmp = lines(l1);
	  lines(l1) = lines(l2);
	  lines(l1).fast_merge(tmp);

	  // We must set manually the tag for lines(l2) since it is
	  // not used directly in merge process so its tag cannot be
	  // updated automatically.
	  lines(l2).update_tag(line::Merged);
	}
	else
	  lines(l1).fast_merge(lines(l2));

	dta[l1].finalize();

	// l1's tag is automatically set to line::Needs_Precise_Stats_Update
	// l2's tag is automatically set to line::Merged

	return l1;
      }


      box2d enlarge(const box2d& b, int delta)
      {
	box2d b_(point2d(b.pmin().row(), b.pmin().col() - delta),
		 point2d(b.pmax().row(), b.pmax().col() + delta));
	return b_;
      }


      template <typename L>
      void draw_enlarged_box(image2d<unsigned>& output,
			     const util::array<group_data_t<L> >& dta,
			     unsigned l)
      {
	box2d b = dta[l].ebox;
	b.crop_wrt(output.domain());
	draw_box(output, b, l);
      }


      /*! \brief Check whether two lines can merge.

	Criterions:
	- Height ratio must be <= 1.7
	- Baselines delta must be <= 3
	- Boxes must not overlap too much.

      */
      template <typename L>
      bool lines_can_merge(const scribo::line_info<L>& l1,
			   const scribo::line_info<L>& l2)
      {
	// Parameters.
	const float x_ratio_max = 1.7, baseline_delta_max = 3;

	// Similarity of x_height.
	{
	  float x1 = l1.x_height(), x2 = l2.x_height();
	  float x_ratio = std::max(x1, x2) / std::min(x1, x2);
	  if (x_ratio > x_ratio_max)
	    return false;
	}

	// Same baseline.
	{
	  if (std::abs(l1.baseline() - l2.baseline()) > baseline_delta_max)
	    return false;
	}

	// left / right
	unsigned
	  col1 = l1.bbox().pcenter().col(),
	  col2 = l2.bbox().pcenter().col();
	if (col1 < col2)
	  return col1 + l1.bbox().width() / 4  <  col2 - l2.bbox().width() / 4;
	else
	  return col2 + l2.bbox().width() / 4  <  col1 - l1.bbox().width() / 4;
      }


      template <typename L>
      int horizontal_distance(const scribo::line_info<L>& l1,
			      const scribo::line_info<L>& l2)
      {
	if (l1.bbox().pcenter().col() < l2.bbox().pcenter().col())
	  return l2.bbox().pmin().col() - l1.bbox().pmax().col();
	else
	  return l1.bbox().pmin().col() - l2.bbox().pmax().col();
      }


      /*! \brief Check whether a non line component and a line can merge.

	Criterions:
	- Small height (c.height < l.x_height)
	- Character width mean in 'c' must be lower than the character
        width median of 'l'. (c.width / c.ncomps < l.char_width)

	OR

	- Small height (c.height < l.x_height)
	- Not so long width (c.width < 5 * l.char_width)
	- Aligned with the 'x' center ((l.baseline + l.meanline / 2) - c.center.row < 7)
	- tiny spacing (horizontal distance < 5)

      */
      template <typename L>
      bool non_line_and_line_can_merge(const scribo::line_info<L>& l_cur,
				       const group_data_t<L>& dta_cur, // current
				       const scribo::line_info<L>& l_ted,
				       const group_data_t<L>& dta_ted) // touched
      {
	if (dta_cur.looks_like_a_line || ! dta_ted.looks_like_a_line)
	  return false;
	// the current object is a NON-textline
	// the background (touched) object is a textline


	// FIXME: THERE IS A BUG
	// The second condition should be replaced by the commented one.
	//
	// General case (for tiny components like --> ',:."; <--):
	if (l_cur.bbox().height() < l_ted.x_height()
	    && float(l_cur.char_width()) / float(l_cur.card()) < l_ted.char_width())
//	    && float(l_cur.bbox().width()) / float(l_cur.card()) < l_ted.char_width())
	  return true;


	// Special case for '---':
	if (// small height:
	  l_cur.bbox().height() < l_ted.x_height()
	  // // not so long width:
	  && l_cur.bbox().width() < 5 * l_ted.char_width()
	  // align with the 'x' center:
	  && std::abs((l_ted.baseline() + dta_ted.meanline) / 2 - l_cur.bbox().pcenter().row()) < 7
	  // tiny spacing:
	  && horizontal_distance(l_cur, l_ted) < 5
	  )
	{
	  return true;
	}


	// Special case

//     // FIXME: Box are aligned; the main problem is that we can have multiple valid box
//     // depending on the presence of descent and/or ascent!
//     if (std::abs(dta_cur.box.pmin().row() - dta_ted.box.pmin().row()) < 5     // top
// 	&& std::abs(dta_cur.box.pmax().row() - dta_ted.box.pmax().row()) < 5  // bot
// 	&& ((dta_ted.box.pcenter().col() < dta_cur.box.pcenter().col()  &&  dta_cur.box.pmin().col() - dta_ted.box.pmax().col() < 10)     // small distance when cur is at right
// 	    || (dta_cur.box.pcenter().col() < dta_ted.box.pcenter().col()  &&  dta_ted.box.pmin().col() - dta_cur.box.pmax().col() < 10)) // or when is at left.
// 	)
//       return true;


	return false;

	// The unused criterion below is too restrictive; it does not work
	// for ending '-', and neither for ',' when there's no descent.
	//       dta_ted.box.has(dta_cur.box.pcenter())
      }


      /*! \brief Merge text lines.

	This algorithm iterates over all the components ordered by size.
	It uses a 2d labeled image, tmp_box, to draw component bounding
	boxes and uses that image to check bounding box collisions.
	Depending on that collisions and whether the component looks like
	a text line or not, bounding boxes are merged.

	\verbatim
	ALGORITHM:
	for each component 'cur' in decreasing order
	  if already merged
	    continue

	    ///
	    /// x-----------x
	    /// |           |
	    /// x     x     x
	    /// |           |
	    /// x-----------x
	    ///

	    Set labels <- Every labels corresponding to the colliding bounding
	                  boxes (uses only the 7 sites detailled above).

	    If label.card == 1
	      l = label.get(0);
	      If l != background
	        If looks_like_a_line(cur)
		  If looks_like_a_line(l)
		    // Error case: a line is included in a line.
		  else
		    // Line cur is included in a frame or a drawing.
		    draw_enlarged_box(l)
		  end
		else
		  If looks_like_a_line(l)
		  // Component cur is a punctuation overlapping with line l.
		  l_ <- do_union(cur, l)
		  draw_enlarged_box(l_)
		end
	      end
	    else
	      If looks_like_a_line(cur)
	        // Component cur is a new line.
		draw_enlarged_box(l)
	      end
	    end
	  else
	    for each label l in labels
	      If l == background
	        continue
	      end

	      If lines_can_merge(cur, l)
	        l_ <- do_union(cur, l)
		draw_enlarged_box(l_)
		continue
	      end

	      If !looks_like_a_line(cur) and looks_like_a_line(l)
	     	If non_line_and_line_can_merge(cur, l)
		  // A punctuation merge with a line
		  l_ <- do_union(cur, l)
		  draw_enlarged_box(l_)
		  continue
		else
		  // None
		end
	      else
	      // Error case
	    end
	  end

	  \endverbatim
      */
      // FIXME:
      //
      // Important note: after merging two lines, we draw the
      // merged line over the existing one; we have to ensure that we
      // cover the previous rectangle (otherwise we have a label in
      // 'output' that is not used anymore! and it can mix up the
      // detection of upcoming merges...)  so this delta has to remain
      // the same during one pass.  Another solution (yet more costly)
      // could be of erasing the previous rectangle before re-drawing...
      //
      template <typename L>
      image2d<unsigned>
      one_merge_pass(unsigned ith_pass,
		     const box2d& domain,
		     std::vector<scribo::line_id_t>& v,
		     scribo::line_set<L>& lines,
		     util::array<group_data_t<L> >& dta,
		     util::array<unsigned>& parent)
      {
	image2d<unsigned> output(domain);
	data::fill(output, 0);

	image2d<value::int_u8> log(domain);
	data::fill(log, 0);

	const unsigned n = dta.nelements() - 1;
	unsigned l_;

	unsigned
	  count_txtline_IN_txtline = 0,
	  count_txtline_IN_junk = 0,
	  count_two_lines_merge = 0,
	  count_new_txtline = 0,
	  count_comp_IN_txtline = 0,
	  count_comp_HITS_txtline = 0,
	  count_WTF = 0;

	for (int i = n - 1; i >= 0; --i)
	{
	  unsigned l = v[i];

	  if (parent[l] != l) // not a root, so has already merged, thus ignore it
	    continue;

	  box2d b = lines(l).bbox();

	  unsigned tl, tr, ml, mc, mr, bl, br;

	  {
	    box2d b_;

	    b_ = enlarge(lines(l).bbox(), dta[l].delta);
	    b_.crop_wrt(output.domain());


	    /*
	      tl             tr
	      x---------------x
	      |               |
	      |       mc      |
	   ml x       x       x mr
	      |               |
	      |               |
	      x---------------x
	      bl             br

	    */


	    tl = output(b_.pmin());
	    tr = output.at_(b_.pmin().row(), b_.pmax().col());
	    ml = output.at_(b_.pcenter().row(), b_.pmin().col());
	    mc = output.at_(b_.pcenter().row(), b_.pcenter().col());
	    mr = output.at_(b_.pcenter().row(), b_.pmax().col());
	    bl = output.at_(b_.pmax().row(), b_.pmin().col());
	    br = output(b_.pmax());
	  }

	  typedef std::set<unsigned> set_t;
	  std::set<unsigned> labels;
	  labels.insert(tl);
	  labels.insert(tl);
	  labels.insert(tr);
	  labels.insert(ml);
	  labels.insert(mc);
	  labels.insert(mr);
	  labels.insert(bl);
	  labels.insert(br);


	  if (labels.size() == 1) // Same behavior for all ancors.
	  {
	    if (mc != 0)
	    {
	      // Main case: it is an "included" box (falling in an already drawn box)

	      if (dta[l].looks_like_a_line)
	      {
		if (dta[mc].looks_like_a_line)
		{
		  ++count_txtline_IN_txtline;
		  std::cout << "weird: inclusion of a txt_line in a txt_line!" << std::endl;
		}
		else
		{
		  ++count_txtline_IN_junk;

		  // a non-line (probably a drawing or a frame) includes a line
		  draw_enlarged_box(output, dta, l);
		  // Log:
		  draw_box(log, b, 100);
		}
	      }
	      else // the current object is NOT a line
	      {
		if (dta[mc].looks_like_a_line)
		{
		  ++count_comp_IN_txtline;
		  // FIXME: critere petouille a ajouter ici

		  // Merge non-line #l into line #mc.
		  l_ = do_union(dta, lines, mc, l, parent);
		  // We have to re-draw the original largest line since
		  // it may change of label (take the one of the included line).
		  draw_enlarged_box(output, dta, l_);

		  // Log:
		  draw_box(log, b, 128);
		}
	      }
	    }
	    else
	    {
	      // Main case: it is a "new" box, that might be drawn in the background.

	      // we only draw this box if it is a text-line!!!
	      if (dta[l].looks_like_a_line)
	      {
		++count_new_txtline;
		draw_enlarged_box(output, dta, l);
		// Log:
		draw_box(log, b, 127);
	      }
	      else
		draw_box(log, b, 1);
	    }
	  }
	  else
	  {
	    // Particular cases.
	    for (set_t::const_iterator it = labels.begin();
		 it != labels.end();
		 ++it)
	    {
	      unsigned lcand = *it;

	      if (lcand == 0) // Skip background.
		continue;

	      if (lines_can_merge(lines(l), lines(lcand)))
	      {
		++count_two_lines_merge;
		l_ = do_union(dta, lines, l, lcand,  parent);
		draw_enlarged_box(output, dta, l_);
		// Log:
		draw_box(log, b, 151);
		continue;
	      }


	      if (! dta[l].looks_like_a_line && dta[lcand].looks_like_a_line)
	      {
		++count_comp_HITS_txtline;
		if (non_line_and_line_can_merge(lines(l), dta[l], lines(lcand), dta[lcand]))
		  // a petouille merges with a text line?
		{
		  ++count_comp_HITS_txtline;
		  l_ = do_union(dta, lines, l, lcand,  parent);
		  draw_enlarged_box(output, dta, l_);
		  // Log:
		  draw_box(log, b, 169);
		  continue;
		}
		else
		{
		  // Log:
		  draw_box(log, b, 254);
		}
	      }
	      else
	      {
		++count_WTF;
		// Log:
		draw_box(log, b, 255);
	      }

	    }
	  }

	}


	std::cout
	  << "   new txtline        = " << count_new_txtline        << std::endl
	  << "   comp IN txtline    = " << count_comp_IN_txtline    << std::endl
	  << "   2 lines merge      = " << count_two_lines_merge    << std::endl
	  << "   comp HITS txtline  = " << count_comp_HITS_txtline  << std::endl
	  << "   txtline IN junk    = " << count_txtline_IN_junk    << std::endl
	  << "   txtline IN txtline = " << count_txtline_IN_txtline << std::endl
	  << "   WTF!               = " << count_WTF << std::endl;


	(void) ith_pass;
// 	if (ith_pass == 1)
// 	{
// 	  mln::io::pgm::save(log, "log_1.pgm");
// 	  mln::io::pgm::save(data::wrap(int_u8(), output), "log_1e.pgm");
// 	}
// 	else if (ith_pass == 2)
// 	{
// 	  mln::io::pgm::save(log, "log_2.pgm");
// 	  mln::io::pgm::save(data::wrap(int_u8(), output), "log_2e.pgm");
// 	}
// 	else if (ith_pass == 3)
// 	{
// 	  mln::io::pgm::save(log, "log_3.pgm");
// 	  mln::io::pgm::save(data::wrap(int_u8(), output), "log_3e.pgm");
// 	}

	return output;

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


	// Caching line temporary data.
	util::array<group_data_t<L> >
	  dta;
	dta.reserve(unsigned(lines.nelements()) + 1);
	dta.append(group_data_t<L>());
	for_all_lines(l, lines)
	  dta.append(group_data_t<L>(lines(l)));

	// First pass
	t.start();
	one_merge_pass(1, input_domain, v, lines, dta, parent);
	float ts = t.stop();
	std::cout << "time " << ts << std::endl;

	// Second pass
	t.start();
	canvas = one_merge_pass(2, input_domain, v, lines, dta, parent); // <- last pass
	ts = t.stop();
	std::cout << "time " << ts << std::endl;


// 	using value::int_u8;
	//io::pgm::save(data::wrap(int_u8(), canvas), "merge_result.pgm");

// 	mln::io::ppm::save(labeling::colorize(value::rgb8(), canvas),
// 		      "merge_result.ppm");


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
