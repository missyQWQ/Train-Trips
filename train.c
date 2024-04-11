// Author: Yichun ZHANG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****Data-structure*****/

/* Define the structure 'path' with two elements. 
** 1) 'dis' represented the distance between two stations.
** 2) 'dst' represented the mark number of destination.
*/
struct path
{
	int dis;
	int dst;
};

/* Define the structure 'city' with two elements. 
** 1) A city name (which is a string).
** 2) Data-structure of 'path' (which has been defined above).
*/
struct city
{
	char *name;
	struct path *city_path;
};

/* Typedef path to a public name 'Path'. */
typedef struct path Path;
/* Typedef city to a public name 'City'. */
typedef struct city City;

/*****************************************************************************/


/*****Functions*****/

/* Function to check whether the station names are repeated. */
int isRepeat(char *tgt, City *list, int idx);

/* Function to check whether there exist enough space to malloc. */
void isAllocate(void *space);

/* Function to expand the length of city names. 
** If the length of city name beyond the boundary,
** then double the size of the original malloc.
*/
void expand_char(char **old, int *block);

/* Function to expand the size of 'City' structure. 
** If the size beyond the boundary,
** then double the size of the original malloc.
*/
void expand_City(City **old, int *block, int idx);

/* Function to free the malloc of 'City' structure. */
void free_city(City **city, int city_num);

/* Function to print error messages. */
void error_msg(int exit_code);

/*****************************************************************************/


/*****Main-function*****/

int main(int argc, char const *argv[])
{
	int exit_code = 0;

	/* Check the numbers of parameters in command line which must be 2. */
	if (argc != 2)
	{
		exit_code = 4;
		error_msg(exit_code);
	}

	/* Open the file, read only. */
	FILE *f = NULL;
	f = fopen(argv[1], "r");
	if (f == NULL)
	{
		exit_code = 1;
		error_msg(exit_code);
	}

	/* Check the first character which must be ',' otherwise print error message. */
	char firstC;
	fscanf(f, "%c", &firstC);
	if (firstC != ',')
	{
		exit_code = 2;
		error_msg(exit_code);
	}

	/* Malloc memory. */
	int char_block = 32;
	int city_block = 256;
	City *city_list = (City*)malloc(sizeof(City) * city_block);
	isAllocate(city_list);
	char *cur_name = (char*)malloc(sizeof(char) * char_block);
	isAllocate(cur_name);

	/* Read in all the station names one character by one character
	** and store them in the 'name' part of 'City' structure. */
	int i = 0;
	int idx = 0;
	char c;
	fscanf(f, "%c", &c);

	while (c != '\n')
	{
		if (c != ',')
		{
			if (i >= char_block)
			{
				expand_char(&cur_name, &char_block);
			}
			cur_name[i] = c;
			i++;
		}
		else
		{
			if (idx >= city_block)
			{
				expand_City(&city_list, &city_block, idx);
			}

			cur_name[i] = '\0';
			city_list[idx].name = cur_name;
			city_list[idx].city_path = NULL;

			if (isRepeat(cur_name, city_list, idx))
			{
				exit_code = 2;
				error_msg(exit_code);
			}

			idx++;
			i = 0;
			char_block = 32;
			cur_name = (char*)malloc(sizeof(char) * char_block);
			isAllocate(cur_name);
		}
		fscanf(f, "%c", &c);
	}

	cur_name[i] = '\0';
	city_list[idx].name = cur_name;
	city_list[idx].city_path = NULL;

	if (isRepeat(cur_name, city_list, idx))
	{
		exit_code = 2;
		error_msg(exit_code);
	}

	int row = idx;

	/* Read in the second line of the file,
	** and compare the station name with city_list,
	** which contains all the station names,
	** and store the distance to the 'dis' part of city_path,
	** and set the mark number of destination in the 'dst' part.
	*/
	while (!feof(f))
	{
		fscanf(f, "%c", &c);

		/* Ignore the blank lines. */
		if (c == '\n')
		{
			continue;
		}

		/* Malloc memory for cur_name. */
		int j = 0;
		char_block = 32;
		cur_name = (char*)malloc(sizeof(char) * char_block);
		isAllocate(cur_name);

		/* Read in station name. */
		while (c != ',')
		{
			if (j >= char_block)
			{
				expand_char(&cur_name, &char_block);
			}
			cur_name[j] = c;
			j++;
			fscanf(f, "%c", &c);
		}
		cur_name[j] = '\0';

		/* Compare the current station name with the city list, 
		** which contains all the station names. */
		int k = 0;
		while (k <= idx)
		{
			if (strcmp(cur_name, city_list[k].name) == 0)
			{
				free(cur_name);
				break;
			}
			k++;
		}

		/* If the current station name is not contained in the
		** city list, then add the current station name to the
		** city list. */
		if (k == idx + 1)
		{
			if (row >= city_block - 1)
			{
				expand_City(&city_list, &city_block, idx);
			}
			city_list[++row].name = cur_name;
			city_list[row].city_path = NULL;
			k = row;
		}

		/* Malloc memory for city_path. */
		city_list[k].city_path = malloc(sizeof(Path) * (idx + 2));
		isAllocate(city_list[k].city_path);

		/* Read in the distance between two stations and store them
		** in the 'dis' part of city_path,
		** and set the mark number of destination in the 'dst' part.
		*/
		int cur_dis = 0;
		int path_num = 0;
		int city_idx = 0;
		int clm = 0;
		fscanf(f, "%c", &c);
		while (c != '\n')
		{
			if (c == ',')
			{	
				/* If the distance is not empty. */
				if (cur_dis != 0)
				{
					city_list[k].city_path[path_num].dis = cur_dis;
					city_list[k].city_path[path_num].dst = city_idx;
					path_num++;
					cur_dis = 0;
				}
				city_idx++;
				clm++;
			}

			/* Make sure the distance is a positive non-zero integer. */
			else if (c < '0' || c > '9')
			{
				exit_code = 2;
				error_msg(exit_code);
			}

			/* Change ASCII code to integer numbers. */
			else
			{
				if (cur_dis == 0)
				{
					cur_dis = c - 48;
				}
				else
				{
					cur_dis = cur_dis * 10 + (c - 48);
				}
			}

			/* Exit if it's end of file. */
			if (fscanf(f, "%c", &c) == EOF)
			{
				if (cur_dis != 0)
				{
					city_list[k].city_path[path_num].dis = cur_dis;
					city_list[k].city_path[path_num].dst = city_idx;
					path_num++;
				}
				break;
			}

		}
		if (cur_dis != 0)
		{
			city_list[k].city_path[path_num].dis = cur_dis;
			city_list[k].city_path[path_num].dst = city_idx;
			path_num++;
		}

		/* Add '-1' to the end of each city_path. */
		city_list[k].city_path[path_num].dis = -1;
		if (clm != idx)
		{
			exit_code = 2;
			error_msg(exit_code);
		}
	}

	/* Prompt and get input of start station from user. 
	** One character by one character. */
	char_block = 32;
	printf("Start station: ");
	scanf("%c", &c);
	while (c != '\n')
	{
		/* Malloc memory for user's input of start station and end station. */
		char *start = (char*)malloc(sizeof(char) * char_block);
		isAllocate(start);
		char *end = (char*)malloc(sizeof(char) * char_block);
		isAllocate(end);

		/* Read in start station. */
		int cnt = 0;
		int start_node = -1;
		int end_node = -1;
		while (c != '\n')
		{
			if (cnt >= char_block)
			{
				expand_char(&start, &char_block);
			}
			start[cnt] = c;
			cnt++;
			scanf("%c", &c);
		}
		start[cnt] = '\0';

		/* Compare the start station with city_list. */
		for (int num=0; num<=row; num++)
		{
			if (strcmp(start, city_list[num].name) == 0)
			{
				start_node = num;
				break;
			}
		}

		if (start_node == -1)
		{
			printf("No such station.\n");
		}
		else
		{
			/* Prompt and get input of end station from user. 
			** One character by one character. */
			char_block = 32;
			printf("End station: ");
			scanf("%c", &c);

			/* Read in end station. */
			int cnt = 0;
			while (c != '\n')
			{
				if (cnt >= char_block)
				{
					expand_char(&end, &char_block);
				}
				end[cnt] = c;
				cnt++;
				scanf("%c", &c);
			}
			end[cnt] = '\0';

			/* Compare the end station with city_list. */
			for (int num=0; num<=row; num++)
			{
				if (strcmp(end, city_list[num].name) == 0)
				{	
					end_node = num;
					break;
				}
			}

			if (end_node == -1)
			{
				printf("No such station.\n");
			}
			else if(strcmp(start, end) == 0)
			{
				printf("No journey, same start and end station.\n");
			}
			else
			{
				/* Both station names are valid, then use
				** Dijkstra Algorithm 
				** to calculate the shortest journey. */

				/* Initialize */
				int node_value[row+1];
				int node_mark[row+1];
				int pre_node[row+1];
				for (int cnt = 0; cnt <= row; cnt++)
				{
					node_value[cnt] = -1;
					node_mark[cnt] = 1;
					pre_node[cnt] = -1;
				}

				// Set the value of the start station as 0
				node_value[start_node] = 0;

				/* Find the shortest distance. */
				for (int i = 0; i <= row; i++)
				{
					/* Find the the node(station) whose value is minimum. */
					int min = -1;
					int min_node;
					for (int cnt = 0; cnt <= row; cnt++)
					{
						if ((node_value[cnt] < min || min == -1) && node_mark[cnt] && node_value[cnt] != -1)
						{
							min_node = cnt;
							min = node_value[cnt];
						}
					}

					// min equals 1 means there is no route exist.
					if (min == -1)
					{
						break;
					}

					// Update all the node values which direct-linked with the minimum node.
					int i = 0;
					int pass_node, pass_edge;
					while (city_list[min_node].city_path[i].dis != -1)
					{
						pass_node = city_list[min_node].city_path[i].dst;
						pass_edge = city_list[min_node].city_path[i].dis;
						if ((node_value[pass_node] == -1) || (node_value[pass_node] > node_value[min_node] + pass_edge))
						{
							node_value[pass_node] = node_value[min_node] + pass_edge;
							pre_node[pass_node] = min_node;
						}
						i++;
					}

					// Set the mark of the minmum node as 0 to show that it has been viewed.
					node_mark[min_node] = 0;
				}

				// No possible journey if the value of the end_node is not updated.
				if (node_value[end_node] == -1)
				{
					printf("No possible journey.\n");
				}
				else
				{
					// The shorest journey is the value of the end node.
					const int shortest_dis = node_value[end_node];

					/* Find the pass-by node of the shortest distance. */
					int shortest_path[row+1];
					int cur_node = end_node;
					int cnt = 0;

					/* Store all the pass-by in the array: shortest_path,
					** but note that they are stored from end to start,
					** which means inverted order.
					*/
					while (pre_node[cur_node] != start_node)
					{
						shortest_path[cnt] = pre_node[cur_node];
						cur_node = pre_node[cur_node];
						cnt++;
					}
					const int shortest_node = cnt;
					
					/* Output */
					printf("From %s\n", start);
					if (pre_node[end_node] == start_node)
					{
						printf("direct\n");
					}
					else
					{
						printf("via\n");
						/* Invert the ouput order (from start to end) of the array. */
						for (int i = shortest_node - 1; i >= 0; i--)
						{
							printf("%s\n", city_list[shortest_path[i]].name);
						}
					}
					printf("To %s\n", end);
					printf("Distance %d km\n", shortest_dis);

					/* Calculate the cost. 
					** Use integer calculation to gurantee the precision. */
					int cost = shortest_dis * 12 + shortest_node * 250;
					if (cost % 10 > 0)
					{
						cost = cost / 10 + 1;
					}
					else
					{
						cost = cost / 10;
					}
					printf("Cost %d RMB\n", cost);	
				}	
			}
		}
		// Free the memory of start and end.
		free(start);
		free(end);

		printf("Start station: ");
		scanf("%c", &c);
	}

	// Free the memory of City structure.
	free_city(&city_list, row);

	// Close the file.
	fclose(f);
	return 0;
}

/*****************************************************************************/


/*****Functions*****/

/* Return 1 if 'tgt' exist it the 'list',
** otherwise return 0. */
int isRepeat(char *tgt, City *list, int idx)
{
	int ans = 0;
	for (int cnt = 0; cnt < idx; cnt++)
	{
		if (strcmp(tgt, list[cnt].name) == 0)
		{
			ans = 1;
		}
	}
	return ans;
}

/* exit with exit code 3 if unable to malloc. */
void isAllocate(void *space)
{
	if (space == NULL)
	{
		int exit_code = 3;
		error_msg(exit_code);
	}
	return;
}

/* Expand the length of city names: 'old'.
** If the size beyond one 'block',
** then double the size of 'old'. */
void expand_char(char **old, int *block)
{
	char *new = (char*)malloc(sizeof(char) * (*block * 2));
	if (new == NULL)
	{
		free(*old);
		isAllocate(new);
	}
	for (int i = 0; i < *block; i ++)
	{
		new [i] = (*old) [i];
	}
	new [*block] = '\0';
	*block = *block * 2;
	free(*old);
	*old = new;
	return;
}

/* Expand the size of 'City' structure. 
** If the size beyond one 'block',
** then double the size of 'old'. */
void expand_City(City **old, int *block, int idx)
{
	City *new = (City *)malloc(sizeof(City) * (*block * 2));

	if (new == NULL)
	{
		free(*old);
		isAllocate(new);
	}
	
	// Copy old to new.
	for (int i = 0; i < *block ; i++)
	{

		new[i].name = (*old)[i].name;
		if ((*old)[i].city_path != NULL)
		{
			new[i].city_path = malloc (sizeof (Path) * (idx + 1));	
			int j = 0;
			while ((*old)[i].city_path[j].dis != -1)
			{
				new[i].city_path[j].dis = (*old)[i].city_path[j].dis;
				new[i].city_path[j].dst = (*old)[i].city_path[j].dst;
				j++;
			}
			new[i].city_path[j].dis = -1;
			free ((*old)[i].city_path);
		}
		else
		{
			new[i].city_path = NULL;
		}
	}

	free (*old);
	*old = new;
	*block = *block * 2;
	return;
}

/* Free the malloc of city */
void free_city(City **city, int city_num)
{
	for (int i = 0; i <= city_num; i++)
	{
		if ((*city)[i].city_path != NULL)
		free((*city)[i].city_path);
		free((*city)[i].name);
	}
	free(*city);
	return;
}

/* Print error messages.
** exit with exit code 1 when fail to open the file.
** exit with exit code 2 when invalid files.
** exit with exit code 3 when it cannot allocate memory.
** exit with exit code 4 otherwise.
*/
void error_msg(int exit_code)
{
	if (exit_code == 1)
	{
		perror("Cannot open file.");
		exit(1);
	}

	if (exit_code == 2)
	{
		printf("Invalid disances file.\n");
		exit(2);
	}

	if (exit_code == 3)
	{
		printf("Unable to allocate memory.\n");
		exit(3);
	}

	if (exit_code == 4)
	{
		printf("Invalid command line arguments. Usage: train <disances.txt>\n");
		exit(4);
	}

	return;
}