#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/all.hh>

#include <mln/draw/line.hh>

#include <mln/data/wrap.hh>
#include <mln/data/fill.hh>
#include <mln/data/convert.hh>


#include <mln/labeling/colorize.hh>
#include <mln/labeling/relabel.hh>
#include <mln/labeling/blobs.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/win/vline2d.hh>

#include <mln/logical/not.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/literal/colors.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/draw/box_plain.hh>

#include <mln/transform/influence_zone_geodesic.hh>

#include <mln/data/stretch.hh>

#include <mln/util/timer.hh>

#include <mln/norm/l1.hh>

#include <scribo/core/object_groups.hh>
#include <scribo/core/component_set.hh>
#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/extract/lines_h_pattern.hh>
#include <scribo/primitive/remove/separators.hh>

#include <scribo/preprocessing/denoise_fg.hh>
#include <scribo/preprocessing/rotate_90.hh>

#include <scribo/primitive/link/internal/dmax_default.hh>

#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio_aligned.hh>

#include <scribo/primitive/group/from_double_link_any.hh>

#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/filter/object_links_top_aligned.hh>
#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_links_bottom_aligned.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>
#include <scribo/debug/decision_image.hh>

namespace mln
{

  using namespace scribo;


  // Enable debug.
  bool _debug_;


  template <typename L>
  void filter_bad_groups(object_groups<L>& top_groups,
			 object_groups<L>& bot_groups)
  {
    const component_set<L>& comps = top_groups.components();
    const L& lbl = comps.labeled_image();

    for_all_groups(c, top_groups)
    {
      box2d b = comps(c).bbox();
      b.enlarge(0, comps(c).bbox().height());
      b.crop_wrt(lbl.domain());

      typedef mln_value(L) V;

      const V* top_ptr = & lbl(b.pmin());
      const V* bot_ptr = & lbl(point2d(b.pmax().row(), b.pmin().col()));

      unsigned ntop = 0, nbot = 0;
      for (unsigned n = 0; n < b.width(); ++n)
      {
	if (*top_ptr)
	  ++ntop;
	if (*bot_ptr)
	  ++nbot;
      }

      if (ntop / b.width() > 0.50f)
	top_groups(c) = c;

      if (nbot / b.width() > 0.50f)
	bot_groups(c) = c;
    }

  }


  template <typename L>
  mln_site(L)
  my_anchors(const component_set<L>& comps,
	     unsigned current_object,
	     anchor::Type anchor)
  {
    mln_site(L) sp;// = comps(current_object).bbox().pcenter();

    unsigned h = comps(current_object).bbox().height();

    switch (anchor)
    {
      default:
	return sp;


	// Bounding box top center
      case anchor::Top: // FIXME: rename as TopLeft
	sp.col() = comps(current_object).bbox().pmin().col();
	sp.row() = comps(current_object).bbox().pmin().row()
	  + math::min(2u, (h + 1) / 2 - 1);
	break;


	// Bounding box bottom center
      case anchor::Bottom: // FIXME: rename as BottomLeft
	sp.col() = comps(current_object).bbox().pmax().col();
	sp.row() = comps(current_object).bbox().pmax().row()
	  - math::min(2u, (h + 1) / 2 - 1);
	break;
    }

    return sp;
  }



  using namespace primitive::link::internal;

  template <typename L, typename E>
  class single_dmax_ratio_aligned_functor_base
    : public link_single_dmax_ratio_base<L, dmax_default, E>
  {
//    typedef single_right_dmax_ratio_aligned_functor<L> self_t;
    typedef link_single_dmax_ratio_base<L, dmax_default, E> super_;

  public:
    typedef mln_site(L) P;

    single_dmax_ratio_aligned_functor_base(
      const image2d<bool>& input,
      const component_set<L>& components,
      unsigned dmax,
      float min_angle,
      float max_angle,
      anchor::Type anchor_)
      : super_(components,
	       anchor::Horizontal,
	       dmax_default(dmax)),
	anchor(anchor_),
	debug_(data::convert(value::rgb8(), input)),
	debug_angle_(data::convert(value::rgb8(), input))
    {
      min_alpha_rad = (min_angle / 180.0f) * math::pi;
      max_alpha_rad = (max_angle / 180.0f) * math::pi;
    }

    void compute_next_site_(P& p)
    {
      ++p.col();
    }

    void compute_next_site_f_(unsigned& p)
    {
      ++p;
    }


    mln_site(L)
    start_point_(unsigned current_object, anchor::Type anchor)
    {
      return my_anchors(this->components_, current_object, anchor);
    }


    inline
    bool
    valid_link_(unsigned current_object,
		const P& start_point,
		const P& p)
    {
      if (!super_::valid_link_(current_object, start_point, p))
	return false;

      box<P> b = this->components_(current_object).bbox();

      // Distance between the two components.
      float dist;

      // current object is on the left.
      if (p[this->direction_] > b.pmax()[this->direction_])
	dist = math::abs(p[this->direction_] - b.pmax()[this->direction_]);
      // current object is on the right.
      else
	dist = math::abs(p[this->direction_] - b.pmin()[this->direction_]);


      int ldist = this->components_(current_object).bbox().width();

      // Components are really close, so the angle is more permissive.
      if (dist < 3 * ldist)
      {
	return
	  filter::internal::component_aligned_rad(this->components_,
						  current_object,
						  this->labeled_image_(p),
						  anchor,
						  max_alpha_rad);
      }


      // Components are really far, so the angle is less permissive.
      return
	filter::internal::component_aligned_rad(this->components_,
						current_object,
						this->labeled_image_(p),
						anchor,
						min_alpha_rad);
    }

    void validate_link_(unsigned current_object,
			const P& start_point,
			const P& p,
			anchor::Type anchor)
    {
      super_::validate_link_(current_object, start_point, p, anchor);

      if (_debug_)
      {
	mln_site(L)
	  p1 = mln::my_anchors(this->components_, current_object, anchor),
	  p2 = mln::my_anchors(this->components_, this->labeled_image_(p),
			       anchor);
	draw::line(debug_, p1, p2, literal::green);


	float
	  angle = filter::internal::alignment_angle(this->components_,
						    current_object,
						    this->labeled_image_(p),
						    anchor);
	angle = (angle * 180.0f) / math::pi;
	angle = angle * 20.0f + 1.0f;
	draw::line(debug_angle_, p1, p2, value::rgb8(angle, angle, angle));
      }
    }

    void invalidate_link_(unsigned current_object,
			  const P& start_point,
			  const P& p,
			  anchor::Type anchor)
    {
      super_::invalidate_link_(current_object, start_point, p, anchor);

      if (_debug_)
      {
	if (this->labeled_image_.domain().has(p) && this->labeled_image_(p) != 0)
	{
	  mln_site(L)
	    p1 = mln::my_anchors(this->components_, current_object, anchor),
	    p2 = mln::my_anchors(this->components_, this->labeled_image_(p),
				 anchor);
	  if (this->labeled_image_.domain().has(p2) && norm::l1_distance(p1.to_vec(), p2.to_vec()) < 300)
	  {
	    draw::line(debug_, p1, p2, literal::red);
	  }


	  float
	    angle = filter::internal::alignment_angle(this->components_,
						      current_object,
						      this->labeled_image_(p),
						      anchor);
	  angle = (angle * 180.0f) / math::pi;
	  angle = angle * 20.0f + 1.0f;
	  draw::line(debug_angle_, p1, p2, value::rgb8(angle, angle, angle));
	}
      }
    }


    float min_alpha_rad;
    float max_alpha_rad;
    anchor::Type anchor;


    image2d<value::rgb8> debug_;
    image2d<value::rgb8> debug_angle_;
  };


  template <typename L>
  class single_right_dmax_ratio_aligned_functor
    : public single_dmax_ratio_aligned_functor_base<L, single_right_dmax_ratio_aligned_functor<L> >
  {
    typedef single_right_dmax_ratio_aligned_functor<L> self_t;
    typedef single_dmax_ratio_aligned_functor_base<L, self_t> super_;

  public:
    typedef mln_site(L) P;

    single_right_dmax_ratio_aligned_functor(
      const image2d<bool>& input,
      const component_set<L>& components,
      unsigned dmax,
      float min_angle,
      float max_angle,
      anchor::Type anchor)
      : super_(input, components, dmax, min_angle, max_angle, anchor)
    {
    }

    void compute_next_site_(P& p)
    {
      ++p.col();
    }

    void compute_next_site_f_(unsigned& p)
    {
      ++p;
    }

  };


  template <typename L>
  class single_left_dmax_ratio_aligned_functor
    : public single_dmax_ratio_aligned_functor_base<L, single_left_dmax_ratio_aligned_functor<L> >
  {
    typedef single_left_dmax_ratio_aligned_functor<L> self_t;
    typedef single_dmax_ratio_aligned_functor_base<L, self_t> super_;

  public:
    typedef mln_site(L) P;

    single_left_dmax_ratio_aligned_functor(
      const image2d<bool>& input,
      const component_set<L>& components,
      unsigned dmax,
      float min_angle,
      float max_angle,
      anchor::Type anchor)
      : super_(input, components, dmax, min_angle, max_angle, anchor)
    {
    }

    void compute_next_site_(P& p)
    {
      --p.col();
    }

    void compute_next_site_f_(unsigned& p)
    {
      --p;
    }


  };

}


int main(int argc, char *argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc != 6 && argc != 7)
  {
    std::cout << "Usage: " << argv[0] << " <in.pbm> <min_angle = 3> <max_angle = 5> <min_card = 3> <out.ppm> <debug_enabled>" << std::endl;
    return 1;
  };

  _debug_ = (argc == 7);

  util::timer t;
  util::timer gt;

  // Load (OK)
  t.start();
  image2d<bool> input;
  mln::io::pbm::load(input, argv[1]);
  float t_ = t;
  std::cout << "Image loaded - " << t_ << std::endl;

  gt.start();

  // Closing structural - Connect characters.
  t.restart();

  win::hline2d vl(17);
  image2d<bool> input_clo = morpho::closing::structural(input, vl);

//  input_clo = scribo::preprocessing::rotate_90(input_clo, true);

  t_ = t;
  std::cout << "closing_structural - " << t_ << std::endl;

  if (_debug_)
  {
    // Restore input orientation.
    input = scribo::preprocessing::rotate_90(input, false);

    io::pbm::save(input_clo, "input_clo.pbm");
  }

  // Rotate (OK)
  t.restart();
  input_clo = scribo::preprocessing::rotate_90(input_clo, false);
  t_ = t;
  std::cout << "rotate_90 - " << t_ << std::endl;



  /// Finding components.
  typedef value::int_u16 V;
  typedef image2d<V> L;

  t.restart();
  V ncomponents;
  component_set<L>
    components = scribo::primitive::extract::components(input_clo, c8(),
							ncomponents);
  t_ = t;
  std::cout << "extract::components - " << t_ << std::endl;

  if (_debug_)
    io::pgm::save(data::convert(value::int_u8(), components.labeled_image()),
		  "lbl.pgm");


  unsigned min_angle = atoi(argv[2]), max_angle = atoi(argv[3]);

  unsigned dmax = 5;

  t.restart();
  object_links<L> top_right, bot_right;

  object_links<L> top_left, bot_left;


  // Top
  {
    // Right
    mln::single_right_dmax_ratio_aligned_functor<L>
      functor(input_clo, components, dmax, min_angle, max_angle, anchor::Top);
    top_right = primitive::link::impl::compute_fastest(functor, anchor::Top);

    t.stop();


    if (_debug_)
    {
      io::ppm::save(functor.debug_, "right_top.ppm");
      io::ppm::save(functor.debug_angle_, "right_top_angle.ppm");
    }

    t.resume();

    // Left
    mln::single_left_dmax_ratio_aligned_functor<L>
      lfunctor(input_clo, components, dmax, min_angle, max_angle, anchor::Top);
    top_left = primitive::link::compute(lfunctor, anchor::Top);


    t.stop();

    if (_debug_)
    {
      io::ppm::save(lfunctor.debug_, "left_top.ppm");
      io::ppm::save(lfunctor.debug_angle_, "left_top_angle.ppm");

      image2d<value::rgb8> output = duplicate(functor.debug_);
      data::paste((lfunctor.debug_ | (pw::value(lfunctor.debug_) != pw::cst(literal::black))) | (pw::value(lfunctor.debug_) != pw::cst(literal::white)), output);

      io::ppm::save(output, "left_right_top.ppm");
    }

    t.resume();
  }


  // Bottom
  {
    // Right
    mln::single_right_dmax_ratio_aligned_functor<L>
      functor(input_clo, components, dmax, min_angle, max_angle, anchor::Bottom);
    bot_right = primitive::link::compute(functor, anchor::Bottom);
    t.stop();

    if (_debug_)
    {
      io::ppm::save(functor.debug_, "right_bot.ppm");
      io::ppm::save(functor.debug_angle_, "right_bot_angle.ppm");
    }

    t.resume();

    // Left
    mln::single_left_dmax_ratio_aligned_functor<L>
      lfunctor(input_clo, components, dmax, min_angle, max_angle, anchor::Bottom);
    bot_left = primitive::link::compute(lfunctor, anchor::Bottom);
    t.stop();

    if (_debug_)
    {
      io::ppm::save(lfunctor.debug_, "left_bot.ppm");
      io::ppm::save(lfunctor.debug_angle_, "left_bot_angle.ppm");
    }

    if (_debug_)
    {
      image2d<value::rgb8> output = duplicate(functor.debug_);
      data::paste((lfunctor.debug_ | (pw::value(lfunctor.debug_) != pw::cst(literal::black))) | (pw::value(lfunctor.debug_) != pw::cst(literal::white)), output);

      io::ppm::save(output, "left_right_bot.ppm");
    }
  }


  t_ = t;
  std::cout << "links - " << t_ << std::endl;


  t.restart();
  std::cout << "group - top" << std::endl;
  object_groups<L>
    top_groups = primitive::group::from_double_link_any(top_left, top_right);
  std::cout << "group - bot" << std::endl;
  object_groups<L>
    bot_groups = primitive::group::from_double_link_any(bot_left, bot_right);
  t_ = t;
  std::cout << "group - " << t_ << std::endl;


  unsigned min_card = atoi(argv[4]);


  t.restart();
  util::array<accu::shape::bbox<point2d> >
    btop_accu(top_groups.nelements()),
    bbot_accu(bot_groups.nelements());


  for_all_groups(c, top_groups)
  {
    btop_accu(top_groups(c)).take(components(c).bbox());
    bbot_accu(bot_groups(c)).take(components(c).bbox());
  }
  t_ = t;
  std::cout << "groups to group bboxes - " << t_ << std::endl;



  if (_debug_)
  {

    image2d<value::rgb8>
      wo_filtering = data::convert(value::rgb8(), input);

    for_all_comp_data(d, btop_accu)
    {
      if (btop_accu(d).is_valid())
      {
	mln::draw::line(wo_filtering,
			btop_accu(d).to_result().pmin(),
			point2d(btop_accu(d).to_result().pmin().row(),
				btop_accu(d).to_result().pmax().col()),
			literal::green);

      }
    }

    for_all_comp_data(d, bbot_accu)
    {
      if (bbot_accu(d).is_valid())
      {
	mln::draw::line(wo_filtering,
			point2d(bbot_accu(d).to_result().pmax().row(),
				bbot_accu(d).to_result().pmin().col()),
			bbot_accu(d).to_result().pmax(),
			literal::green);
      }

    }
    io::ppm::save(wo_filtering, "wo_filtering.ppm");
  }


  t.restart();
  top_groups = filter::object_groups_small(top_groups, min_card);
  bot_groups = filter::object_groups_small(bot_groups, min_card);
  t_ = t;
  std::cout << "small groups - " << t_ << std::endl;



  t.restart();
  util::array<accu::shape::bbox<point2d> >
    top_accu(top_groups.nelements()),
    bot_accu(bot_groups.nelements());


  for_all_groups(c, top_groups)
  {
    top_accu(top_groups(c)).take(components(c).bbox());
    bot_accu(bot_groups(c)).take(components(c).bbox());
  }
  t_ = t;
  std::cout << "groups to group bboxes - " << t_ << std::endl;








  t.restart();
  image2d<bool> separators;
  initialize(separators, input_clo);
  data::fill(separators, false);
  t_ = t;
  std::cout << "Initialize separators image - " << t_ << std::endl;

  image2d<value::rgb8> both;

  if (_debug_)
    both = data::convert(value::rgb8(), input);



  t.restart();
  for_all_comp_data(d, top_accu)
  {
    if (top_accu(d).is_valid()  ||  btop_accu(d).is_valid())
    {
      if (top_accu(d).is_valid())
      {
	if (_debug_)
	  mln::draw::line(both,
			  top_accu(d).to_result().pmin(),
			  point2d(top_accu(d).to_result().pmin().row(),
				  top_accu(d).to_result().pmax().col()),
			  literal::green);

	mln::draw::line(separators,
			top_accu(d).to_result().pmin(),
			point2d(top_accu(d).to_result().pmin().row(),
				top_accu(d).to_result().pmax().col()),
			true);
      }
      else
	if (_debug_ && btop_accu(d).is_valid())
	  mln::draw::line(both,
			  btop_accu(d).to_result().pmin(),
			  point2d(btop_accu(d).to_result().pmin().row(),
				  btop_accu(d).to_result().pmax().col()),
			  literal::yellow);

    }
    if (bot_accu(d).is_valid() ||  bbot_accu(d).is_valid())
    {
      if (bot_accu(d).is_valid())
      {
	if (_debug_)
	  mln::draw::line(both,
			  point2d(bot_accu(d).to_result().pmax().row(),
				  bot_accu(d).to_result().pmin().col()),
			  bot_accu(d).to_result().pmax(),
			  literal::green);

	mln::draw::line(separators,
			point2d(bot_accu(d).to_result().pmax().row(),
				bot_accu(d).to_result().pmin().col()),
			bot_accu(d).to_result().pmax(),
			true);
      }
      else
	if (_debug_ && bbot_accu(d).is_valid())
	  mln::draw::line(both,
			  point2d(bbot_accu(d).to_result().pmax().row(),
				  bbot_accu(d).to_result().pmin().col()),
			  bbot_accu(d).to_result().pmax(),
			  literal::yellow);
    }

  }
  t_ = t;
  std::cout << "Drawing output image - " << t_ << std::endl;


  if (_debug_)
  {
    io::ppm::save(both, argv[5]);
    io::pbm::save(separators, "separators.pbm");
  }


  // Hit or miss
  {
    if (_debug_)
    {
      image2d<bool> input_with_seps = duplicate(input_clo);
      data::paste(separators | pw::value(separators), input_with_seps);

      io::pbm::save(input_with_seps, "input_with_seps.pbm");
    }

    t.restart();
    unsigned length = 25;

    dpoint2d
      dp1(-21, 0),
      dp2( 21, 0);

    // Adjusting extension.
    t.restart();
    extension::adjust_fill(input_clo, length / 2, 0);

    accu::count_value<bool> accu(true);
    typedef image2d<unsigned> I;

    I tmp = accu::transform_line(accu, input_clo, length, 1);
    t_ = t;
    std::cout << "* accu::transform_line - " << t_ << std::endl;

    if (_debug_)
      io::pgm::save(data::convert(value::int_u8(), tmp), "tmp.pgm");


    t.restart();
    value::int_u8 nlabels;
    image2d<value::int_u8>
      sep_lbl = labeling::value(separators, true, c8(), nlabels);
    t_ = t;
    std::cout << "* labeling seps - " << t_ << std::endl;


    t.restart();
//     util::array<unsigned> lcard(unsigned(nlabels) + 1, 0);
//     util::array<unsigned> lfalse(unsigned(nlabels) + 1, 0);

    util::array<bool> relbl(unsigned(nlabels) + 1, true);
    relbl(0) = false;


//     unsigned
//       tmp_next_line_dp1 = tmp.delta_index(dp1),
//       tmp_next_line_dp2 = tmp.delta_index(dp2);

//     const mln_value(I)* val_sep = &separators(separators.domain().pmin());
//     const value::int_u8* val_lbl = &sep_lbl(sep_lbl.domain().pmin());

//     bool go_next = false;
    unsigned invalid_ratio = unsigned(length * 0.30f);

//     for (unsigned row = 0; i < nrows; ++row)
//     {
//       for (unsigned col = 0; col < ncols; ++col)
//       {

//       }

//       val_sep += ;
//       val_lbl += ;
//     }

    mln_piter_(I) p(separators.domain());
    for_all(p)
      if (separators(p))
      {
	unsigned lbl = sep_lbl(p);
//	++lcard(lbl);

	unsigned
	  top_count = tmp(p + dp1),
	  bot_count = tmp(p + dp2);

	// This site is wrapped between two lines of text so we don't
	// want it.
	if (top_count >= invalid_ratio + 1
	    && bot_count >= invalid_ratio + 1)
	{
	  relbl(lbl) = false;
//	  go_next = true;
	}
//	  ++lfalse(lbl);
      }

//     for_all_comps(i, relbl)
//       relbl(i) = (lfalse(i) / (float) lcard(i)) < 0.02f;
    t_ = t;
    std::cout << "* reading data - " << t_ << std::endl;

    t.restart();
    labeling::relabel_inplace(sep_lbl, nlabels, relbl);
    t_ = t;
    std::cout << "* relabel_inplace - " << t_ << std::endl;

    image2d<bool> output = data::convert(bool(), sep_lbl);

    if (_debug_)
    {
      io::pbm::save(output, "separators_hom.pbm");
      io::pbm::save(separators, "separators_filtered.pbm");
    }
  }

  t_ = t;
  std::cout << "Separator image - " << t_ << std::endl;

  gt.stop();
  t_ = gt;
  std::cout << "Total time: " << t_ << std::endl;
}
