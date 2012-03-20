// -*- coding: utf-8 -*-
#ifndef __LOGGED_VECTOR_H
#define __LOGGED_VECTOR_H

#include <vector>
#include <Eigen/Dense>
#include <string>
#include <iostream>
#include <list>

#include "model.h"

class LoggedVectorTxt;
#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVectorTxt> LoggedVectorTxtPtr;
#else
    typedef class LoggedVectorTxt * LoggedVectorTxtPtr;
#endif
#include "logged_vector_txt.h"

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class LoggedVector> LoggedVectorPtr;
#else
    typedef class LoggedVector * LoggedVectorPtr;
#endif

/**
 * Memorize a possibly infinite time-stamped sequence of Eigen::VectorXf.
 */
class LoggedVector : public Model
{
 public:
  typedef float T_Time;
  typedef Eigen::VectorXf T_Vect;
    
  struct S_Logged {
    T_Time t;
    T_Vect v;
  };
  typedef struct S_Logged T_Logged;

 public:
  /** Create: default*/
  LoggedVector( void );
  /** Create: copy*/
  LoggedVector( const LoggedVector &lv );
  /** Destruction */
  ~LoggedVector();

  /** dump to STR */
  std::string dumpToString();
  /** display to STR */
  std::string toString();

  /** Clear log */
  void clear();
  /** Add a new element at given time */
  void add_vector( T_Time t, T_Vect v); 

 public:
  /** What is memorized */
  std::vector <T_Logged> _data;

};
#endif //__LOGGED_VECTOR_H
