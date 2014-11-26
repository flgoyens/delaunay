/*
 *  delaunay.c
 *  
 */

#include "delaunay.h"
#define Error(a)   Error(a,__LINE__,__FILE__)
#define Warning(a) Warning(a,  __LINE__, __FILE__)

static Triangle *trigGlobal =NULL;
static Edge *edgeGlobal=NULL;

void triangulation(char *FileName, const char *ResultName)
{
printf("on est dans triangulation \n");

Triangulation *theTriangulation = TriangulationCreate(FileName);// doit aussi initialiser le triangle p0,p-1,p-2
/*printf("X[1]: %f \n",   theTriangulation->points[1].x);// OK 
printf("Y[1]: %f \n",   theTriangulation->points[1].y);// OK
printf("indice[1]: %d \n",   theTriangulation->points[1].indice);// OK*/

TriangulationWrite(ResultName, theTriangulation);

// TODO: ComputeRandom: un vecteur avec les nombres de 1 à n+1 mélangés pour voir dans quel ordre on ajoute les pts
// ce vecteur s'appelle de random. 
int *random = ComputeRandom(theTriangulation->nNode-1);

int i=0;
for(i=0; i<theTriangulation->nNode; i++)
{
    AddPoint(&(theTriangulation->points[random[i]]), theTriangulation);
	break;
}



// TODO: RemoveExtraPoints (enlever les points -1 et -2 ainsi que les edges qui les touchent)

TriangulationWrite(ResultName, theTriangulation);
TriangulationFree(theTriangulation);
}
/////////////////////////////////////////////////////////////////////////////////
void AddPoint(Point *point, Triangulation *theTriangulation)
{
	//Triangle *trig =NULL;
	//Edge *edge=NULL;
	trigGlobal =NULL;
	edgeGlobal=NULL;

	int nEdge=theTriangulation->nEdge;
	int nElem=theTriangulation->nElem;
	//PointLocate(edge, trig, point,theTriangulation,theTriangulation->theTree->theRoot); 
	PointLocate(point,theTriangulation,theTriangulation->theTree->theRoot); 
	
	if(trigGlobal != NULL)
	{// on ajoute point à l'intérieur du triangle trig
	 // il faut ajouter 3 edges vers les 3 sommets du triangle
	 	 printf("on ajoute des triangles \n ");
	/* AJOUT DES TRIG */
	theTriangulation->elem[nElem].sommet0=point;
	theTriangulation->elem[nElem].sommet1=trigGlobal->sommet0;
	theTriangulation->elem[nElem].sommet2=trigGlobal->sommet1;
	theTriangulation->elem[nElem].indice=nElem;
	
	theTriangulation->elem[nElem+1].sommet0=point;
	theTriangulation->elem[nElem+1].sommet1=trigGlobal->sommet1;
	theTriangulation->elem[nElem+1].sommet2=trigGlobal->sommet2;
	theTriangulation->elem[nElem+1].indice=nElem+1;
	
	Point *pt=theTriangulation->elem[trigGlobal->indice].sommet0;
	theTriangulation->elem[trigGlobal->indice].sommet0=point;
	theTriangulation->elem[trigGlobal->indice].sommet1=trigGlobal->sommet2;
//	theTriangulation->elem[trigGlobal->indice].sommet2=trigGlobal->sommet0;
	theTriangulation->elem[trigGlobal->indice].sommet2=pt;
	theTriangulation->elem[trigGlobal->indice].indice=trigGlobal->indice;
	
	theTriangulation->nElem=nElem+2;
	
	/* AJOUT DES EDGES */
	theTriangulation->edges[nEdge].P0=point;
	theTriangulation->edges[nEdge].P1=trigGlobal->sommet0;
	theTriangulation->edges[nEdge].indice=nEdge;	
	theTriangulation->edges[nEdge].elem0=&theTriangulation->elem[nElem];
	theTriangulation->edges[nEdge].elem1=&theTriangulation->elem[trigGlobal->indice];
		
	theTriangulation->edges[nEdge+1].P0=point;
	theTriangulation->edges[nEdge+1].P1=trigGlobal->sommet1;
	theTriangulation->edges[nEdge+1].indice=nEdge+1;
	theTriangulation->edges[nEdge+1].elem0=&theTriangulation->elem[nElem+1];
	theTriangulation->edges[nEdge+1].elem1=&theTriangulation->elem[nElem];
	
	theTriangulation->edges[nEdge+2].P0=point;
	theTriangulation->edges[nEdge+2].P1=trigGlobal->sommet2;
	theTriangulation->edges[nEdge+2].indice=nEdge+2;
	theTriangulation->edges[nEdge+2].elem0=&theTriangulation->elem[trigGlobal->indice];
	theTriangulation->edges[nEdge+2].elem1=&theTriangulation->elem[nElem+1];
	
	theTriangulation->nEdge=nEdge+3;
	
	LegalizeEdge(point, &theTriangulation->edges[nEdge], theTriangulation);
	LegalizeEdge(point, &theTriangulation->edges[nEdge+1], theTriangulation);
	LegalizeEdge(point, &theTriangulation->edges[nEdge+2], theTriangulation);
	
	}
	if(edgeGlobal != NULL)
	{// le point est sur l'edge
	// TODO 
	}
						            
	
	

}

//////////////////////////////////////////////////

int IsLegal(Edge *edge, Triangulation *theTriangulation)
{

return 1; // true
}

////////////////////////////////////////////////////////////////////////////////////////////////
int *ComputeRandom(int n)
{// returns a vector of int value from 1 to n in a random order
int *tab= malloc(sizeof(int)*n);
int i;
for(i=0;i<n;i++)// valeur par defaut non aléatoire :-)
	{tab[i]=i+1;}
return tab; 
}
/////////////////////////////////////////////////////////////////////////////////////////////


void PointLocate(Point *point,Triangulation *theTriangulation,myLeaf *leaves)
{
/* PointLocate: 
[[ATTENTION]]: Il faut mettre à jour des pointeurs static qui sont trigGlobal et edgeGlobal. Parce qu'on en a besoin dans une autre fonction
et un peu bizarrement c'est le seul truc que j'ai trouvé, quand j'y pense j'ai du louper quelque chose...
Normalement quand tu passe un pointeur en argument et que tu le modifie, il change et on a sa valeur modifiée partout dans le code. 
Je ne sais aps pourquoi cela n'a pas marché ..

 Il faut voir où se trouve *point et mettre à jour les pointeurs
*edge et *trig vers les éléments correspondant à la localisation de *point (un deux deux pointeurs doit être mis à NULL
puisque le point n'est pas à la fois sur une edge et dans un triangle).
*/

//Par défaut la fonction choisi que le point à chercher est dans le premier triangle (le grand triangle 0)
//et pas sur une edge. Il faudra modifier cela avec toute le systeme de recherche d'un point dans les arbres etc :-)

  int i = 0;
  
 /* while (leaves->theChildren[i] != NULL)
    {
      // TODO                                       [[[[[[[[[[[[[[[[[[[COMPILE PAS]]]]]]]]]]]]]]]]]]] 
    }*/ 

trigGlobal=&theTriangulation->elem[0];
//trigGlobal = leaves->theTriangle;// par defaut, le premier triangle initialisé 
edgeGlobal = NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////
void LegalizeEdge(Point *point, Edge *edge, Triangulation *theTriangulation)
{// TODO
	if( IsLegal(edge,theTriangulation)==0 )// TODO attention, IsLegal a peut être besoin de point pour être efficace 
	{					//et ne pas tester trop de truc (on sait ce qu'on vient d'ajouter)
						// au pire on fait le IsLegal dans le AddPoint et on appelera Legalize que s'il
						// faut legaliser :-)
	// 1) on switch
	// 2) on refait des LegalizeEdge 
	}

}
//////////////////////////////////// CREATE  ////////////////////////////////////////////////////////////
Triangulation *TriangulationCreate(char *FileName)
{
// lire le fichier data :-)

Triangulation *theTriangulation = malloc(sizeof(Triangulation));

    int i,trash;
    
    FILE* file = fopen(FileName,"r");
    //if (file == NULL) Error("No data file !",35,"something");

    trash = fscanf(file, "Number of nodes %d \n", &theTriangulation->nNode);
    int nNode=theTriangulation->nNode;
    
    theTriangulation->points = malloc(sizeof(Point)*(nNode+2));
    for (i = 0; i < nNode; ++i) {
	trash = fscanf(file,"%d : %le %le \n",&theTriangulation->points[i].indice,
					      &theTriangulation->points[i].x,
					      &theTriangulation->points[i].y);
         }
    theTriangulation->nElem = 1;    // valeur au départ
    
    findP0(theTriangulation);
        
    theTriangulation->elem  = malloc(sizeof(Triangle)*(2*nNode -2));//TODO ajuster ce nombre normalement ce sont des bornes sup 
    theTriangulation->edges = malloc(sizeof(Edge)*(3*nNode -3));// sur la taille possible des trucs (k=0 Thm9.1)
    
    // TODO traiter p-1 et p-2 de manière symbolique 
	
	// init p-1 en (0.0 ,-1.0)
	theTriangulation->points[nNode].x= 0.0;
	theTriangulation->points[nNode].y=-1.0;
	theTriangulation->points[nNode].indice=nNode;
	
	// init p-2 en (-10.0 , 10.0 )
	theTriangulation->points[nNode+1].x=-10.0;
	theTriangulation->points[nNode+1].y= 10.0;
	theTriangulation->points[nNode+1].indice=nNode+1;
	
	// init le triangle[0]	
	theTriangulation->elem[0].indice= 0;
	theTriangulation->elem[0].sommet0= &theTriangulation->points[0];     // pointe vers p0;
	theTriangulation->elem[0].sommet1= &theTriangulation->points[nNode];
	theTriangulation->elem[0].sommet2= &theTriangulation->points[nNode+1];
	//theTriangulation->elem[0].nodes[0]=0;
        //theTriangulation->elem[0].nodes[1]=nNode;
        //theTriangulation->elem[0].nodes[2]=nNode+1;	
        
        
        
    // initialiser les 3 first edges: 
    
    theTriangulation->edges[0].indice=0;//va de p0 à p-1
    theTriangulation->edges[0].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[0].elem1=NULL;
    theTriangulation->edges[0].P0=&theTriangulation->points[0];
    theTriangulation->edges[0].P1=&theTriangulation->points[nNode];

    theTriangulation->edges[1].indice=1;// va de p-1 à p-2
    theTriangulation->edges[1].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[1].elem1=NULL;
    theTriangulation->edges[1].P0=&theTriangulation->points[nNode];
    theTriangulation->edges[1].P1=&theTriangulation->points[nNode+1];


    theTriangulation->edges[2].indice=2;// va de p-2 à 0
    theTriangulation->edges[2].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[2].elem1=NULL;
    theTriangulation->edges[2].P0=&theTriangulation->points[nNode+1];
    theTriangulation->edges[2].P1=&theTriangulation->points[0];
    
    theTriangulation->nEdge=3;
    
    /*theTriangulation->edges[0].elem[0]= 0;
    theTriangulation->edges[0].elem[1]=-10;
    theTriangulation->edges[0].node[0]= ;
    theTriangulation->edges[0].node[1]= ;
    
    theTriangulation->edges[1].elem[0]= 0; 
    theTriangulation->edges[1].elem[1]= -10; 
    theTriangulation->edges[1].node[0]= ; 
    theTriangulation->edges[1].node[1]= ; 
    
    theTriangulation->edges[2].elem[0]= 0; 
    theTriangulation->edges[2].elem[1]= -10; 
    theTriangulation->edges[2].node[0]= ; 
    theTriangulation->edges[2].node[1]= ; */
    
    fclose(file);

     
    /*theTriangulation->theTree->theRoot->theTriangle = theTriangulation->elem;
    theTriangulation->theTree->theRoot->theChildren = NULL;*/ // [[[[[[[[[[[[[[[[[SEGMENTATION FAULT, il faut malloc ]]]]]]]]]]]]]]]]]
    
    return theTriangulation;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void findP0(Triangulation *theTriangulation)
{

//1) on cherche l'indice de P0
int ind=0;
int i;
for(i=1;i<theTriangulation->nNode; i++)
{
	if(theTriangulation->points[i].y > theTriangulation->points[ind].y)
	{
		ind =i;
	}
	else if(theTriangulation->points[i].y == theTriangulation->points[ind].y && theTriangulation->points[i].x > theTriangulation->points[ind].x ){
		ind = i;
	}
}

// 2) switch pour mettre P0 devant :-)
Point trash = theTriangulation->points[0]; 
theTriangulation->points[0]=theTriangulation->points[ind];
theTriangulation->points[ind]=trash;

theTriangulation->points[0].indice=0;
theTriangulation->points[ind].indice=ind;
}



void TriangulationFree(Triangulation *theTriangulation)
{
    free(theTriangulation->edges);
    free(theTriangulation->points);
    free(theTriangulation->elem);
    free(theTriangulation);
}


void TriangulationWrite(const char *ResultName,Triangulation *theTriangulation)
{
    int i;
    
    FILE* file = fopen(ResultName,"w");
    
    fprintf(file, "Number of nodes %d \n", theTriangulation->nNode);
    for (i = 0; i < theTriangulation->nNode+2; i++) {
        fprintf(file,"%6d : %14.7e %14.7e \n",i,theTriangulation->points[i].x,theTriangulation->points[i].y); }
    
  	     fprintf(file, "Number of triangles %d \n", theTriangulation->nElem);  
        for (i = 0; i < theTriangulation->nElem; i++) {
            //elem = &(theMesh->elem[i*3]);
            fprintf(file,"%6d : %6d %6d %6d \n", i,theTriangulation->elem[i].sommet0->indice,
            				           theTriangulation->elem[i].sommet1->indice,
            				           theTriangulation->elem[i].sommet2->indice);}
    
    fclose(file);
}
  
  
  /*
void femEdgesPrint(femEdges *theEdges)
{
    int i;    
    for (i = 0; i < theEdges->nEdge; ++i) {
        printf("%6d : %4d %4d : %4d %4d \n",i,
               theEdges->edges[i].node[0],theEdges->edges[i].node[1],
               theEdges->edges[i].elem[0],theEdges->edges[i].elem[1]); }
}

*/




/*
void Error(char *text, int line, char *file)                                  
{ 
    printf("\n-------------------------------------------------------------------------------- ");
    printf("\n  Error in %s at line %d : \n  %s\n", file, line, text);
    printf("--------------------------------------------------------------------- Yek Yek !! \n\n");
    exit(69);                                                 
}

void femWarning(char *text, int line, char *file)                                  
{ 
    printf("\n-------------------------------------------------------------------------------- ");
    printf("\n  Warning in %s at line %d : \n  %s\n", file, line, text);
    printf("--------------------------------------------------------------------- Yek Yek !! \n\n");                                              
}

*/


