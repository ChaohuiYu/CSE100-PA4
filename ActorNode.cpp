#include <fstream>
#include <iostream>
#include <string>
#include <vector> 
#include <list>
#include <limits>
#include "ActorNode.h"



/* Constructor */
ActorNode::ActorNode(std::string name){
  actorName = name;
  check = false;
  prev = nullptr;
  prevMovieWeight = 2147483647;
  totalDist = 2147483647;
  bandwidth = 0;
  rank = 1;
  prevMovieYear = 0;
}

/* Destructor */
ActorNode::~ActorNode(){

}
