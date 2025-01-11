//MIT License
//Copyright (c) 2024 Citizen7751

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAFE_LENGTH	3
#define STRETCH_RATIO	2       //stretching to look like a square
#define KILLSWITCH	100
#define PATH		' '
#define WALL		'8'

typedef char** mtx;
typedef unsigned int uint;


void enter() {
	printf("<enter>");
	while(getchar()!='\n');
}

void allocate_maze(mtx* mref, uint s) {
	*mref = (mtx)malloc(s * sizeof(char*));
	for (uint i = 0; i < s; i++)
		(*mref)[i] = (char*)malloc(s * sizeof(char));
	if (*mref == NULL) {
		perror("Failed to allocate memory!\n");
		enter();
		exit(1);
	}
}

void deallocate_maze(mtx m, uint s) {
	for (uint i = 0; i < s; i++)
		free(m[i]);
	free(m);
}

void maze_frame(mtx m, uint s) {		//outer walls, entrance & exit
	for (uint y = 0; y < s; y++)
		for (uint x = 0; x < s; x++)
			if (x == 0 || x == s - 1 || y == 0 || y == s - 1)
				m[x][y] = WALL;
			else m[x][y] = PATH;
	m[1][0] = m[s-2][s-1] = PATH;
}

void print(mtx m, uint s) {
	for (uint y = 0; y < s; y++) {
		for (uint x = 0; x < s; x++)
			for (uint sq = 0; sq < STRETCH_RATIO; sq++)
				putchar(m[x][y]);
		putchar('\n');
	}
}

int random(int min, int max) {			//min & max included in the boundries
	return rand() % (max - min + 1) + min;
}

//------------------------------------------- Recursive Division ------------------------------------------

void make_path(mtx m, uint start, uint length, uint wall_i, int v) {	// puts a hole into the wall
	int path_i = random(start, start+length-1);
	(v ? (m[wall_i][path_i] = PATH) : (m[path_i][wall_i] = PATH));
}


void RecDiv(mtx m, uint start_x, uint end_x, uint start_y, uint end_y, int v) {
	
// exits, if area is too small
	uint hlength = end_x - start_x;
	uint vlength = end_y - start_y;
	if (hlength <= SAFE_LENGTH || vlength <= SAFE_LENGTH) return;


// looks for a place for the wall to be put
// if can't find and stucks in a infinite loop, the killswitch returns it after 100 trials
	uint wall_i, sbound, ebound, tries = 0;
	do {
		wall_i = v ? random(start_x+2, end_x-2) : random(start_y+2, end_y-2);
		sbound = v ? m[wall_i][start_y] : m[start_x][wall_i];		// checking wall boundries ->
		ebound = v ? m[wall_i][end_y] : m[end_x][wall_i];		// wall can only come between other walls
		tries++;
	} while ((sbound != WALL || ebound != WALL) && tries < KILLSWITCH);

	if (tries == KILLSWITCH) return;


// draws the wall
	for (uint y = start_y; y <= end_y; y++)
		for (uint x = start_x; x <= end_x; x++)
			v ? (m[wall_i][y] = WALL) : (m[x][wall_i] = WALL);


// makes a passage in that wall
	uint start = v ? start_y : start_x;
	uint length = v ? vlength : hlength;
	make_path(m, start+1, length-1, wall_i, v);


// calculating the divided areas and recursively going to both directions
	int nv = hlength >= vlength;		// new vertical from the current area
	uint nex_lu = v ? wall_i : end_x;	// new end x - left, up
	uint ney_lu = v ? end_y : wall_i;	// new end y - left, up
	uint nsx_rd = v ? wall_i : start_x;	// new start x - right, down
	uint nsy_rd = v ? start_y : wall_i;	// new start y - right, down
	RecDiv(m, start_x, nex_lu, start_y, ney_lu, nv);
	RecDiv(m, nsx_rd, end_x, nsy_rd, end_y, nv);
}

//---------------------------------------------------------------------------------------------------------


int main(void) {
	srand(time(NULL));
	mtx Maze = NULL;
	uint s = 20;

	allocate_maze(&Maze, s);
	maze_frame(Maze, s);
	RecDiv(Maze, 0, s-1, 0, s-1, random(0, 1));		//indexing: 0..s-1
	print(Maze, s);
	deallocate_maze(Maze, s);
    
	enter();
	return EXIT_SUCCESS;
}
