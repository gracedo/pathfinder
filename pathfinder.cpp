/**
* File: pathfinder.cpp
* --------------------
* This contains the primary code that runs the pathfinder application.
*/
 
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "pathfinder-graph.h"
#include "pathfinder-graphics.h"
#include "error.h"
#include "gwindow.h"
#include "map.h"
#include "pqueue.h"
#include "point.h"
#include "tokenscanner.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "path.h"
using namespace std;
 
/* Constants */
const string FILE_PREPEND = "data-files/";
 
/* Function Prototypes */
static void giveInstructions();
static void mapAction(PathfinderGraph &g);
static void dijkstraAction(PathfinderGraph &g);
static void kruskalAction(PathfinderGraph &g);
static void quitAction();
void readMapData(PathfinderGraph &g);
void openDataFile(ifstream &infile);
void dataInput(ifstream &infile, PathfinderGraph &g);
void drawNodes(Set<Node *> nodes, string color);
void drawArcs(Set<Arc *> arcs, string color);
void getMouseClickLocations(PathfinderGraph &g, Node *&startNode, Node *&finishNode);
void clickAndHighlightNode(PathfinderGraph &g, Node *&currNode, string str);
Path findShortestPath(Node *start, Node *finish);
 
/**
* Function: main
* --------------
* Defines the entry point for the entire application.
*/
int main()
{
  PathfinderGraph g;
  GWindow gw(kWindowWidth, kWindowHeight + kControlStripHeight);
  initPathfinderGraphics(gw);
  giveInstructions();
  addButton("Map", mapAction, g);
  addButton("Dijkstra", dijkstraAction, g);
  addButton("Kruskal", kruskalAction, g);
  addButton("Quit", quitAction);
 
  ifstream infile; //default to usa map
  string filename = FILE_PREPEND + "usa.txt";
  infile.open(filename.c_str());
  dataInput(infile, g);
  drawNodes(g.getNodeSet(), kNodeColor);
  drawArcs(g.getArcSet(), kArcColor);
 
  pathfinderEventLoop();
 
  return 0;
}
 
/**
* Function: giveInstructions
* Usage: giveInstructions();
* --------------------------
* Describes the Pathfinder assignment on the console.  This text has
* been retained in its original form to preserve the assignment history.
*/
static void giveInstructions()
{
  cout << "This masterful piece of work is a graph extravaganza!" << endl;
  cout << "The main attractions include a lovely visual presentation" << endl;
  cout << "of the graph along with an implementation of Dijkstra's" << endl;
  cout << "shortest path algorithm and Kruskal's computation of" << endl;
  cout << "a minimal spanning tree.  Enjoy!" << endl << endl;
}
/* Sets up graphical interface according to user-designated map file */
static void mapAction(PathfinderGraph &g)
{
  g.clear();
  readMapData(g);
}
 
/* Executes Edsgar Dijkstra's algorithm for finding the shortest path
* betwen two nodes in the graph */
static void dijkstraAction(PathfinderGraph &g)
{
  drawPathfinderMap(g.getMapFile());
  drawNodes(g.getNodeSet(), kNodeColor);
  drawArcs(g.getArcSet(), kArcColor);
 
  Node *start, *finish;
  getMouseClickLocations(g, start, finish);
 
  Path shortestPath = findShortestPath(start, finish);
  if(shortestPath.totalCost() != 0)
  {
    Set<Arc *> highlightedPath;
    for (int i = 0; i < shortestPath.size(); i++)
      highlightedPath.add(shortestPath.getArc(i));
 
    cout << shortestPath.toString() << endl;
 
    drawNodes(g.getNodeSet(), kDimColor);
    drawNodes(shortestPath.getNodes(), kHighlightColor);
    drawPathfinderNode(start->loc, kHighlightColor, start->name);
    drawPathfinderNode(finish->loc, kHighlightColor, finish->name);
 
    drawArcs(g.getArcSet(), kDimColor);
    drawArcs(highlightedPath, kHighlightColor);
 
    cout << "The total cost of this path is: " << shortestPath.totalCost() << endl;
  }
  else cout << "No path found." << endl;
}
 
/* Executes Joseph Kruskal's algorithm for finding the lowest-cost, cycle-free
* sub-graph connecting all the nodes in the graph */
static void kruskalAction(PathfinderGraph &g)
{
  drawPathfinderMap(g.getMapFile());
 
  PriorityQueue<Arc *> queue;
  Vector<Set<Node *> > connectedNodeSet;
  Set<Arc *> minSpanTree;
 
  foreach(Arc *arc in g.getArcSet())
    queue.enqueue(arc, arc->cost);
     
  foreach(Node *node in g.getNodeSet())
  {
    Set<Node *> newSet;
    newSet.add(node);
    connectedNodeSet.add(newSet);
  }
 
  while(!queue.isEmpty())
  {
    Arc *arc = queue.dequeue();
 
    int currIndex = 0, index1 = 0, index2 = 0;
    foreach(Set<Node *> nodes in connectedNodeSet)
    {
      if(nodes.contains(arc->start))
        index1 = currIndex;
 
      if(nodes.contains(arc->finish))
        index2 = currIndex;
 
      currIndex++;
    }
 
    if(index1 != index2) //if not in same sets, merge
    {
      connectedNodeSet[index1] += connectedNodeSet[index2];
      connectedNodeSet.remove(index2);
      minSpanTree.add(arc);
    }
  }
 
  drawNodes(g.getNodeSet(), kHighlightColor);
  drawArcs(minSpanTree, kHighlightColor);
}
 
 
/**
* Function: quitAction
* Usage: quitAction(g);
* ---------------------
* Called when the user clicks the Quit button in the control strip.
*/
static void quitAction()
{
  exitGraphics();
}
 
/* Opens the file, inputs the data, and draws the map */
void readMapData(PathfinderGraph &g)
{
  ifstream infile;
  openDataFile(infile);
  dataInput(infile, g);
  drawNodes(g.getNodeSet(), kNodeColor);
  drawArcs(g.getArcSet(), kArcColor);
}
 
/* Prompts user for map file and opens it */
void openDataFile(ifstream &infile)
{
  string filename;
  string fullFilename;
  string txtSuffix = ".txt";
 
  while(true)
  {
    cout << "Please enter name of graph data file: ";
    filename = getLine();
    if(filename.find(txtSuffix) == string::npos) filename = filename + txtSuffix;
    fullFilename = FILE_PREPEND + filename;
    infile.open(fullFilename.c_str());
    if(infile.fail())
    {
      cout << "Unable to open file named " + filename + ". Please try again." << endl;
      infile.clear();
    } else break;
  }
}
 
/* Sets up the graphical display and mapping based on map file */
void dataInput(ifstream &infile, PathfinderGraph &g)
{
  TokenScanner scanner;
  Map<string, Node *> nodeMap;
  Node* node;
  Set<Node *> allNodes;
  string line, place;
  double xcoord, ycoord;
 
  getline(infile,line);
  string imageFilename = "images/" + line;
  g.setMapFile(imageFilename);
  drawPathfinderMap(g.getMapFile()); //set the map picture
 
  getline(infile,line);
  if(line != "NODES") error("Your file is formatted differently.");
 
  Arc* arc;
  Set<Arc *> allArcs;
  getline(infile,line);
  while(line != "ARCS") //input node data into graph
  {
    scanner.setInput(line);
    scanner.ignoreWhitespace();
    string place = scanner.nextToken();
    scanner.scanNumbers();
    xcoord = stringToReal(scanner.nextToken());
    scanner.scanNumbers();
    ycoord = stringToReal(scanner.nextToken());
         
    GPoint pt(xcoord, ycoord);
    node = new Node();
    node->name = place;
    node->loc = pt;
    allNodes.add(node);
    g.addNode(node);
    nodeMap[place] = node;
 
    getline(infile,line);
  }
 
  string start, finish;
  double cost;
  getline(infile,line);
  while(line != "") //input arc data into graph
  {
    scanner.setInput(line);
    start = scanner.nextToken();
    finish = scanner.nextToken();
    scanner.scanNumbers();
    cost = stringToReal(scanner.nextToken());
 
    arc = new Arc();
    arc->start = nodeMap[start];
    arc->finish = nodeMap[finish];
    arc->cost = cost;
    allArcs.add(arc);
    g.addArc(arc);
    nodeMap[start]->arcs.add(arc);
         
    arc = new Arc();
    arc->start = nodeMap[finish];
    arc->finish = nodeMap[start];
    arc->cost = cost;
    allArcs.add(arc);
    g.addArc(arc);
    nodeMap[finish]->arcs.add(arc);
 
    getline(infile,line);
  }
 
  infile.close();
}
 
/* Draws the nodes */
void drawNodes(Set<Node *> nodes, string color)
{
  foreach(Node *node in nodes)
    drawPathfinderNode(node->loc, color, node->name);
}
 
/* Draws the arcs */
void drawArcs(Set<Arc *> arcs, string color)
{
  foreach(Arc *arc in arcs)
    drawPathfinderArc(arc->start->loc, arc->finish->loc, color);
}
 
/* Wrapper for clickAndHighlightNode function */
void getMouseClickLocations(PathfinderGraph &g, Node *&startNode, Node *&finishNode)
{
  clickAndHighlightNode(g, startNode, "starting");
  clickAndHighlightNode(g, finishNode, "ending");
}
 
/* Update mouseclick locations and highlights nodes clicked on */
void clickAndHighlightNode(PathfinderGraph &g, Node *&currNode, string str)
{
  GPoint mousePt, currPt;
  while(true)
  {
    cout << "Click on " << str << " location..." << endl;
    mousePt = getMouseClick();
 
    bool nodeFound = false;
    double mouseX = mousePt.getX();
    double mouseY = mousePt.getY();
    double xcoord, ycoord;
 
    foreach(Node *node in g.getNodeSet())
    {
      currNode = node;
      currPt = node->loc;
      xcoord = currPt.getX();
      ycoord = currPt.getY();
      double dist = sqrt((mouseX-xcoord)*(mouseX-xcoord) + (mouseY-ycoord)*(mouseY-ycoord));
      if(dist <= kNodeRadius) //in node
      {
        nodeFound = true;
        break;
      }
    }
    if(!nodeFound) cout << "Did not click on node. Try again." << endl;
    else
    {
      g.setHighlightedNode(currNode);
      drawPathfinderNode(currNode->loc, kHighlightColor, currNode->name);
      break;
    }
  }
}
 
/* Employs Dijkstra's algorithm to find the shortest path between two nodes */
Path findShortestPath(Node *start, Node *finish)
{
  Path path;
  PriorityQueue<Path> queue;
  Map<string, double> fixed;
 
  while(start != finish)
  {
    if(!fixed.containsKey(start->name))
    {
      fixed.put(start->name, path.totalCost());
      foreach(Arc *arc in start->arcs)
      {
        if(!fixed.containsKey(arc->finish->name))
        {
          Path newPath = path;
          newPath.add(arc);
          queue.enqueue(newPath, newPath.totalCost());
        }
      }
    }
    if(queue.isEmpty()) return path;
    path = queue.dequeue();
    start = path.getArc(path.size()-1)->finish;
  }
 
  return path;
}