// -*- coding: utf-8 -*-
#include "model.h"
#include <sstream>

/******************************************************************************************/
Model::Model( void )
{
}
Model::Model( const Model &m )
{
  _observers.clear();
  std::list<ObserverPtr>::const_iterator _iter_obs;
  for( _iter_obs = m._observers.begin(); _iter_obs != m._observers.end(); _iter_obs++) {
    attach_observer( (*_iter_obs) );
  }
}
Model::~Model()
{
}
 /******************************************************************************************/
std::string
Model::dumpToString()
{
  std::stringstream ss;

  ss << "Attached to " << _observers.size() << " observers\n";
  ss << toString() << "\n";
  
  return ss.str();
}
std::string
Model::toString()
{
  std::stringstream ss;

  return ss.str();
}
/******************************************************************************************/
void
Model::attach_observer( ObserverPtr obs )
{
  _observers.push_back( obs );
}
void
Model::detach_observer( ObserverPtr obs )
{
  _observers.remove( obs );
}
/**
 * @todo only if modified.
 */
void
Model::notify_observers( int signal )
{
  std::list<ObserverPtr>::const_iterator _iter_obs;
  for( _iter_obs = _observers.begin(); _iter_obs != _observers.end(); _iter_obs++) {
    (*_iter_obs)->update( signal );
  }
}
/******************************************************************************************/


