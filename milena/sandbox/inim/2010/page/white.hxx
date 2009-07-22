#ifndef WHITE_HXX
# define WHITE_HXX

# include <white.hh>

namespace whitespace
{

  template <typename P>
  bool is_box_ok (const box<P>& box)
  {
    return (box.len(0) < 50 && box.len(1) < 30)
      && (box.len(0) >= 5 || box.len(1) >= 5);
  }

  template <typename I>
  util::array< box<mln_psite(I)> > find_connected_components (I& ima)
  {
//    std::cout << "Computing connected components..." << std::endl;
    typedef value::label_16 V;
    V num_labels;
    mln_ch_value(I, V) labels = labeling::blobs (ima, c8(), num_labels);

//    std::cout << "Computing connected components bounding boxes..." << std::endl;
    typedef util::array< box<mln_psite(I)> > A;
    A components =
      labeling::compute (accu::shape::bbox<mln_psite(I)>(),
			 labels, num_labels);

//    std::cout << "Writing components image..." << std::endl;
    I out (ima.domain());
    mln_fwd_eiter(A) it (components);
    A final_components;
    for_all (it)
      if (is_box_ok (it.element()))
      {
	final_components.append (it.element());
	data::fill((out | it.element()).rw(), literal::one);
      }
//    io::pbm::save(out, "out.pbm");
    return final_components;
  }

  template <typename P>
  bool find_pivot (util::array< box<P> > components,
		   box<P> bound,
		   box<P>& pivot)
  {
    bool found = false;
    P bound_min = bound.pmin();
    P bound_max = bound.pmax();
    unsigned int best_dist = 0xffffffff; //bound_max[0] + bound_max[1];
    unsigned int bound_middle_x = (bound_max[0] + bound_min[0]) / 2;
    unsigned int bound_middle_y = (bound_max[1] + bound_min[1]) / 2;

    mln_eiter(util::array< box<P> >) it (components);
    for_all (it)
    {
      P it_min = it.element().pmin();
      P it_max = it.element().pmax();

      if (((bound.pmin()[0] <= it_max[0] &&
	    it_max[0] <= bound.pmax()[0]) ||
	   (bound.pmin()[0] <= it_min[0] &&
	    it_min[0] <= bound.pmax()[0])) &&
	  ((bound.pmin()[1] <= it_max[1] &&
	    it_max[1] <= bound.pmax()[1]) ||
	   (bound.pmin()[1] <= it_min[1] &&
	    it_min[1] <= bound.pmax()[1])))
//       if (inter (it.element(), bound).nsites() != 0)
      {
	found = true;
	unsigned int dist = abs(bound_middle_x - it_min[0]) +
	  abs(it_max[0] - bound_middle_x) +
	  abs(bound_middle_y - it_min[1]) +
	  abs(it_max[1] - bound_middle_y);
	if (dist < best_dist)
	{
	  best_dist = dist;
	  pivot = it.element();
	}
      }
    }
    return found;
  }

// Determinate whether the given box must be considered to search for
// whitespace
  template <typename P>
  bool searchable_for_whitespace (box<P> pbox, util::array< box<P> > others)
  {
    if (pbox.nsites() < 4000)
      return false;

    if (pbox.len (1) < 2 || pbox.len (0) < 10)
      return false;

    typedef util::array< box<P> >  A;
    mln_fwd_eiter(A) it (others);

    for_all(it)
    {
      unsigned int box_area = (pbox.pmax()[0] - pbox.pmin()[0]) *
	                      (pbox.pmax()[1] - pbox.pmin()[1]);

      unsigned int left_inter_x = std::max(pbox.pmin()[0], it.element().pmin()[0]);
      unsigned int right_inter_x = std::min(pbox.pmax()[0], it.element().pmax()[0]);
      unsigned int top_inter_x = std::min(pbox.pmax()[1], it.element().pmax()[1]);
      unsigned int bottom_inter_x = std::max(pbox.pmin()[1], it.element().pmin()[1]);

      if ((right_inter_x >= left_inter_x) && (top_inter_x >= bottom_inter_x))
      {
	unsigned int inter_area = (right_inter_x - left_inter_x) *
	                          (top_inter_x - bottom_inter_x);

	if (inter_area >= 0.8 * box_area)
	  return false;
      }
    }
    return true;
  }

// Returns a lower value for high quality rectangles
// For use with a minimal heap
  template <typename P>
  value::int_u32 quality (box<P> pbox)
  {
    // Not 0xffffffff because int_u32 constructor
    // takes int instead of uint
    return (0x7fffffff - pbox.nsites());
  }

  template <typename P>
  util::array< box<P> >
  find_whitespaces (util::array< box<P> > components,
		    box<P> bound)
  {
    typedef util::fibonacci_heap<value::int_u32, box<P> > H;
    H h;
    util::array< box<P> > res;
    h.push (quality(bound), bound);
    box<P> b;
    box<P> pivot;
    while (h.is_valid())
    {
      b = h.pop_front();
      if (find_pivot (components, b, pivot))
      {
	P pmin = b.pmin();
	P pmax = b.pmax();
	box<P> tmp_box;
	for (int i = 0; i < P::dim; ++i)
	{
	  if (pivot.pmax()[i] < b.pmax()[i])
	  {
	    pmin[i] = pivot.pmax()[i] + 1;
	    tmp_box = box<P>(pmin, pmax);
	    if (searchable_for_whitespace (tmp_box, res))
	      h.push (quality(tmp_box), tmp_box);
	    pmin[i] = b.pmin()[i];
	  }
	  if (pivot.pmin()[i] > b.pmin()[i])
	  {
	    pmax[i] = pivot.pmin()[i] - 1;
	    tmp_box = box<P>(pmin, pmax);
	    if (searchable_for_whitespace (tmp_box, res))
	      h.push (quality(tmp_box), tmp_box);
	    pmax[i] = b.pmax()[i];
	  }
	}
      }
      else
	// Found a white rectangle \o/
      {
	res.append (b);
      }
    }
    return res;
  }

  void whitespace_compute (image2d<bool>& ima,
                           image2d<bool>& cleaned)
  {
    typedef mln_psite_(image2d<bool>) P;

//  image2d<bool> ima = io::pbm::load (argv[1]);
    border::fill (ima, true);

    util::array< box<P> > components =
      whitespace::find_connected_components(ima);
//  std::cout << "Computing whitespace zones..." << std::endl;
    util::array< box<P> > final_components =
      whitespace::find_whitespaces (components,ima.domain());


// std::cout << "Merging whitespace zones..." << std::endl;
    image2d<bool> zones (ima.domain());
    data::fill (zones, literal::one);
    mln_fwd_eiter_(util::array< box<P> >) it (final_components);
    for_all (it)
      data::fill ((zones | it.element()).rw(), literal::zero);
    border::fill (zones, false);

//  std::cout << "Writing whitespace image..." << std::endl;
//  io::pbm::save(zones, "zones.pbm");

//  std::cout << "Cleaning object zones..." << std::endl;
    typedef value::label_16 V;
    V num_labels;
    image2d<V> labels =
      labeling::blobs (zones, c8(), num_labels);
//  io::pgm::save (labeling::wrap(labels), "labels.pgm");

    util::array< box<P> > boxes =
      labeling::compute (accu::shape::bbox<P>(), labels, num_labels);

    labeling::relabel_inplace (labels, num_labels,
                               CheckBoxSize<P, V>(boxes));
    labeling::pack_inplace (labels, num_labels);
//  io::pgm::save (labeling::wrap(labels), "labels2.pgm");

    image2d<value::int_u32> size_label =
      labeling::compute_image (accu::math::sum<V, value::int_u32>(),
                               labels, labels, num_labels);

    cleaned = mln::binarization::threshold (size_label, 1);

//  std::cout << "Writing final image..." << std::endl;
//  io::pbm::save (cleaned, "white.pbm");
  }
} // End of namespace whitespace

#endif /* !WHITE_HXX */
