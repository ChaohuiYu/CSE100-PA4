#include "ActorGraph.h"
#include "UnionFind.hpp"
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
  // Which kind of algorithm we are using
  string arg4 = argv[4];

  ActorGraph *graph = new ActorGraph();
  UnionFind *ufind = new UnionFind();

  // ActorGraph if using widestp
  if (arg4 == "widestp"){
    graph->loadFromFile(argv[1],"w");
  }
  // else using ufind
  else if (arg4 == "ufind"){
    ufind->loadFromFile(argv[1]);
  }

  // Reading in from file   
  ifstream infile(argv[2]);
  ofstream stream; // Ofstream
  stream.open(argv[3], ios::out);
  // Writing the header
  stream << "Actor1\tActor2\tYear\n";

  
  bool have_header = false;

  // Keep reading lines until EOF is reached.
  while (infile){

    //cout << "Inside pathfinder reading infile" << endl;
    string s;
   
    // Get the next line
    if (!getline(infile,s)) break;
    //cout << "string s: " << s << endl;

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
    string actor1 = record[0];
    string actor2 = record[1];
 
    // Using widest path alg
    if (arg4 == "widestp"){
      graph->search(actor1, actor2, stream, "w", true);
    }
    else if (arg4 == "ufind"){
      ufind->usearch(actor1, actor2, stream);
    }
    else{
      cout << "didn't go into search" << endl;
    }
  }

  if (!infile.eof()) {
        cerr << "Failed to read " << argv[1] << "!\n";
        return false;
  }
  infile.close();
  stream.close();

  delete graph;
  delete ufind;
  
  return 0;
}
