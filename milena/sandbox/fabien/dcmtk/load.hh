// Copyright (C) 2009 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/*
 *
 *  Copyright (C) 1994-2005, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module:  dcmdata
 *
 *  Author:  Gerd Ehlers
 *
 *  Purpose: List the contents of a dicom file
 *
 *  Last Update:      $Author: meichel $
 *  Update Date:      $Date: 2005/12/08 15:40:46 $
 *  CVS/RCS Revision: $Revision: 1.55 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#ifndef MLN_IO_DICOM_LOAD_HH
# define MLN_IO_DICOM_LOAD_HH

/*!
 * \file   mln/io/dicom/load.hh
 *
 * \brief Define a function which loads an image of kind dicom with
 * given path.
 *
 */

# include <mln/io/pnm/load.hh>

# include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
# include <dcmtk/ofstd/ofstream.h>
# include <dcmtk/dcmdata/dctk.h>
# include <dcmtk/dcmdata/dcdebug.h>
# include <dcmtk/dcmdata/cmdlnarg.h>
# include <dcmtk/ofstd/ofconapp.h>
# include <dcmtk/dcmdata/dcuid.h>       /* for dcmtk version name */
# include <dcmtk/dcmdata/dcistrmz.h>    /* for dcmZlibExpectRFC1950Encoding */

# define INCLUDE_CSTDLIB
# define INCLUDE_CSTRING
# include <dcmtk/ofstd/ofstdinc.h>

# ifdef WITH_ZLIB
#  include <zlib.h>        /* for zlibVersion() */
# endif

# define SHORTCOL 3
# define LONGCOL 20
# define PATH_SEPARATOR '/'



namespace mln
{

  namespace io
  {

    namespace dicom
    {

      /*! Load a dicom image in a Milena image.
       *
       * \param[out] ima A reference to the image which will receive
       * data.
       * \param[in] filename The source.
       */
      template <typename I>
      void load(Image<I>& ima,
		const std::string& filename);

      /*! Load a dicom image in a Milena image. To use this routine, you
       *  should specialize the template whith the value type of the
       *  image loaded. (ex : load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       */
      template <typename V>
      image2d<V> load(const std::string& filename);

      /*! Load a dicom image in a Milena image. To use this routine, you
       *  should specialize the template whith the value type of the
       *  image loaded. (ex : load<value::int_u8>("...") )
       *
       * \param[in] filename The image source.
       *
       * \return An image2d which contains loaded data.
       */
      template <typename V>
      image3d<V> load(const std::string& filename);

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      image2d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::dicom::load");
	image2d<V> ima;// = io::pnm::load<V>(DICOM, filename);
	trace::exiting("mln::io::dicom::load");
	return ima;
      }

      template <typename V>
      inline
      image3d<V> load(const std::string& filename)
      {
	trace::entering("mln::io::dicom::load");
	image2d<V> ima;// = io::pnm::load<V>(DICOM, filename);
	trace::exiting("mln::io::dicom::load");
	return ima;
      }






      static OFBool printAllInstances = OFTrue;
      static OFBool prependSequenceHierarchy = OFFalse;
      static int printTagCount = 0;
      static const int MAX_PRINT_TAG_NAMES = 1024;
      static const char* printTagNames[MAX_PRINT_TAG_NAMES];
      static const DcmTagKey* printTagKeys[MAX_PRINT_TAG_NAMES];
      static OFCmdUnsignedInt maxReadLength = 4096; // default is 4 KB

      static OFBool addPrintTagName(const char* tagName)
      {
	if (printTagCount >= MAX_PRINT_TAG_NAMES) {
	  std::cerr << "error: too many print Tag options (max: " << MAX_PRINT_TAG_NAMES << ")" << endl;
	  return OFFalse;
	}

	unsigned int group = 0xffff;
	unsigned int elem = 0xffff;
	if (sscanf(tagName, "%x,%x", &group, &elem) != 2)
	{
	  /* it is a name */
	  const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
	  const DcmDictEntry *dicent = globalDataDict.findEntry(tagName);
	  if (dicent == NULL)
	  {
	    std::cerr << "error: unrecognised tag name: '" << tagName << "'" << endl;
	    dcmDataDict.unlock();
	    return OFFalse;
	  }
	  else
	  {
	    /* note for later */
	    printTagKeys[printTagCount] = new DcmTagKey(dicent->getKey());
	  }
	  dcmDataDict.unlock();
	}
	else
	{
	  /* tag name has format xxxx,xxxx */
	  /* do not lookup in dictionary, tag could be private */
	  printTagKeys[printTagCount] = NULL;
	}

	printTagNames[printTagCount] = strcpy(OFstatic_cast(char*, malloc(strlen(tagName)+1)), tagName);
	printTagCount++;
	return OFTrue;
      }


      template <typename I>
      static void printResult(Image<I>& ima, DcmStack& stack, size_t printFlags)
      {
	unsigned long n = stack.card();
	if (n == 0)
	  return;

	if (prependSequenceHierarchy) {
	  /* print the path leading up to the top stack elem */
	  for (unsigned long i = n - 1; i >= 1; i--) {
	    DcmObject *dobj = stack.elem(i);
	    /* do not print if a DCM_Item as this is not
	     * very helpful to distinguish instances.
	     */
	    if (dobj != NULL && dobj->getTag().getXTag() != DCM_Item) {
	      char buf[512];
	      sprintf(buf, "(%x,%x).",
		  OFstatic_cast(unsigned, dobj->getGTag()),
		  OFstatic_cast(unsigned, dobj->getETag()));
	      std::cout << buf;
	    }
	  }
	}

	/* print the tag and its value */
	DcmObject *dobj = stack.top();
	dobj->print(std::cout, printFlags);
      }

      template <typename I>
      static int dumpFile(Image<I>& ima,
			  const char *ifname,
			  const E_FileReadMode readMode,
			  const E_TransferSyntax xfer,
			  const size_t printFlags,
			  const OFBool loadIntoMemory,
			  const OFBool stopOnErrors,
			  const OFBool writePixelData,
			  const char *pixelDirectory)
      {
	int result = 0;

	DcmFileFormat dfile;
	DcmObject *dset = &dfile;

	if (readMode == ERM_dataset)
	  dset = dfile.getDataset();

	// Load file

	OFCondition cond = dfile.loadFile(ifname, xfer, EGL_noChange, maxReadLength, readMode);

	// Read error

	if (!cond.good())
	{
	  std::cerr << "error: " << dfile.error().text()
	    << ": reading file: "<< ifname << endl;
	  result = 1;
	  if (stopOnErrors)
	    return result;
	}

	if (loadIntoMemory)
	  dfile.loadAllDataIntoMemory();

	if (printTagCount == 0)
	{
	  if (writePixelData)
	  {
	    OFString str = ifname;
	    OFString rname = pixelDirectory;
	    if ((rname.length() > 0) && (rname[rname.length() - 1] != PATH_SEPARATOR))
	      rname += PATH_SEPARATOR;
	    size_t pos = str.find_last_of(PATH_SEPARATOR);
	    if (pos == OFString_npos)
	      rname += str;
	    else
	      rname += str.substr(pos + 1);
	    size_t counter = 0;
	    dset->print(std::cout, printFlags, 0 /*level*/, rname.c_str(), &counter);
	  }
	  else
	    dset->print(std::cout, printFlags);
	}
	else
	{
	  /* only print specified tags */
	  for (int i = 0; i < printTagCount; i++)
	  {
	    unsigned int group = 0xffff;
	    unsigned int elem = 0xffff;
	    DcmTagKey searchKey;
	    const char* tagName = printTagNames[i];
	    if (printTagKeys[i])
	      searchKey = *printTagKeys[i];
	    else
	    {
	      if (sscanf(tagName, "%x,%x", &group, &elem) == 2)
		searchKey.set(group, elem);
	      else
	      {
		std::cerr << "Internal ERROR in File " << __FILE__ << ", Line "
		  << __LINE__ << std::endl
		  << "-- Named tag inconsistency" << std::endl;
		abort();
	      }
	    }

	    DcmStack stack;
	    if (dset->search(searchKey, stack, ESM_fromHere, OFTrue) == EC_Normal)
	    {
	      printResult(ima, stack, printFlags);
	      if (printAllInstances)
	      {
		while (dset->search(searchKey, stack, ESM_afterStackTop, OFTrue)  == EC_Normal)
		  printResult(ima, stack, printFlags);
	      }
	    }
	  }
	}

	return result;
      }


      template <typename I>
      inline
      void load(Image<I>& ima,
		const std::string& filename)
      {
	trace::entering("mln::io::dicom::load");

	std::ifstream file(filename.c_str());
	if (! file)
	{
	  std::cerr << "error: cannot open file '" << filename << "'!";
	  abort();
	}

	int opt_debugMode = 0;
	OFBool loadIntoMemory = OFTrue;
	size_t printFlags = DCMTypes::PF_shortenLongTagValues /*| DCMTypes::PF_showTreeStructure*/;
	OFBool printFilename = OFFalse;
	OFBool writePixelData = OFFalse;
	E_FileReadMode readMode = ERM_autoDetect;
	E_TransferSyntax xfer = EXS_Unknown;
	OFBool stopOnErrors = OFTrue;
	const char *current = NULL;
	const char *pixelDirectory = NULL;


	/* make sure data dictionary is loaded */
	if (!dcmDataDict.isDictionaryLoaded())
	{
	  std::cerr << "Warning: no data dictionary loaded, "
		    << "check environment variable: "
		    << DCM_DICT_ENVIRONMENT_VARIABLE;
	}

	int errorCount = 0;
	dumpFile(ima, current, readMode, xfer, printFlags, loadIntoMemory, stopOnErrors,
	         writePixelData, pixelDirectory);

	trace::exiting("mln::io::dicom::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::dicom

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_DICOM_LOAD_HH
