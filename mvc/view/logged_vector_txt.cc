// -*- coding: utf-8 -*-
#include "logged_vector_txt.h"

/******************************************************************************************/
LoggedVectorTxt::LoggedVectorTxt() : Observer()
{
}
LoggedVectorTxt::LoggedVectorTxt( LoggedVectorPtr model ) : Observer()
{
  _model = model;
}
LoggedVectorTxt::LoggedVectorTxt( const LoggedVectorTxt &lv ) : Observer()
{
  _model = lv._model;
}
LoggedVectorTxt::~LoggedVectorTxt()
{
}
/******************************************************************************************/
std::string
LoggedVectorTxt::dumpToString()
{
  std::stringstream ss;
  
  ss << Observer::dumpToString();
  ss << "LoggedVectorTxt::dumpToString()\n";
  
  return ss.str();
}
std::string
LoggedVectorTxt::toString()
{
  std::stringstream ss;

  ss << Observer::toString();
  ss << "LoggedVectorTxt::toString()\n";
  
  return ss.str();
}
/******************************************************************************************/
void
LoggedVectorTxt::update( int signal )
{
  Observer::update( signal );
  
  for( int i=0; i<_model->_data.size(); i++) {
    if( signal < _model->_data[i].v.size() ) {
      std::cout << "--" << _model->_data[i].v(signal) << "\n";
    }
  }
}
/******************************************************************************************/
