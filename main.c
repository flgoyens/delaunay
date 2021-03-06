#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glfem.h"

#define GRAPHICS

int triangulation(char *FileName, const char *ResultName, const char *PlotName);

int main(void)
{   
		//test(42);
		//char *FileName="data_random100_000.txt";
		//char *FileName="triangles_6315.txt";
		//char *FileName="triangles_166.txt";
		char *FileName="data_randomGrid.txt";
		struct timespec start, finish;
		double elapsed;
		char *ResultName="solution.txt";
		char *PlotName="solutionPlot.txt";
				
		
		int nPrint = triangulation(FileName, ResultName,PlotName);
    		int iter=0;
		
		#ifdef GRAPHICS 
		
		glfemInit("Delaunay");
    		clock_gettime(CLOCK_MONOTONIC, &start);
    		do
    		{ 
    		clock_gettime(CLOCK_MONOTONIC, &finish);
    		elapsed=(finish.tv_sec - start.tv_sec);
    		//printf("%f \n",floor(elapsed));
    	    const char *basename = "%s-%08d.txt";
            char filename[256];
    	    sprintf(filename,basename,PlotName,(int)floor(elapsed));
    		    		
int w,h;
        glfwGetWindowSize(&w,&h);
        glfemReshapeWindows(filename,w,h);
        glfemPlotMesh(filename);// file name va chercher le fichier à lire mnt
        glfwSwapBuffers();    		
    		iter++;
    		}
    	while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
   		        glfwGetWindowParam(GLFW_OPENED) && elapsed <=nPrint-2);
    do
    {
        int w,h;
        glfwGetWindowSize(&w,&h);
        glfemReshapeWindows(PlotName,w,h);
        glfemPlotMesh(PlotName);// je crois que la fonction PlotMesh va prendre en argument PlotName pour aller lire dedans:-) 
        glfwSwapBuffers();
    }
    while( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED) );
           
    // Check if the ESC key was pressed or the window was closed 
    glfwTerminate(); 
    
    
		#endif
/* 
		elapsed = (finish.tv_sec - start.tv_sec);
		elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
		printf("time:%f\n",elapsed);
*/		
	 exit(0);
       
}


