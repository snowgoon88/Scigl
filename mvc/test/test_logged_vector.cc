// -*- coding: utf-8 -*-
/**
 * Test de LoggedVector
 * => ajout 
 * => dump
 * => clear
 */
#include <Eigen/Dense>
//#include "random.h"
#include <iostream>
#include <fstream>
#include "logged_vector.h"

//Random rrand;

void test_create()
{
  LoggedVector mem;
  std::cout << mem.dumpToString();

  std::cout << "Adding sample\n";
  Eigen::VectorXf v2(3);
  v2 << 1, 2, 3;
  mem.add_vector( 1.1, v2);
  std::cout << mem.dumpToString();

  std::cout << "Adding sample\n";
  v2 << 4, 4.1, 4.2;
  mem.add_vector( 2.0, v2);
  std::cout << mem.dumpToString();

  std::cout << "Clearing\n";
  mem.clear();
  std::cout <<  mem.dumpToString();

  std::cout << "--ATTACHING--\n";

  LoggedVectorTxt obs( &mem );
  mem.attach_observer( &obs );
  std::cout << mem.dumpToString();

  std::cout << "Adding sample\n";
  v2 << 1, 2, 3;
  mem.add_vector( 1.1, v2);
  std::cout << mem.dumpToString();
  
  std::cout << "Adding sample\n";
  v2 << 4, 4.1, 4.2;
  mem.add_vector( 2.0, v2);
  std::cout << mem.dumpToString();
  mem.notify_observers( 1 );

  std::cout << "Clearing\n";
  mem.clear();
  std::cout <<  mem.dumpToString();
  std::cout << "--END--\n";


}

int main( int argc, char *argv[] )
{
  //std::string filename( argv[1] );
  test_create();
  //test_wr();
  //test_online( 50, 5, 2 );
}
