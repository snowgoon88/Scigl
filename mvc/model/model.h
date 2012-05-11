// -*- coding: utf-8 -*-
#ifndef __MODEL_H
#define __MODEL_H

#include <string>
#include <iostream>
#include <list>

class Observer;
#ifdef HAVE_BOOST
#include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Observer> ObserverPtr;
#else
    typedef class Observer * ObserverPtr;
#endif
#include "observer.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class Model> ModelPtr;
#else
    typedef class Model * ModelPtr;
#endif

/**
 * Abstract class for a model that can be observed.
 */
class Model
{
 public:
  /** Create: default*/
  Model( void );
  /** Create: copy*/
  Model( const Model &m );
  /** Destruction */
  ~Model();

  /** dump to STR */
  std::string dumpToString();
  /** display to STR */
  std::string toString();

 public:
  /** Attach observer */
  void attach_observer( ObserverPtr obs );
  /** Detach observer */
  void detach_observer( ObserverPtr obs );
  /** Notify observers */
  void notify_observers( int signal=0 );
 public:
  /** Observers */
  std::list< ObserverPtr > _observers;
};
#endif //__MODEL_H
