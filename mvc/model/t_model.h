// -*- coding: utf-8 -*-
#ifndef __MODEL_H
#define __MODEL_H

#include <string>
#include <iostream>
#include <list>

class LoggedVectorTxt;
#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVectorTxt> LoggedVectorTxtPtr;
#else
    typedef class LoggedVectorTxt * LoggedVectorTxtPtr;
#endif
#include "logged_vector_txt.h"

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
  typedef LoggedVectorTxtPtr ObservablePtr;
  /** Attach observable*/
  void attach_observable( ObservablePtr obs );
  /** Detach observable */
  void detach_observable( ObservablePtr obs );
  /** Notify observables */
  void notify_observables( int signal=0 );
 public:
  /** Observables */
  std::list< ObservablePtr > _observables;
};
#endif //__MODEL_H
