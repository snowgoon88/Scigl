// -*- coding: utf-8 -*-
#ifndef __LOGGED_VECTOR_CONTROL_H
#define __LOGGED_VECTOR_CONTROL_H

#include <string>
#include <iostream>

#include "logged_vector.h"
#include "logged_vector_scigl.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVectorControl> LoggedVectorControlPtr;
#else
    typedef class LoggedVectorControl * LoggedVectorControlPtr;
#endif

/**
 * Control some SCIGL Views (models can be different but this is tricky)
 */
class LoggedVectorControl
{
 public:
  /** Create: default */
  LoggedVectorControl( PlaneCoordPtr plane_ref );
  /** Create: copy*/
  LoggedVectorControl( const LoggedVectorControl &lv );
  /** Destruction */
  ~LoggedVectorControl();

  /** dump to STR */
  std::string dumpToString();
  /** display to STR */
  std::string toString();

  /** Attach observer to a model and dimension */
  LoggedVectorSciglPtr add_viewer( LoggedVectorPtr model, 
				   unsigned int dim_to_observe = 0, int abs_type = -2);
  /** Check status (true by default) */
  bool is_window_mode();
  /** Switch between full and window mode */
  void set_full_mode();
  void set_window_mode();

 public:
  /** vector of LoggedVectorSciglPtr */
  std::vector< LoggedVectorSciglPtr > _v_obs;
  /** Reference frame */
  PlaneCoordPtr _plane_ref;
  /** Store range for window mode */
  Range _stored_rg1;
  /** Store max if sliding window */
  float _stored_sl1 ;

};
#endif //__LOGGED_VECTOR_CONTROL_H
