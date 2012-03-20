// -*- coding: utf-8 -*-
#ifndef __OBSERVER_H
#define __OBSERVER_H

#include <string>
#include <iostream>

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class Observer> ObserverPtr;
#else
    typedef class Observer * ObserverPtr;
#endif

/**
 * Observes and display a Model.
 */
class Observer
{
 public:
  /** Create: default*/
  Observer();
  /** Create: copy*/
  Observer( const Observer &obs );
  /** Destruction */
  ~Observer();

  /** dump to STR */
  std::string dumpToString();
  /** display to STR */
  std::string toString();

 public:
  /** update called from Model */
  virtual void update( int signal );
};
#endif //__OBSERVER_H
