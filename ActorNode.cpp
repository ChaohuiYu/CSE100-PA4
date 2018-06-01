#include <fstream>
#include <iostream>
#include <string>
#include <vector> 
#include <list>
#include "ActorNode.h"



/* Constructor */
ActorNode::ActorNode(std::string name){
  actorName = name;
  check = false;
  prev = nullptr;
}

/* Destructor */
ActorNode::~ActorNode(){

}
