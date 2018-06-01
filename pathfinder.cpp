#include "ActorGraph.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
  if (argv[1] == NULL || argv[2] == NULL){
    std::cout << "No input file\n";
    return 0;
  }

  ActorGraph *graph = new ActorGraph();
  graph->loadFromFile(argv[1],argv[2]);

  // Reading in from file 
  ifstream infile(argv[3]);
  
  bool have_header = false;

  // Keep reading lines until EOF is reached.
  while (infile){
    string s;
   
    // Get the next line
    if (!getline(infile,s)) break;

    if (!have_header){
      //skip the header
      have_header = true;
      continue;
    }

    istringstream ss(s);
    vector<string> record;

    while (ss) {
      string next;

      // get the next string before the tab char
      if (!getline(ss, next, '\t')) break;
      record.push_back(next);
    }

    if (record.size() != 2){
	// We should have exactly 2 columns
	continue;
    }
    string actor1(record[0]);
    string actor2(record[1]);
    
    graph->search(actor1, actor2, argv[3]);

  }

  
  //std::cout << "ActorGraph size: " << graph->actorGraph.size() << "\n";
  //cout << "movieGraph size: " << graph->movieGraph.size() << endl;
  //ActorNode *actor = graph->actorGraph["BACON, KEVIN (I)"];
  //for (int index = 0; index < actor->movieList.size(); index++){
  //  cout << "Movies for KB: " << actor->movieList[index] << endl;
  //}
  //graph->search("BACON, KEVIN (I)", "HOUNSOU, DJIMON");
  //graph->search("BACON, KEVIN (I)", "KIDMAN, NICOLE");
  //graph->search("BACON, KEVIN (I)", "WILLIS, BRUCE");
  //graph->search("BACON, KEVIN (I)", "GIAMATTI, PAUL");
  //graph->search("HOUNSOU, DJIMON", "50 CENT");


  return 0;
}
