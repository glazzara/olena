p_array<point2d> arr;

// The bbox is computed thanks to bbox() algorithm.
box2d box = geom::bbox(arr);
std::cout << box << std::endl;

// p_array provides nsites(),
// it can be retrieved in constant time.
std::cout << "nsites = " << arr.nsites() << std::endl;
