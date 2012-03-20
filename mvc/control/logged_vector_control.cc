// -*- coding: utf-8 -*-
#include "logged_vector_control.h"

/******************************************************************************************/
LoggedVectorControl::LoggedVectorControl( PlaneCoordPtr plane_ref )
{
  _plane_ref = plane_ref;
  // Store
  _stored_rg1 = _plane_ref->get_range_coord1();
  _stored_sl1 = _plane_ref->get_slide_max1();
  // Observers
  _v_obs.clear();
}
LoggedVectorControl::LoggedVectorControl( const LoggedVectorControl &lv )
{
  _plane_ref = lv._plane_ref;
  _stored_rg1 = lv._stored_rg1;
  _stored_sl1 = lv._stored_sl1;
  for( unsigned int i=0; i<lv._v_obs.size(); i++) {
    _v_obs.push_back( lv._v_obs[i] );
  }
}
LoggedVectorControl::~LoggedVectorControl()
{
}
/******************************************************************************************/
std::string
LoggedVectorControl::dumpToString()
{
  std::stringstream ss;
  
  ss << "LoggedVectorControl::dumpToString()\n";
  
  return ss.str();
}
std::string
LoggedVectorControl::toString()
{
  std::stringstream ss;
  
  ss << "LoggedVectorControl::toString()\n";
  
  return ss.str();
}
/******************************************************************************************/
LoggedVectorSciglPtr
LoggedVectorControl::add_viewer( LoggedVectorPtr model, 
				 unsigned int dim_to_observe, int abs_type)
{
  LoggedVectorSciglPtr obs = LoggedVectorSciglPtr( new LoggedVectorScigl( model ));
  obs->attach_to_coord( _plane_ref );
  model->attach_observer( obs );
  obs->_dim = dim_to_observe;
  obs->_abs = abs_type;

  _v_obs.push_back( obs );

  return obs;
}
/******************************************************************************************/
bool
LoggedVectorControl::is_window_mode()
{
  if( _v_obs.empty() == true )
    return true; // by default
  
  return _v_obs[0]->_fg_window_mode;
}
/******************************************************************************************/
void 
LoggedVectorControl::set_full_mode()
{
  if( _v_obs.empty() == true )
    return;
  LoggedVectorPtr model = _v_obs[0]->_model;
  // Check that it is not already set
  if( _v_obs[0]->_fg_window_mode == false )
    return;

  // Store parameters
  _stored_rg1 = _plane_ref->get_range_coord1();
  _stored_sl1 = _plane_ref->get_slide_max1();
  // Compute full range
  if( _v_obs[0]->_abs == -2 ) {
    //std::cout << "T: _abs == -2\n";
    _plane_ref->set_range_coord1( Range( 0, model->_data.size(), 
					 _stored_rg1.major,
					 _stored_rg1.minor ) );
  }
  else if( _v_obs[0]->_abs == -1 ) {
    //std::cout << "T: _abs == -1\n";
    _plane_ref->set_range_coord1( Range( model->_data[0].t,
					 model->_data[model->_data.size()-1].t, 
					 _stored_rg1.major,
					 _stored_rg1.minor ) );
  }
  else {
    //std::cout << "T: _abs >= 0 (" << _vec_obs1->_abs << "\n";
    _plane_ref->set_range_coord1( Range( model->_data[0].v(_v_obs[0]->_abs),
					 model->_data[model->_data.size()-1].v(_v_obs[0]->_abs), 
					 _stored_rg1.major,
					 _stored_rg1.minor ) );
  }
  // Notify
  //std::cout << "T : notify scigl element full mode\n";
  for( unsigned int i=0; i<_v_obs.size(); i++) {
    _v_obs[i]->set_full_mode();
  }
  _plane_ref->set_sliding_coord1( false );
}
void
LoggedVectorControl::set_window_mode()
{
  if( _v_obs.empty() == true )
    return;
  // Check that it is not already set
  if( _v_obs[0]->_fg_window_mode == true )
    return;

  // Restore
  _plane_ref->set_range_coord1( _stored_rg1 );
  _plane_ref->set_slide_max1( _stored_sl1 );
  // Notify
  //std::cout << "T : notify scigl element window mode\n";
  for( unsigned int i=0; i<_v_obs.size(); i++) {
    _v_obs[i]->set_window_mode();
  }
  _plane_ref->set_sliding_coord1( true );
}
/******************************************************************************************/
