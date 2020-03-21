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
#include <string.h>
#include <stdarg.h>
#include "types.h"
#include "globals.h"

#include "read_xml_config_file.h"
#include "utils.h"

// libxml includes
#include <libxml/xmlmemory.h> //#include <libxml/xmlmemory.h>
#include <libxml/parser.h> //#include <libxml/parser.h>

config_t configuration;

/*-------------------------------------------------------------------------
 * (function: read_config_file)
 * This reads an XML config file that specifies what we will do in the tool.
 *
 * See config_t in types.h to see the data structures used in this read.
 *-----------------------------------------------------------------------*/
void read_config_file(char *config_file_name)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(config_file_name);
	
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
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "ga_config")) 
	{
		fprintf(stderr,"document of the wrong type, root node != ga_config");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	{
		xmlChar *string_data;

		if ((!xmlStrcmp(cur->name, (const xmlChar *)"ga_type")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.ga_type = (enum GA_TYPE)return_string_in_list((char*)string_data, (char**)ga_types_name, NUM_GA_TYPES);
			xmlFree(string_data);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"crossover_type")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.crossover_type = (enum CROSSOVER_TYPE)return_string_in_list((char*)string_data, (char**)crossover_types_name, NUM_CROSSOVER_TYPES);
			xmlFree(string_data);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"selector_type")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.selector_type = (enum SELECTOR_TYPE)return_string_in_list((char*)string_data, (char**)selector_types_name, NUM_SELECTOR_TYPES);
			xmlFree(string_data);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"benchmark_file_name")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.benchmark_file_name = (char*)string_data;
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"problem_type")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.problem_type = (char*)string_data;
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"rand_seed")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.rand_seed = atoi((char*)string_data);
			xmlFree(string_data);
		}
		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"num_threads")))
		{
			string_data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			configuration.num_threads = atoi((char*)string_data);
			xmlFree(string_data);
		}

		else if ((!xmlStrcmp(cur->name, (const xmlChar *)"ga_params")))
		{
			xmlNodePtr ga_params = cur->xmlChildrenNode;
			while (ga_params != NULL) 
			{
				if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"num_population")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.num_population = atoi((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"percent_to_keep")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.percent_to_keep = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"percent_from_mutate")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.percent_from_mutate = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"percent_from_breed")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.percent_from_breed = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"percent_from_breed_and_mutate")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.percent_from_breed_and_mutate = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"percent_to_random")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.percent_to_random = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"top_percent_to_breed_from")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.top_percent_to_breed_from = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"top_percent_to_mutate_from")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.top_percent_to_mutate_from = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"top_percent_to_breed_mutate_from")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.top_percent_to_breed_mutate_from = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"percent_of_genome_mutations")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.percent_of_genome_mutations = atof((char*)string_data);
					xmlFree(string_data);
				}
				else if ((!xmlStrcmp(ga_params->name, (const xmlChar *)"tournament_size")))
				{
					string_data = xmlNodeListGetString(doc, ga_params->xmlChildrenNode, 1);
					genomes.tournament_size = atoi((char*)string_data);
					xmlFree(string_data);
				}



				ga_params = ga_params->next;
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
void free_configuration(char *config_file_name)
{
	if(configuration.benchmark_file_name != NULL)
		xmlFree(configuration.benchmark_file_name);
	if(configuration.problem_type != NULL)
		xmlFree(configuration.problem_type);
}
