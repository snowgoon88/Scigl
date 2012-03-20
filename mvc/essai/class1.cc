// -*- coding: utf-8 -*-
#include "class1.h"

Class1::Class1()
{
}
void Class1::printId()
{
  std::cout << "Class1::printId()\n";
  if( pc2 )
    pc2->printId();
  else
    std::cout << "No pc2\n";
}
void Class1::attach( Class2Ptr c2 )
{
  pc2 = c2;
}
