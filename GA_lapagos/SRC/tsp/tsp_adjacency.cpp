/*
Copyright (c) 2019 Peter Andrew Jamieson (jamieson.peter@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/ 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "types.h"

#include "tsp_globals.h"
#include "tsp_types.h"
#include "tsp_adjacency.h"
#include "tsp_ga_lehmer_encoding.h"

/* Prototypes */

/* globals */
tsp_problem_adjacency_t *tsp_problem_adjacency;

// libxml includes
#include <libxml/xmlmemory.h> //#include <libxml/xmlmemory.h>
#include <libxml/parser.h> //#include <libxml/parser.h>

/*-------------------------------------------------------------------------
 * (function: read_tsp_benchmark)
 * Reads TSPLIB xml benchmarks.	 These are a vertex weigh form.
 * See: https://wwwproxy.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/index.html
 *-----------------------------------------------------------------------*/
void read_tsp_problem_adjacency(char *benchmark_name, tsp_problem_adjacency_t **tsp_problem_adjacency)
{
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlNodePtr pass;
	int edge_count = 0;
	int i;

	doc = xmlParseFile(benchmark_name);
	
	if (doc == NULL ) 
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) 
	{
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "travellingSalesmanProblemInstance")) 
	{
		fprintf(stderr,"document of the wrong type, root node != ga_config");
		xmlFreeDoc(doc);
		return;
	}

	/* allocate tsp bench */
	tsp_problem_adjacency[0] = (tsp_problem_adjacency_t*)malloc(sizeof(tsp_problem_adjacency_t));
	tsp_problem.num_cities = 0;

	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	{
		xmlChar *string_data;

		if ((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			
			tsp_problem_adjacency[0]->problem_name = (char*)malloc(sizeof(char)*(strlen((char*)string_data) + 1));
			strcpy(tsp_problem_adjacency[0]->problem_name, (char*)string_data);

			xmlFree(string_data);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"graph")))
		{
			xmlNodePtr vertex;
			xmlNodePtr edge;
			int vertex_count;
			int cur_edge_count;

			vertex_count = -1;
			vertex = cur->xmlChildrenNode;
			while (vertex != NULL) 
			{
				if ((!xmlStrcmp(vertex->name, (const xmlChar *)"vertex")))
				{
					vertex_count ++;

					if (edge_count == 0)
					{
						/* IF - the first time we count number of nodes...= edges + 1 */
						edge = vertex->xmlChildrenNode;

						while (edge != NULL)
						{
							if ((!xmlStrcmp(edge->name, (const xmlChar *)"edge")))
							{
								edge_count ++;
							}
							edge = edge->next;
						}

						edge_count ++;
						tsp_problem.num_cities = edge_count;

						/* initialize the adjacency matrix */
						printf("Initializing adjacency matrix with Vertexes = %d\n", edge_count); 

						/* allocate the adjacency matrix */
						tsp_problem_adjacency[0]->adjacency_matrix = (double**)malloc(sizeof(double*)*edge_count);
						for (i = 0; i < edge_count; i++)
						{
							tsp_problem_adjacency[0]->adjacency_matrix[i] = (double*)calloc(edge_count, sizeof(double));
						}
					}

					edge = vertex->xmlChildrenNode;
					cur_edge_count = -1;

					while (edge != NULL)
					{
						if ((!xmlStrcmp(edge->name, (const xmlChar *)"edge")))
						{
							int index;

							cur_edge_count ++;
							
							if (cur_edge_count == vertex_count)
								cur_edge_count++;
	
							string_data = xmlNodeListGetString(doc, edge->xmlChildrenNode, 1);
							index = atoi((char*)string_data);
							xmlFree(string_data);
							tsp_problem_adjacency[0]->adjacency_matrix[vertex_count][index] = atof((const char*)xmlGetProp(edge, (const xmlChar*)"cost"));	
						}
						edge = edge->next;
					}
				}	
				vertex = vertex->next;
			}
		}
		cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

/*-------------------------------------------------------------------------
 * (function: free_configuartion)
 *-----------------------------------------------------------------------*/
void free_tsp_problem_adjacency(tsp_problem_adjacency_t *tsp_problem_adjacency)
{
	int i; 

	if(tsp_problem_adjacency->problem_name != NULL)
	{
		free(tsp_problem_adjacency->problem_name);
	}	

	if (tsp_problem.num_cities != 0)
	{
		for (i = 0; i < tsp_problem.num_cities; i++)
		{
			free(tsp_problem_adjacency->adjacency_matrix[i]);
		}
		free(tsp_problem_adjacency->adjacency_matrix);
	}
		
	free(tsp_problem_adjacency);
}


/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function)
 *-------------------------------------------------------------------------------------------*/
double tsp_cost_function_from_adjacency_permutation(void *genome)
{
	int i;
	double cost = 0.0;
	int *solution = (int*)genome;

	for (i = 1; i < tsp_problem.num_cities; i++)
	{
		cost += tsp_problem_adjacency->adjacency_matrix[solution[i]][solution[i-1]];
	}

	return cost;
}

/*---------------------------------------------------------------------------------------------
 * (function: tsp_cost_function)
 *-------------------------------------------------------------------------------------------*/
double tsp_cost_function_from_adjacency_lehmer(void *genome)
{
	int i;
	double cost = 0.0;
	int *solution;

	/* decode lehmer encoding */
	solution = lehmer_decode((int*)genome, tsp_problem.num_cities);

	for (i = 1; i < tsp_problem.num_cities; i++)
	{
		cost += tsp_problem_adjacency->adjacency_matrix[solution[i]][solution[i-1]];
	}

	free(solution);

	return cost;
}


