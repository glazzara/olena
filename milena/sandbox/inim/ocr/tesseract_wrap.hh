#ifndef TESSERACT_WRAP_HH
# define TESSERACT_WRAP_HH

# include <cassert>

# include <mln/core/image/image2d.hh>

# include <tesseract/baseapi.h>

struct TessWrap : public TessBaseAPI
{
  static int InitWithLanguage(const char* datapath, const char* outputbase,
        const char* language, const char* configfile,
        bool numeric_mode, int argc, char* argv[])
  {
    return TessBaseAPI::InitWithLanguage(datapath, outputbase,
					 language, configfile,
					 numeric_mode, argc, argv);
  }

  static char* TesseractRect(const unsigned char* imagedata,
                             int bytes_per_pixel,
                             int bytes_per_line,
                             int left, int top, int width, int height,
			     float* score)
  {
    int kMinRectSize = 10; // Quick and dirty... (value imported from tesseract src)
    if (width < kMinRectSize || height < kMinRectSize)
      return NULL;  // Nothing worth doing.

    // Copy/Threshold the image to the tesseract global page_image.
    CopyImageToTesseract(imagedata, bytes_per_pixel, bytes_per_line,
	left, top, width, height);

    { // RecognizeToString
      BLOCK_LIST*    block_list = FindLinesCreateBlockList();
      PAGE_RES* page_res = Recognize(block_list, NULL);

      char* string;
      int* lengths;
      float* costs;
      int* x0;
      int* y0;
      int* x1;
      int* y1;
      // Retrieve Tesseract internal data to compute the quality score.
      int n = TessBaseAPI::TesseractExtractResult(&string, &lengths, &costs, &x0, &y0, &x1, &y1, page_res);

      float average_uncertainty = 0.f;
      for (int i = 0; i < n; ++i)
	average_uncertainty += costs[i];

      if (n)
	*score = average_uncertainty / n;
      else
	*score = -1;

      // Some memory cleanup
      delete[] string;
      delete[] lengths;
      delete[] costs;
      delete[] x0;
      delete[] y0;
      delete[] x1;
      delete[] y1;

      return TesseractToText(page_res);
    }
  }
};

// Call tesseract
// lang: expected language
template <typename T>
char*	tesseract(const char* lang, const mln::image2d<T>& input, float* score)
{
  assert(score);
  TessWrap::InitWithLanguage(NULL, NULL, lang, NULL, false, 0, NULL);
  char* s = TessWrap::TesseractRect(
			  (unsigned char*) input.buffer(),
			  sizeof (T),
			  input.ncols() * sizeof (T),
			  0, 0,
			  input.ncols(),
			  input.nrows(),
			  score);
  return s;
}


#endif // ! TESSERACT_WRAP_HH
