// -*- coding: utf-8 -*-
#ifndef __CLASS2
#define __CLASS2

#include <iostream>

class Class1;
//typedef boost::shared_ptr< class Class2> Class2Ptr;
typedef Class1 * Class1Ptr;
#include "class1.h"

class Class2
{
 public:
  void printId();
  void attach( Class1Ptr c1 );

  Class1Ptr pc1;
};
#endif //__CLASS2
