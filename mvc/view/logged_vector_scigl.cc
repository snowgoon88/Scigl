// -*- coding: utf-8 -*-
#include "logged_vector_scigl.h"
#include <algorithm>

/******************************************************************************************/
LoggedVectorScigl::LoggedVectorScigl() : Observer(), Object()
{
  _cache_size = 200;
  _dim = 0;
  _abs = -2; // default for index of LoggedVector
  _fg_window_mode = true;

  // SCIGL
  set_size (1.0f, 1.0f, 1.0f);
  set_position (0.0f, 0.0f, 0.0f);
  set_fg_color (0.0f, 0.0f, 0.0f, 1.0f);
  set_bg_color (1.0f, 1.0f, 1.0f, 1.0f);
  set_thickness (1.01);
  
  std::ostringstream oss;
  oss << "Curve_" << id_;
  set_name (oss.str());
}
LoggedVectorScigl::LoggedVectorScigl( LoggedVectorPtr model ) : Observer(), Object()
{
  _model = model;
  _cache_size = 200;
  _dim = 0;
  _abs = -2; // default for index of LoggedVector
  _fg_window_mode = true;

  // SCIGL
  set_size (1.0f, 1.0f, 1.0f);
  set_position (0.0f, 0.0f, 0.0f);
  set_fg_color (0.0f, 0.0f, 0.0f, 1.0f);
  set_bg_color (1.0f, 1.0f, 1.0f, 1.0f);
  set_thickness (1.01);

  std::ostringstream oss;
  oss << "Curve_" << id_;
  set_name (oss.str());
}
LoggedVectorScigl::LoggedVectorScigl( const LoggedVectorScigl &lv ) : Observer(), Object()
{
  _model = lv._model;
  _cache_size = lv._cache_size;
  _dim = lv._dim;
  _abs = lv._abs; // default for index of LoggedVector
  _fg_window_mode = lv._fg_window_mode;
  _stored_rg1 = lv._stored_rg1;
  _stored_sl1 = lv._stored_sl1;

  //SCIGL
  // @todo : copy from other
  set_size ( lv.get_size() );
  set_position ( lv.get_position());
  set_fg_color ( lv.get_fg_color());
  set_bg_color ( lv.get_bg_color());
  set_thickness ( lv.get_thickness());

  std::ostringstream oss;
  oss << "Curve_" << id_;
  set_name (oss.str());
}
LoggedVectorScigl::~LoggedVectorScigl()
{
}
/******************************************************************************************/
std::string
LoggedVectorScigl::dumpToString()
{
  std::stringstream ss;
  
  ss << Observer::dumpToString();
  ss << "LoggedVectorScigl::dumpToString()\n";
  
  return ss.str();
}
std::string
LoggedVectorTxt::toString()
{
  std::stringstream ss;

  ss << Observer::toString();
  ss << "LoggedVectorScigl::toString()\n";
  
  return ss.str();
}
/******************************************************************************************/
void
LoggedVectorScigl::update( int signal )
{
  // @todo Change ranges
  // @todo if internal data (if visualisation is faster than computation), change internal data
  // @todo maybe useful with threads
  // @todo maybe with several curves...
}
/******************************************************************************************/
void
LoggedVectorScigl::attach_to_coord( PlaneCoordPtr plane )
{
  _plane_coord = plane;
}
void
LoggedVectorScigl::set_thickness( float thickness )
{
  _thickness = thickness;
}
float
LoggedVectorScigl::get_thickness( void ) const
{
  return _thickness;
}
/******************************************************************************************/
void
LoggedVectorScigl::set_window_mode()
{
  //std::cout << "LoggedVectorScigl WINDOW\n";
  _fg_window_mode = true;
//   _plane_coord->set_sliding_coord1( true );
//   // restore stored values
//   _plane_coord->set_range_coord1( _stored_rg1 );
//   _plane_coord->set_slide_max1( _stored_sl1 );
}
void
LoggedVectorScigl::set_full_mode()
{
  //std::cout << "LoggedVectorScigl FULL\n";
  _fg_window_mode = false;
//   _plane_coord->set_sliding_coord1( false );
//   // store sliding window parameters
//   _stored_rg1 = _plane_coord->get_range_coord1();
//   _stored_sl1 = _plane_coord->get_slide_max1();
//   // Compute full range
//   if( _abs == -2 ) {
//     _plane_coord->set_range_coord1( Range( 0, _model->_data.size(), 
// 					   _stored_rg1.major,
// 					   _stored_rg1.minor ) );
//   }
//   else if( _abs == -1 ) {
//     _plane_coord->set_range_coord1( Range( _model->_data[0].t,
// 					   _model->_data[_model->_data.size()-1].t, 
// 					   _stored_rg1.major,
// 					   _stored_rg1.minor ) );
//   }
//   else {
//     _plane_coord->set_range_coord1( Range( _model->_data[0].v(_abs),
// 					   _model->_data[_model->_data.size()-1].v(_abs), 
// 					   _stored_rg1.major,
// 					   _stored_rg1.minor ) );
//   }
}
/******************************************************************************************/
void
LoggedVectorScigl::render (void)
{
  compute_visibility();
  if (!get_visible()) {
    return;
  }
  Color c;
  glColor4f(fg_color_.r, fg_color_.g, fg_color_.b,
	    fg_color_.a*alpha_);
  //std::cout << "Color " << fg_color_.r << ", " << fg_color_.g << ", " << fg_color_.b << ", " << fg_color_.a*alpha_ << "\n";
  // -------------------------------------------------------------------------
  //  Rendering using GL_LINE_STRIP
  // -------------------------------------------------------------------------
  if ((_thickness == 0) or (_thickness > 1.0)) {
    glEnable (GL_BLEND);
    glEnable (GL_LINE_SMOOTH);
    if (_thickness == 0)
      glLineWidth (1.0);
    else
      glLineWidth (_thickness);
    
    // XYZ
    // ---------------------------------------------------------------------
    if( _plane_coord ) {
      Position p = _plane_coord->get_position();
      Size s = _plane_coord->get_size();
      Range rgX = _plane_coord->get_range_coord1();
      Range rgY = _plane_coord->get_range_coord2();
      GLdouble xval = 0.0;
      glBegin(GL_LINE_STRIP);
      {
	// slide if needed
	if( _model->_data.size() > rgX.max ) {
	  _plane_coord->set_slide_max1( _model->_data.size() );
	}
	int i_start;
	if( _fg_window_mode ) {
	  i_start = std::max( 0, (int) _model->_data.size() - (int) _cache_size);
	}
	else {
	  i_start = 0;
	}
	for( unsigned int i = i_start; i < _model->_data.size(); i++ ) {
	  // select the right abcisse
	  if( _abs == -2 ) {
	    xval = (float) i;
	  }
	  else if( _abs == -1 ) {
	    xval = _model->_data[i].t;
	  }
	  else {
	    xval = _model->_data[i].v(_abs);
	  }
	  //std::cout << i << " [" << xval << "] " << (GLfloat) ((xval - rgX.min) / (rgX.max - rgX.min))  << " " <<  (GLfloat) ((_model->_data[i].v(1) - rgY.min) / (rgY.max - rgY.min)) << " ==> " << (GLfloat) ((xval - rgX.min) / (rgX.max - rgX.min) + p.x) * s.x << ", " << (GLfloat) ((_model->_data[i].v(1) - rgY.min) / (rgY.max - rgY.min) + p.y) * s.y << ", " << (GLfloat) ( 0.0 + p.z) * s.z << "\n";
	  
	  glVertex3f( (GLfloat) _plane_coord->get_projection_coord1( xval ),
		      (GLfloat) _plane_coord->get_projection_coord2( _model->_data[i].v(_dim) ),
		      (GLfloat) _plane_coord->get_projection_coord3( 0.0 ) );
	  //(GLfloat) (((*_iter_data).z - _rgZ.min) / (_rgZ.max - _rgZ.min) + p.z) * s.z);
	  //xval += 1.0;
	  }
      }
      glEnd();
    }
  }
  else {
    std::cout << "LoggedVectorScigl::render() NO _plane_coord\n";
  }
}
