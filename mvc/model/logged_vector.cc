// -*- coding: utf-8 -*-
#include "logged_vector.h"

/******************************************************************************************/
LoggedVector::LoggedVector( void ) : Model()
{
}
LoggedVector::LoggedVector( const LoggedVector &lv ) : Model(lv)
{
  clear();
  for( unsigned int i=0; i<_data.size(); i++) {
    _data.push_back( lv._data[i] );
  }
}
LoggedVector::~LoggedVector()
{
}
 /******************************************************************************************/
std::string
LoggedVector::dumpToString()
{
  std::stringstream ss;

  ss << Model::dumpToString();
  ss << toString() << "\n";
  
  return ss.str();
}
std::string
LoggedVector::toString()
{
  std::stringstream ss;

  for( unsigned int i=0; i<_data.size(); i++) {
    ss << "[" << i << "] ";
    ss << "t=" << _data[i].t << " (";
    for( unsigned int j=0; j<_data[i].v.size(); j++) {
      ss << _data[i].v[j] << ", ";
    }
    ss << ")\n";
  }

  return ss.str();
}
/******************************************************************************************/
void
LoggedVector::clear()
{
  _data.clear();
  notify_observers();
}
void
LoggedVector::add_vector( T_Time t, T_Vect v)
{
  T_Logged new_data = {t,v};
  _data.push_back( new_data );
  notify_observers();
}
/******************************************************************************************/


