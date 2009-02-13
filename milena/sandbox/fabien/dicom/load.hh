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

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/ofstd/ofstream.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcdebug.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/dcmdata/dcuid.h"       /* for dcmtk version name */
#include "dcmtk/dcmdata/dcistrmz.h"    /* for dcmZlibExpectRFC1950Encoding */

#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#include "dcmtk/ofstd/ofstdinc.h"

#ifdef WITH_ZLIB
#include <zlib.h>        /* for zlibVersion() */
#endif

#define OFFIS_CONSOLE_APPLICATION "dcmdump"

static char rcsid[] = "$dcmtk: " OFFIS_CONSOLE_APPLICATION " v"
  OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

#ifdef HAVE_GUSI_H
    /* needed for Macintosh */
#include <GUSI.h>
#include <SIOUX.h>
#endif

static int dumpFile(ostream & out,
            const char *ifname,
            const E_FileReadMode readMode,
            const E_TransferSyntax xfer,
            const size_t printFlags,
            const OFBool loadIntoMemory,
            const OFBool stopOnErrors,
            const OFBool writePixelData,
            const char *pixelDirectory);

// ********************************************

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
        CERR << "error: too many print Tag options (max: " << MAX_PRINT_TAG_NAMES << ")" << endl;
        return OFFalse;
    }

    unsigned int group = 0xffff;
    unsigned int elem = 0xffff;
    if (sscanf( tagName, "%x,%x", &group, &elem ) != 2 )
    {
    /* it is a name */
        const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
        const DcmDictEntry *dicent = globalDataDict.findEntry(tagName);
        if( dicent == NULL ) {
            CERR << "error: unrecognised tag name: '" << tagName << "'" << endl;
            dcmDataDict.unlock();
            return OFFalse;
        } else {
            /* note for later */
            printTagKeys[printTagCount] = new DcmTagKey(dicent->getKey());
        }
        dcmDataDict.unlock();
    } else {
        /* tag name has format xxxx,xxxx */
        /* do not lookup in dictionary, tag could be private */
        printTagKeys[printTagCount] = NULL;
    }

    printTagNames[printTagCount] = strcpy(OFstatic_cast(char*, malloc(strlen(tagName)+1)), tagName);
    printTagCount++;
    return OFTrue;
}

#define SHORTCOL 3
#define LONGCOL 20

int main(int argc, char *argv[])
{
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


#ifdef HAVE_GUSI_H
    /* needed for Macintosh */
    /* set options for the Metrowerks CodeWarrior SIOUX console */
    SIOUXSettings.autocloseonquit = OFFalse;
    SIOUXSettings.asktosaveonclose = OFFalse;
    SIOUXSettings.showstatusline = OFTrue;
    SIOUXSettings.setupmenus = OFTrue;
    /* set options for the GUSI sockets library */
    GUSISetup(GUSIwithSIOUXSockets);
    GUSISetup(GUSIwithInternetSockets);
#endif

    SetDebugLevel(( 0 ));

    OFConsoleApplication app(OFFIS_CONSOLE_APPLICATION, "Dump DICOM file and data set", rcsid);
    OFCommandLine cmd;
    cmd.setOptionColumns(LONGCOL, SHORTCOL);
    cmd.setParamColumn(LONGCOL + SHORTCOL + 4);

    cmd.addParam("dcmfile-in", "DICOM input filename to be dumped", OFCmdParam::PM_MultiMandatory);

    cmd.addGroup("general options:", LONGCOL, SHORTCOL + 2);
      cmd.addOption("--help",                 "-h",     "print this help text and exit");
      cmd.addOption("--version",                        "print version information and exit", OFTrue /* exclusive */);
#ifdef USE_EXPERIMENTAL_QUIET_MODE
      cmd.addOption("--quiet",                "-q",     "quiet mode, print no warnings and errors");
#endif
      cmd.addOption("--debug",                "-d",     "debug mode, print debug information");

    cmd.addGroup("input options:");
      cmd.addSubGroup("input file format:");
        cmd.addOption("--read-file",          "+f",     "read file format or data set (default)");
        cmd.addOption("--read-file-only",     "+fo",    "read file format only");
        cmd.addOption("--read-dataset",       "-f",     "read data set without file meta information");
      cmd.addSubGroup("input transfer syntax:");
        cmd.addOption("--read-xfer-auto",     "-t=",    "use TS recognition (default)");
        cmd.addOption("--read-xfer-detect",   "-td",    "ignore TS specified in the file meta header");
        cmd.addOption("--read-xfer-little",   "-te",    "read with explicit VR little endian TS");
        cmd.addOption("--read-xfer-big",      "-tb",    "read with explicit VR big endian TS");
        cmd.addOption("--read-xfer-implicit", "-ti",    "read with implicit VR little endian TS");
      cmd.addSubGroup("parsing of odd-length attributes:");
        cmd.addOption("--accept-odd-length",  "+ao",    "accept odd length attributes (default)");
        cmd.addOption("--assume-even-length", "+ae",    "assume real length is one byte larger");
      cmd.addSubGroup("handling of undefined length UN elements:");
       cmd.addOption("--enable-cp246",        "+ui",    "read undefined len UN as implicit VR (default)");
       cmd.addOption("--disable-cp246",       "-ui",    "read undefined len UN as explicit VR");
      cmd.addSubGroup("handling of defined length UN elements:");
       cmd.addOption("--retain-un",           "-uc",    "retain elements as UN (default)");
       cmd.addOption("--convert-un",          "+uc",    "convert to real VR if known");
      cmd.addSubGroup("automatic data correction:");
        cmd.addOption("--enable-correction",  "+dc",    "enable automatic data correction (default)");
        cmd.addOption("--disable-correction", "-dc",    "disable automatic data correction");
#ifdef WITH_ZLIB
    cmd.addSubGroup("bitstream format of deflated input:");
     cmd.addOption("--bitstream-deflated",    "+bd",    "expect deflated bitstream (default)");
     cmd.addOption("--bitstream-zlib",        "+bz",    "expect deflated zlib bitstream");
#endif

    cmd.addGroup("output options:");
      cmd.addSubGroup("printing:");
        cmd.addOption("--load-all",           "+M",     "load very long tag values (default)");
        cmd.addOption("--load-short",         "-M",     "do not load very long values (e.g. pixel data)");
        cmd.addOption("--max-read-length",    "+R",  1, "[k]bytes: integer [4..4194302] (default: 4)",
                                                        "set threshold for long values to k kbytes");
        cmd.addOption("--print-all",          "+L",     "print long tag values completely");
        cmd.addOption("--print-short",        "-L",     "print long tag values shortened (default)");
        cmd.addOption("--print-filename",     "+F",     "print header with filename for each input file");

      cmd.addSubGroup("error handling:");
        cmd.addOption("--stop-on-error",      "-E",     "do not print if file is damaged (default)");
        cmd.addOption("--ignore-errors",      "+E",     "attempt to print even if file is damaged");

      cmd.addSubGroup("searching:");
        cmd.addOption("--search",             "+P",  1, "[t]ag: \"xxxx,xxxx\" or a data dictionary name",
                                                        "print the value of tag t\nthis option can be specified multiple times\n(default: the complete file is printed)");

        cmd.addOption("--search-all",         "+s",     "print all instances of searched tags (default)");
        cmd.addOption("--search-first",       "-s",     "only print first instance of searched tags");

        cmd.addOption("--prepend",            "+p",     "prepend sequence hierarchy to printed tag,\ndenoted by: (xxxx,xxxx).(xxxx,xxxx).*\n(only with --search-all or --search-first)");
        cmd.addOption("--no-prepend",         "-p",     "do not prepend hierarchy to tag (default)");

      cmd.addSubGroup("writing:");
        cmd.addOption("--write-pixel",        "+W",  1, "[d]irectory : string",
                                                        "write pixel data to a .raw file stored in d\n(little endian, filename created automatically)");

    /* evaluate command line */
    prepareCmdLineArgs(argc, argv, OFFIS_CONSOLE_APPLICATION);
    if (app.parseCommandLine(cmd, argc, argv, OFCommandLine::ExpandWildcards))
    {
      /* check exclusive options first */

      if (cmd.getParamCount() == 0)
      {
        if (cmd.findOption("--version"))
        {
            app.printHeader(OFTrue /*print host identifier*/);          // uses ofConsole.lockCerr()
            CERR << endl << "External libraries used:";
#ifdef WITH_ZLIB
            CERR << endl << "- ZLIB, Version " << zlibVersion() << endl;
#else
            CERR << " none" << endl;
#endif
            return 0;
         }
      }

      /* options */

#ifdef USE_EXPERIMENTAL_QUIET_MODE
      if (cmd.findOption("--quiet"))
      {
        // tbd: disable ofConsole output!
        app.setQuietMode();
      }
#endif
      if (cmd.findOption("--debug")) opt_debugMode = 5;

      cmd.beginOptionBlock();
      if (cmd.findOption("--read-file")) readMode = ERM_autoDetect;
      if (cmd.findOption("--read-file-only")) readMode = ERM_fileOnly;
      if (cmd.findOption("--read-dataset")) readMode = ERM_dataset;
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--read-xfer-auto"))
        xfer = EXS_Unknown;
      if (cmd.findOption("--read-xfer-detect"))
        dcmAutoDetectDatasetXfer.set(OFTrue);
      if (cmd.findOption("--read-xfer-little"))
      {
        app.checkDependence("--read-xfer-little", "--read-dataset", readMode == ERM_dataset);
        xfer = EXS_LittleEndianExplicit;
      }
      if (cmd.findOption("--read-xfer-big"))
      {
        app.checkDependence("--read-xfer-big", "--read-dataset", readMode == ERM_dataset);
        xfer = EXS_BigEndianExplicit;
      }
      if (cmd.findOption("--read-xfer-implicit"))
      {
        app.checkDependence("--read-xfer-implicit", "--read-dataset", readMode == ERM_dataset);
        xfer = EXS_LittleEndianImplicit;
      }
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--accept-odd-length"))
      {
        dcmAcceptOddAttributeLength.set(OFTrue);
      }
      if (cmd.findOption("--assume-even-length"))
      {
        dcmAcceptOddAttributeLength.set(OFFalse);
      }
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--enable-cp246"))
      {
        dcmEnableCP246Support.set(OFTrue);
      }
      if (cmd.findOption("--disable-cp246"))
      {
        dcmEnableCP246Support.set(OFFalse);
      }
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--retain-un"))
      {
        dcmEnableUnknownVRConversion.set(OFFalse);
      }
      if (cmd.findOption("--convert-un"))
      {
        dcmEnableUnknownVRConversion.set(OFTrue);
      }
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--enable-correction"))
      {
        dcmEnableAutomaticInputDataCorrection.set(OFTrue);
      }
      if (cmd.findOption("--disable-correction"))
      {
        dcmEnableAutomaticInputDataCorrection.set(OFFalse);
      }
      cmd.endOptionBlock();

#ifdef WITH_ZLIB
      cmd.beginOptionBlock();
      if (cmd.findOption("--bitstream-deflated"))
      {
        dcmZlibExpectRFC1950Encoding.set(OFFalse);
      }
      if (cmd.findOption("--bitstream-zlib"))
      {
        dcmZlibExpectRFC1950Encoding.set(OFTrue);
      }
      cmd.endOptionBlock();
#endif

      if (cmd.findOption("--max-read-length"))
      {
          app.checkValue(cmd.getValueAndCheckMinMax(maxReadLength, 4, 4194302));
          maxReadLength *= 1024; // convert kbytes to bytes
      }
      cmd.beginOptionBlock();
      if (cmd.findOption("--load-all")) loadIntoMemory = OFTrue;
      if (cmd.findOption("--load-short")) loadIntoMemory = OFFalse;
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--print-all")) printFlags &= ~DCMTypes::PF_shortenLongTagValues;
      if (cmd.findOption("--print-short")) printFlags |= DCMTypes::PF_shortenLongTagValues;
      cmd.endOptionBlock();

      if (cmd.findOption("--print-filename"))
        printFilename = OFTrue;

      cmd.beginOptionBlock();
      if (cmd.findOption("--stop-on-error")) stopOnErrors = OFTrue;
      if (cmd.findOption("--ignore-errors")) stopOnErrors = OFFalse;
      cmd.endOptionBlock();

      if (cmd.findOption("--search", 0, OFCommandLine::FOM_First))
      {
        do
        {
          app.checkValue(cmd.getValue(current));
          if (!addPrintTagName(current)) return 1;
        } while (cmd.findOption("--search", 0, OFCommandLine::FOM_Next));
      }

      cmd.beginOptionBlock();
      if (cmd.findOption("--search-all"))
      {
        app.checkDependence("--search-all", "--search", printTagCount>0);
        printAllInstances = OFTrue;
      }
      if (cmd.findOption("--search-first"))
      {
        app.checkDependence("--search-first", "--search", printTagCount>0);
        printAllInstances = OFFalse;
      }
      cmd.endOptionBlock();

      cmd.beginOptionBlock();
      if (cmd.findOption("--prepend"))
      {
        app.checkDependence("--prepend", "--search", printTagCount>0);
        prependSequenceHierarchy = OFTrue;
      }
      if (cmd.findOption("--no-prepend"))
      {
        app.checkDependence("--no-prepend", "--search", printTagCount>0);
        prependSequenceHierarchy = OFFalse;
      }
      cmd.endOptionBlock();

      if (cmd.findOption("--write-pixel"))
      {
        app.checkValue(cmd.getValue(pixelDirectory));
        writePixelData = OFTrue;
      }
    }

    SetDebugLevel((opt_debugMode));

    /* make sure data dictionary is loaded */
    if (!dcmDataDict.isDictionaryLoaded())
    {
        CERR << "Warning: no data dictionary loaded, "
             << "check environment variable: "
             << DCM_DICT_ENVIRONMENT_VARIABLE;
    }

    int errorCount = 0;
    int count = cmd.getParamCount();
    for (int i=1; i<=count; i++)
    {
      cmd.getParam(i, current);
      if (printFilename)
      {
        /* a newline separates two consecutive "dumps" */
        if (i > 1)
            COUT << endl;
        /* print header with filename */
        COUT << "# " << OFFIS_CONSOLE_APPLICATION << " (" << i << "/" << count << "): " << current << endl;
      }
      errorCount += dumpFile(COUT, current, readMode, xfer, printFlags, loadIntoMemory, stopOnErrors,
        writePixelData, pixelDirectory);
    }

    return errorCount;
}

static void printResult(ostream& out, DcmStack& stack, size_t printFlags)
{
    unsigned long n = stack.card();
    if (n == 0) {
        return;
    }

    if (prependSequenceHierarchy) {
        /* print the path leading up to the top stack elem */
        for (unsigned long i=n-1; i>=1; i--) {
            DcmObject *dobj = stack.elem(i);
            /* do not print if a DCM_Item as this is not
             * very helpful to distinguish instances.
             */
            if (dobj != NULL && dobj->getTag().getXTag() != DCM_Item) {
                char buf[512];
                sprintf(buf, "(%x,%x).",
                    OFstatic_cast(unsigned, dobj->getGTag()),
                    OFstatic_cast(unsigned, dobj->getETag()));
                out << buf;
            }
        }
    }

    /* print the tag and its value */
    DcmObject *dobj = stack.top();
    dobj->print(out, printFlags);
}

static int dumpFile(ostream & out,
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

    if ((ifname == NULL) || (strlen(ifname) == 0))
    {
        CERR << OFFIS_CONSOLE_APPLICATION << ": invalid filename: <empty string>" << endl;
        return 1;
    }

    DcmFileFormat dfile;
    DcmObject *dset = &dfile;

    if (readMode == ERM_dataset) dset = dfile.getDataset();

    // Load file

    OFCondition cond = dfile.loadFile(ifname, xfer, EGL_noChange, maxReadLength, readMode);

    // Read error

    if (! cond.good())
    {
        CERR << OFFIS_CONSOLE_APPLICATION << ": error: " << dfile.error().text()
             << ": reading file: "<< ifname << endl;

        result = 1;
        if (stopOnErrors) return result;
    }

    if (loadIntoMemory) dfile.loadAllDataIntoMemory();

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
            dset->print(out, printFlags, 0 /*level*/, rname.c_str(), &counter);
        } else
            dset->print(out, printFlags);
    } else {
        /* only print specified tags */
        for (int i=0; i<printTagCount; i++)
        {
            unsigned int group = 0xffff;
            unsigned int elem = 0xffff;
            DcmTagKey searchKey;
            const char* tagName = printTagNames[i];
            if (printTagKeys[i]) searchKey = *printTagKeys[i];
            else if (sscanf( tagName, "%x,%x", &group, &elem ) == 2 ) searchKey.set(group, elem);
            else {
                CERR << "Internal ERROR in File " << __FILE__ << ", Line "
                     << __LINE__ << endl
                    << "-- Named tag inconsistency" << endl;
                abort();
            }

            DcmStack stack;
            if (dset->search(searchKey, stack, ESM_fromHere, OFTrue) == EC_Normal)
            {
                printResult(out, stack, printFlags);
                if (printAllInstances)
                {
                    while (dset->search(searchKey, stack, ESM_afterStackTop, OFTrue)  == EC_Normal)
                      printResult(out, stack, printFlags);
                }
            }
        }
    }

    return result;
}
