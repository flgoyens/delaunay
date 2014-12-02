
/*
 *  delaunay.h
 */

#ifndef _DELAUNAY_H_
#define _DELAUNAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define Error(a)   Error(a,__LINE__,__FILE__)
#define Warning(a) Warning(a,  __LINE__, __FILE__)

/* //////////////////////////// Les STRUCTURES ///////////////////////////////////// */

typedef struct {
double x; 
double y;
int indice;
} Point; 



typedef struct {
Point *sommet0;
Point *sommet1;
Point *sommet2;
struct Edge *edge0;
struct Edge *edge1;
struct Edge *edge2; 
int indice; 
} Triangle; 

typedef struct Edge Edge;
struct Edge {
Triangle *elem0;
Triangle *elem1;
Point *P0;
Point *P1;
int indice; 
};

typedef struct myLeaf myLeaf;
struct myLeaf{
Triangle* theTriangle;
myLeaf* theChildren;// sans le "struct" a cette ligne il met "unknown type myleaf ..". Encore ?
int nChildren;
};

typedef struct {
  //myLeaf theRoot;
  myLeaf* theRoot;
} myTree;

typedef struct {
  Triangle* elem;
  Edge*     edges;
  Point *points;
  int nElem;// a tenir a jour
  int nNode;// fixe des le debut, on compte pas les p-1 et p-2
  int nEdge;// a tenir a jour
  myTree* theTree;
  Edge *edgeGlobal;
  Triangle *trigGlobal;
} Triangulation;

/* ////////////////////////////////// Les methodes //////////////////////////// */


//void 		    triangulation(char *FileName, const char *ResultName);
Triangulation      *triangulation(char *FileName, const char *ResultName);
Triangulation      *TriangulationCreate(char *FileName);
void                TriangulationWrite(const char *ResultName, Triangulation *theTriangulation);
void                TriangulationFree(Triangulation *theTriangulation);

void                findP0(Triangulation *theTriangulation);
void                AddPoint(Point *point, Triangulation *theTriangulation);
int 		    IsLegal(Edge *edge, Triangulation *theTriangulation);
void                LegalizeEdge(Point *point, Edge *edge, Triangulation *theTriangulation);
//void              PointLocate(Edge *edge,Triangle *trig,Point *point,Triangulation *theTriangulation, myLeaf *leaves);
void  		    PointLocate(Point *point,Triangulation *theTriangulation,myLeaf *leaves);
int                *ComputeRandom(int n);


int 		    withinTriangle(Point *point,Triangle *triangle);
double 		    ptNorm(Point point1, Point point2);
double 	            crossProd(Point point0, Point point1, Point point2);
double 	            triArea(Point point0, Point point1, Point point2);
void                onSide(Point *point, Triangle *triangle,Triangulation *theTriangulation);


/*

femMesh             *femMeshRead(const char *filename);
void                 femMeshWrite(const femMesh* myMesh, const char *filename);
void                 femMeshFree(femMesh *theMesh);

double               femMin(double *x, int n);
double               femMax(double *x, int n);
*/
//void                 Error(char *text, int line, char *file);
//void                 Warning(char *text, int line, char *file);

#endif
