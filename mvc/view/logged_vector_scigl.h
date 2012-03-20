// -*- coding: utf-8 -*-
#ifndef __LOGGED_VECTOR_SCIGL_H
#define __LOGGED_VECTOR_SCIGL_H

#include <string>
#include <iostream>

#include "observer.h" 

#include "object.h"
#include "scene.h"
#include "plane-coord.h"

class LoggedVector;
#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVector> LoggedVectorPtr;
#else
    typedef class LoggedVector * LoggedVectorPtr;
#endif
#include "logged_vector.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVectorScigl> LoggedVectorSciglPtr;
#else
    typedef class LoggedVectorScigl * LoggedVectorSciglPtr;
#endif

/**
 * Observes and display a LoggedVector
 */
class LoggedVectorScigl : public Observer, public Object
{
 public:
  /** Create: default */
  LoggedVectorScigl();
  /** Create: with model */
  LoggedVectorScigl( LoggedVectorPtr model );
  /** Create: copy*/
  LoggedVectorScigl( const LoggedVectorScigl &lv );
  /** Destruction */
  ~LoggedVectorScigl();

  /** dump to STR */
  std::string dumpToString();
  /** display to STR */
  std::string toString();

 public:
  LoggedVectorPtr _model;
  /** Which dimension to plot */
  unsigned int _dim;
  /** Which abcisse to take : -2 = indice, -1 = t of LoggedVector, 0+ v[_abs] of LoggedVector */
  int _abs;
  /** Either window mode or full range mode */
  bool _fg_window_mode;
  /** Store range for window mode */
  Range _stored_rg1;
  /** Store max if sliding window */
  float _stored_sl1 ;


 public:
  virtual void update( int signal );

 public:
  /** Reference frame*/
  PlaneCoordPtr _plane_coord;
  /** Cache size for iteration/time coordinate*/
  int _cache_size;
  /** Line thickness */
  float _thickness;

 public:
  /** Attach to a PlaneCoordPtr */
  void attach_to_coord( PlaneCoordPtr plane );
  /** Set the thickness of lines */
  virtual void set_thickness (float thickness);
  /** Get the thickness of lines */
  virtual float get_thickness (void) const;
  /** Swith between full_range or window (the default) mode */
  void set_window_mode();
  void set_full_mode();
 public:
  /** From Object */
  void render();
  
};
#endif //__LOGGED_VECTOR_SCIGL_H
