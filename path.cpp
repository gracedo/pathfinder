/**
* File: path.cpp
* --------------
* This file implements the Path class.
*/
 
#include "path.h"
#include "strlib.h"
 
Path::Path()
{
  costs = 0;
}
 
Path::~Path()
{
  arcs.clear();
  nodes.clear();
}
 
void Path::add(Arc *arc)
{
  if(costs == 0) nodes.add(arc->start);
  arcs.add(arc);
  nodes.add(arc->finish);
  costs += arc->cost;
}
 
string Path::toString()
{
  string str;
  foreach(Arc *arc in arcs)
    str += arc->start->name + " -> " + arc->finish->name + " (" + realToString(arc->cost) + ")" + "\n";
     
  return str;
}
 
double Path::totalCost()
{
  return costs;
}
 
int Path::size()
{
  return arcs.size();
}
 
Arc* Path::getArc(int index)
{
  return arcs[index];
}
 
Set<Node *> Path::getNodes()
{
  return nodes;
}