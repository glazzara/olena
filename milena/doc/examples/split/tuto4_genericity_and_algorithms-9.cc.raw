  image2d<bool> lena_bw = binarization::binarization(lena, keep_specific_colors());
  value::label_8 nlabels;
  image2d<value::label_8> label = labeling::blobs(lena_bw, c8(), nlabels);
