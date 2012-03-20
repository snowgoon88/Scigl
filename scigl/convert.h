// File: convert.h
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

class BadConversion : public std::runtime_error
{
 public:
  BadConversion(const std::string& s)
    : std::runtime_error(s)
    { }
};

inline double to_double(const std::string& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    throw BadConversion("convertToDouble(\"" + s + "\")");
  return x;
}
inline float to_float(const std::string& s)
{
  std::istringstream i(s);
  float x;
  if (!(i >> x))
    throw BadConversion("convertToFloat(\"" + s + "\")");
  return x;
}
inline int to_int(const std::string& s)
{
  std::istringstream i(s);
  int x;
  if (!(i >> x))
    throw BadConversion("convertToInt(\"" + s + "\")");
  return x;
}
inline int to_unsigned_int(const std::string& s)
{
  std::istringstream i(s);
  unsigned int x;
  if (!(i >> x))
    throw BadConversion("convertToUnsignedInt(\"" + s + "\")");
  return x;
}
/**
 * Tokenize a string 'str' into a vector of strings according to delimiter.
 * @param str string to be tokenized
 * @param tokens vector of string token
 * @param delimiters between each token
 */
void tokenize(const std::string& str,
                      std::vector<std::string>& tokens,
                      const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
