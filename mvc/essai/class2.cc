// -*- coding: utf-8 -*-
#include "class2.h"

void Class2::printId()
{
  std::cout << "Class2::printId()\n";
  if( pc1 ) {
    std::cout << "With pc1\n";
  }
  else
    std::cout << "No pc1\n";
}
void Class2::attach( Class1Ptr c1 )
{
  pc1 = c1;
}
