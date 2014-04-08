/*
* File: path.h
* ------------
* This file is the interface for a Path class, which consists of a
* sequence of arcs.
*/
 
#ifndef _path_h
#define _path_h
#include "graphtypes.h"
 
class Path {
public:
  Path();
  ~Path();
  void add(Arc *arc);
  string toString();
  double totalCost();
  int size();
  Arc* getArc(int index);
  Set<Node *> getNodes();
 
private:
  Vector<Arc *> arcs;
  Set<Node *> nodes;
  double costs;
};
 
#endif