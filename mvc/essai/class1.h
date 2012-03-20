// -*- coding: utf-8 -*-
#ifndef __CLASS1
#define __CLASS1

#include <iostream>
//#include <boost/shared_ptr.hpp>

class Class2;
//typedef boost::shared_ptr< class Class2> Class2Ptr;
typedef Class2 * Class2Ptr;
#include "class2.h"


class Class1
{
 public:
  Class1();
  void printId();
  void attach( Class2Ptr c2 );

  Class2Ptr pc2;
};
#endif //__CLASS1
