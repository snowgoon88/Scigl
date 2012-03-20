// -*- coding: utf-8 -*-
#include "observer.h"
#include <sstream>

/******************************************************************************************/
Observer::Observer()
{
}
Observer::Observer( const Observer &obs )
{
}
Observer::~Observer()
{
}
/******************************************************************************************/
std::string
Observer::dumpToString()
{
  std::stringstream ss;

  ss << "Observer::dumpToString()\n";
  
  return ss.str();
}
std::string
Observer::toString()
{
  std::stringstream ss;

  ss << "Observer::toString()\n";
  
  return ss.str();
}
/******************************************************************************************/
void
Observer::update( int signal )
{
  std::cout << "Received signal=" << signal << "\n";
}
/******************************************************************************************/
