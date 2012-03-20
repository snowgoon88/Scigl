// -*- coding: utf-8 -*-
#include "class1.h"
#include "class2.h"

void test()
{
  Class1 c1;
  c1.printId();
  std::cout << "----\n";

  Class2 c2;
  c2.printId();
  std::cout << "----\n";

  c1.attach( &c2 );
  c1.printId();
  std::cout << "----\n";

  c2.attach( &c1 );
  c2.printId();
  std::cout << "----\n";
}

int main( int argc, char *argv[] )
{
  test();

  return 0;
}
