using namespace mln;
using value::int_u8;
using value::rgb8;

// Find and label the different components.
int_u8 nlabels;
image2d<int_u8> lab = labeling::blobs(ima, c4(), nlabels);

// Store a boolean image. True if the site is part of
// component 2, false otherwise.
mln_VAR(lab_2b, lab | (pw::value(lab) == 2));

// Get the sub site set containing only the sites
// part of component 2.
mln_VAR(lab_2, lab_2b.domain(true));

// Fill the sites of component 2 with red.
fill(lab_2, color::red);
