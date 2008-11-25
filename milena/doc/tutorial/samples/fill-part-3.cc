using namespace mln;
using value::label8;

// Find and label the different components.
label8 nlabels;
image2d<label8> lab = labeling::blobs(ima, c4(), nlabels);

// Fill the sites of component 2 with red.
fill(lab.domain(2).rw(), color::red);
