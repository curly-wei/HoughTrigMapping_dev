//-----------------------------------------------------------------------------
// $Id$
//-----------------------------------------------------------------------------
// Filename : Utilities.h
// Section  : TRG
// Owner    : Yoshihito Iwasaki
// Email    : yoshihito.iwasaki@kek.jp
//-----------------------------------------------------------------------------
// Description : Utility functions
//-----------------------------------------------------------------------------
// $Log$
//-----------------------------------------------------------------------------

#ifndef TRGUtilities_FLAG_
#define TRGUtilities_FLAG_

#include <string>

#if defined(_WIN64) && defined(Utilities_EXPORTS)
#  if defined(LIB_INTERNAL)
#    define API_ATTRI __declspec (dllexport)
#  else
#    define API_ATTRI __declspec (dllimport)
#  endif
#endif

#if !defined(API_ATTRI)
#  define API_ATTRI
#endif

#ifdef TRG_SHORT_NAMES
#define TRGUtil TRGUtilities
#endif

namespace Belle2 {

/// A class to provide TRG utility functions
  class TRGUtilities {

  public: // Utility functions

    /// converts int to string. (Use boost::lexical_cast)
    API_ATTRI static std::string itostring(int i);

    /// converts double to string.
    API_ATTRI static std::string dtostring(double d, unsigned int precision);

    /// CERNLIB car.
    API_ATTRI static std::string carstring(const std::string& s);

    /// CERNLIB cdr.
    API_ATTRI static std::string cdrstring(const std::string& s);

    /// returns date string.
    API_ATTRI static std::string dateString(void);

    /// returns date string for filename.
    API_ATTRI static std::string dateStringF(void);

    /// Dumps bit contents to cout.
    API_ATTRI static void bitDisplay(unsigned);

    /// Dumps bit contents to cout.
    API_ATTRI static void bitDisplay(unsigned val,
                           unsigned firstDigit,
                           unsigned lastDigit);

    /// Dumps bit stream in string.
    API_ATTRI static std::string streamDisplay(unsigned);

    /// Dumps bit stream in string.
    API_ATTRI static std::string streamDisplay(unsigned,
                                     unsigned firstDigit,
                                     unsigned lastDigit);

  };

//-----------------------------------------------------------------------------


} // namespace Belle2


#endif /* TRGUtilities_FLAG_ */
