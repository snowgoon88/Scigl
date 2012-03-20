// -*- coding: utf-8 -*-
#ifndef __LOGGED_VECTOR_TXT_H
#define __LOGGED_VECTOR_TXT_H

#include <string>
#include <iostream>

#include "observer.h" 

class LoggedVector;
#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVector> LoggedVectorPtr;
#else
    typedef class LoggedVector * LoggedVectorPtr;
#endif
#include "logged_vector.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVectorTxt> LoggedVectorTxtPtr;
#else
    typedef class LoggedVectorTxt * LoggedVectorTxtPtr;
#endif

/**
 * Observes and display a LoggedVector
 */
class LoggedVectorTxt : public Observer
{
 public:
  /** Create: default */
  LoggedVectorTxt();
  /** Create: with model */
  LoggedVectorTxt( LoggedVectorPtr model );
  /** Create: copy*/
  LoggedVectorTxt( const LoggedVectorTxt &lv );
  /** Destruction */
  ~LoggedVectorTxt();

  /** dump to STR */
  std::string dumpToString();
  /** display to STR */
  std::string toString();

 public:
  LoggedVectorPtr _model;

 public:
  virtual void update( int signal );
};
#endif //__LOGGED_VECTOR_TXT_H
