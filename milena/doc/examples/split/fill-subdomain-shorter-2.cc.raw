  label_8 nlabels;
  image2d<label_8> lab = labeling::blobs(ima, c4(), nlabels);

  image2d<rgb8> ima2;
  initialize(ima2, ima);
  data::fill(ima2, literal::black);

  data::fill((ima2 | (pw::value(lab) == pw::cst(2u))).rw(), literal::red);
