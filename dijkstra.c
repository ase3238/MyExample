/**
 * Data Structures homework #7
 *
 * "Dijkstra algorithm"
 * - on an undirected weighted graph with 200 vertices labeled 1 to 200
 * - run Dijkstra's shortest-path algorithm on this graph, using 1 (the first
 *   vertex) as the source vertex, and to compute the shortest-path distances
 *   between 1 and every other vertex of the graph. If there is no path between
 *   a * vertex v and vertex 1, we'll define the shortest-path distance between
 *   1 and v to be 1000000. 
 *
 * @ Apr. 24 2017
 * @ Jeongyeup Paek
 **/

/*
 * DATA STRUCTURE HW7
 * DIJKSTRA.C
 * KYUNG SU HYUN 20161202
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN64   // define something for Windows (64-bit)
    #include <time.h>
#elif _WIN32    // define something for Windows (32-bit)
    #include <time.h>
#elif __APPLE__ // apple
    #include <sys/time.h>
#elif __linux   // linux
    #include <time.h>
#elif __unix    // Unix
    #include <time.h>
#elif __posix   // POSIX
    #include <time.h>
#endif
 

#define MAX_DISTANCE    1000000     // if no path exists
#define MAX_NUM_NODES   200         // 200 vertices labeled 1 to 200
#define MAX_VERTEX_ID   200         // 200 vertices labeled 1 to 200

//typedef struct edge m_edge;

struct vertex {
    int             id;             // node id
    unsigned int    dist;           // minimum distance to source
	int				numEdge;		// number of edges
	struct edge**	edges;			// edges which start from this vertex
	struct vertex*	from;			// when using dijkstra, can find what is vertex which is before this node;
	int done;						// check dijkstra ran (0: don;t / 1: done)
};

struct vertex *m_varray[MAX_NUM_NODES]; // array of all vertices

struct edge {
	struct vertex* start;
	struct vertex* end;
	unsigned int weight;
};

int m_n = 0;	// total number of vertices;
int m_e = 0;    // total number of i->j edges. 
                // 'm_e/2' is the actual number of undirected edges

struct vertex* find_vertex (unsigned int id) {
	int i;
    if (id > MAX_VERTEX_ID) {
        printf("%s: error, invalid vertex id %d\n", __func__, id);
        return NULL;
    }
	for (i = 0; i < m_n; i++) {
		if (m_varray[i] == NULL) {
			continue;
		}
		if (m_varray[i]->id == id) {
			return m_varray[i];
		}
	}
    return NULL;
}

// find vertex and add if does not exist
struct vertex* add_vertex (unsigned int id) {
    struct vertex *v;

    if (id > MAX_VERTEX_ID) {
        printf("%s: error, invalid vertex id %d\n", __func__, id);
        return NULL;
    }
    if ((v = find_vertex(id)) != NULL) {
        return v;
    }
	v = (struct vertex*)calloc(1, sizeof(struct vertex));
	v->id = id;
	v->dist = MAX_DISTANCE;
	v->numEdge = 0;

	m_varray[m_n] = v;
	m_n++;

    return v;    // TODO
}

// delete all malloc'ed memory (edges and vertices)
void delete_graph() {
	int i, j;
	for (i = 0; i < m_n; i++) {
		for (j = 0; j < m_varray[i]->numEdge; j++) {
			free(m_varray[i]->edges[j]);
		}
		free(m_varray[i]->edges);
		free(m_varray[i]);
	}
	m_n = 0;
	m_e = 0;
}

// print shortest distnace of all nodes
void print_all_shortest_distance () {
    int i;
	int n = 0;
    for (i = 0; i < m_n; i++) {
		if (m_varray[i] == NULL) {
			continue;
		}
		if (m_varray[i]->dist == MAX_DISTANCE) {
			continue;
		}
		printf("vertex\t%d: shortest distance\t%d\n", m_varray[i]->id, m_varray[i]->dist);
		n++;
    }
	printf("number of nodes processed = %d\n", n);
}


// print shortest path of all nodes
void print_all_shortest_path () {
	int* stack;
	struct vertex* current;
	int size, index;
	int i;
	for (i = 0; i < m_n; i++) {
		size = 8;
		index = -1;
		stack = (int*)calloc(8, sizeof(int));
		current = m_varray[i];
		while (current != NULL) {
			index++;
			if (index + 1 >= size) {
				stack = (int*)realloc(stack, (size * 2) * sizeof(int));
				size *= 2;
			}
			stack[index] = current->id;
			current = current->from;
		}
		printf("vertex\t%d: shortest path\n", m_varray[i]->id);
		for (; index > 0; index--) {
			printf("%d->", stack[index]);
		}
		printf("%d\n", stack[index]);
		free(stack);
	}
}

struct edge* find_edge (unsigned int fromid, unsigned int toid) {
    struct vertex* v = find_vertex(fromid);
	int i;
	for (i = 0; i < v->numEdge; i++) {
		if (v->edges == NULL) {
			return NULL;
		}
		if (v->edges[i] == NULL) {
			continue;
		}
		if (v->edges[i]->end->id == toid) {
			return v->edges[i];
		}
	}
    return NULL;
}

// find edge and add if does not exist
struct edge* add_edge (unsigned int fromid, unsigned int toid,
                       unsigned int ecost) {
    struct vertex *v, *u;
	struct edge *e1, *e2;

    if (fromid > MAX_VERTEX_ID || toid > MAX_VERTEX_ID) {
        printf("%s: error, invalid vertex id %d->%d\n", __func__, fromid, toid);
        return NULL;
    }
	if ((e1 = find_edge(fromid, toid)) != NULL) {
		return e1;
	}
	v = add_vertex(fromid);
	u = add_vertex(toid);

	e1 = (struct edge*)calloc(1, sizeof(struct edge));
	e1->start = v;
	e1->end = u;
	e1->weight = ecost;
	if (v->edges != NULL) {
		v->edges = (struct edge**)realloc(v->edges, (v->numEdge + 1) * sizeof(struct edge*));
	}
	else {
		v->edges = (struct edge**)calloc(1, sizeof(struct edge*));
	}
	v->edges[v->numEdge] = e1;
	v->numEdge++;

	e2 = (struct edge*)calloc(1, sizeof(struct edge));
	e2->start = u;
	e2->end = v;
	e2->weight = ecost;
	if (u->edges != NULL) {
		u->edges = (struct edge**)realloc(u->edges, (u->numEdge + 1) * sizeof(struct edge*));
	}
	else {
		u->edges = (struct edge**)calloc(1, sizeof(struct edge*));
	}
	u->edges[u->numEdge] = e2;
	u->numEdge++;

	m_e = m_e + 2;

    return e1;
}


// run Dijkstra algorithm
void run_dijkstra() {
	int i;
	int n = 1;
	unsigned int minDist = MAX_DISTANCE;
	struct vertex* current;

	for (i = 0; i < m_n; i++) {
		m_varray[i]->done = 0;
		m_varray[i]->dist = MAX_DISTANCE;
		m_varray[i]->from = NULL;
	}
	m_varray[0]->dist = 0;
	m_varray[0]->done = 1;
	current = m_varray[0];
	while (n != m_n) {
		for (i = 0; i < current->numEdge; i++) {
			if (((current->edges[i]->end->done) == 0) && (current->edges[i]->end->dist) >= ((current->edges[i]->weight) + (current->dist))) {
				current->edges[i]->end->dist = current->edges[i]->weight + current->dist;
				current->edges[i]->end->from = current;
			}
		}
		for (i = 0; i < m_n; i++) {
			if (((m_varray[i]->done) == 0) && ((m_varray[i]->dist) < minDist)) {
				minDist = m_varray[i]->dist;
				current = m_varray[i];
			}
		}
		minDist = MAX_DISTANCE;
		current->done = 1;
		n++;
	}
}


int read_input_file(char *filename) {
    FILE *fp;
    char linebuf[1000];
	char *tabbuf;	// for strtok
	int n;	// check how many edges from this vertex
	int m;
	int i;	// variable which use spliting and saving edge(strtok)
	int v;

    // clear global data before reading the file
    m_n = 0;

    // adjacency list representation of a simple undirected graph
    if ((fp = fopen(filename, "r")) == NULL) {
        perror("Input File Error.\n");
        return -1;
    }

    // read data file, line by line
	while (fgets(linebuf, sizeof(linebuf), fp) != NULL) {	// read and calloc vertices first
		if (m_n > MAX_NUM_NODES) {
			printf("too many data (n = %d)\n", m_n);
			break;
		}

		tabbuf = strtok(linebuf, "\t");
		if (find_vertex(atoi(tabbuf)) != NULL) {
			strcpy(linebuf, "");
			continue;
		}
		m_varray[m_n] = (struct vertex*)calloc(1, sizeof(struct vertex));
		m_varray[m_n]->id = atoi(tabbuf);
		m_varray[m_n]->dist = MAX_DISTANCE;
		strcpy(linebuf, "");
		m_n++;
	}
	rewind(fp);
	v = 0;
    while (fgets(linebuf, sizeof(linebuf), fp) != NULL) {	// read one more time and calloc edges
		n = 0;
		m = 0;
		
		if (linebuf[strlen(linebuf) - 1] == '\n' || linebuf[strlen(linebuf) - 1] == '\r') {	// delete enter
			linebuf[strlen(linebuf) - 1] = '\0';
			if (linebuf[strlen(linebuf) - 1] == '\r') {
				linebuf[strlen(linebuf) - 1] = '\0';
			}
		}
		if (strcmp(linebuf, "\0") == 0) {
			break;
		}
		for (i = 0; i < strlen(linebuf); i++) {
			if (linebuf[i] == ',') {
				n++;
			}
		}

		i = 0;
		m_varray[v]->numEdge = n;
		m_varray[v]->edges = (struct edge**)calloc(n, sizeof(struct edge*));

		tabbuf = strtok(linebuf, "\t");
		while (tabbuf != NULL) {
			if (i == 0) {	// first thing is vertex's id
				tabbuf = strtok(NULL, ",");
				i = 1;
			}
			else if (i == 1) {	// other things are edge's elements (end vertex, weight)
				if (find_edge(m_varray[v]->id, atoi(tabbuf)) != NULL) {
					continue;
				}
				m_varray[v]->edges[m] = (struct edge*)calloc(1, sizeof(struct edge));
				m_varray[v]->edges[m]->start = m_varray[v];
				m_varray[v]->edges[m]->end = find_vertex(atoi(tabbuf));
				tabbuf = strtok(NULL, "\t");
				i = 2;
			}
			else {
				m_varray[v]->edges[m]->weight = atoi(tabbuf);
				tabbuf = strtok(NULL, ",");
				m++;
				m_e++;
				i = 1;
			}
		}
		v++;
		strcpy(linebuf, "");
    }
    fclose(fp);

    printf("%s: total: %d nodes, %d edges\n", __func__, m_n, m_e/2); 

    return 0;
}


int main (int argc, char *argv[]) {
    char *filename = NULL;
	clock_t start, stop;
	unsigned long sec, msec;

    /* read input file from filename */
	filename = argv[1];
	start = clock();
    read_input_file(filename); 

    /* run dijkstra here */
    run_dijkstra();

    /* print shortest distance of all nodes in increasing order of ID */    
    print_all_shortest_distance();

    /* if possible, print shortest path of all nodes */
    if (1) {
        print_all_shortest_path();
    }

    printf("homework output:\n");
    printf("shortest distance to vertices 7,37,59,82,99,115,133,165,188,197\n");
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            m_varray[6]->dist, m_varray[36]->dist,
            m_varray[58]->dist, m_varray[81]->dist,
            m_varray[98]->dist, m_varray[114]->dist,
            m_varray[132]->dist, m_varray[164]->dist,
            m_varray[187]->dist, m_varray[196]->dist);

    /* memory clean up */
    delete_graph();
	stop = clock();


    /* calculate running time */
	sec = (unsigned long)(stop - start) / CLOCKS_PER_SEC;
	msec = (unsigned long)(stop - start) * 1000 / CLOCKS_PER_SEC;
    printf("running time = %lu.%03lu seconds\n", sec, msec);

    return 0;
}


