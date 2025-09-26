/* MIT License
   Copyright (c) 2024- Citizen7751
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef char** Mtx;
typedef unsigned int Uint;

typedef unsigned char Bool;
#define true 1
#define false 0

const Uint SAFE_LENGTH   = 3;
const Uint STRETCH_RATIO = 2;     /* stretching to look like a square */
const Uint KILLSWITCH    = 100;
const unsigned char PATH = ' ';
const unsigned char WALL = '8';

void enter()
{
	printf("<enter>");
	while(getchar()!='\n');
}


void allocate_maze(Mtx* mref, Uint s)
{
	*mref = (Mtx)malloc(s * sizeof(char*));
	Uint i;
	for (i = 0; i < s; i++)
		(*mref)[i] = (char*)malloc(s * sizeof(char));
	if (*mref == NULL) {
		perror("Failed to allocate memory!\n");
		enter();
		exit(EXIT_FAILURE);
	}
}


void deallocate_maze(Mtx m, Uint s)
{
	Uint i;
	for (i = 0; i < s; i++)
		free(m[i]);
	free(m);
}


void maze_frame(Mtx m, Uint s)    /* outer walls, entrance & exit */
{
	Uint x, y;
	for (y = 0; y < s; y++)
		for (x = 0; x < s; x++)
			if (x == 0 || x == s - 1 || y == 0 || y == s - 1)
				m[x][y] = WALL;
			else m[x][y] = PATH;
	m[1][0] = m[s-2][s-1] = PATH;
}


void print(Mtx m, Uint s)
{
	Uint x, y, sq;
	for (y = 0; y < s; y++) {
		for (x = 0; x < s; x++)
			for (sq = 0; sq < STRETCH_RATIO; sq++)
				putchar(m[x][y]);
		putchar('\n');
	}
}


int random(int min, int max)    /* min & max are included in the boundries */
{
	return rand() % (max - min + 1) + min;
}


/* --------------------------- Recursive Division --------------------------- */


/* puts a hole into the wall */
void make_path(Mtx m, Uint start, Uint length, Uint wall_i, Bool v)
{
	int path_i = random(start, start+length-1);
	(v ? (m[wall_i][path_i] = PATH) : (m[path_i][wall_i] = PATH));
}


void Recdiv(Mtx m, Uint start_x, Uint end_x, Uint start_y, Uint end_y, Bool v)
{
	Uint hlength, vlength;
	Uint wall_i, sbound, ebound, tries = 0;
	Uint x, y;
	Uint start, length;
	Uint nex_lu, ney_lu, nsx_rd, nsy_rd;
	Bool nv;

/* exits, if area is too small */
	hlength = end_x - start_x;
	vlength = end_y - start_y;
	if (hlength <= SAFE_LENGTH || vlength <= SAFE_LENGTH) return;


/* looks for a place for the wall to be put
   if can't find and stucks in a infinite loop,
   the killswitch returns it after 100 trials
*/
	do { /* checking wall boundries -> wall can only come between other walls */
		wall_i = v ? random(start_x+2, end_x-2) : random(start_y+2, end_y-2);
		sbound = v ? m[wall_i][start_y] : m[start_x][wall_i];
		ebound = v ? m[wall_i][end_y] : m[end_x][wall_i];
		tries++;
	} while ((sbound != WALL || ebound != WALL) && tries < KILLSWITCH);

	if (tries == KILLSWITCH) return;


 /* draws the wall */
	for (y = start_y; y <= end_y; y++)
		for (x = start_x; x <= end_x; x++)
			v ? (m[wall_i][y] = WALL) : (m[x][wall_i] = WALL);


/* makes a passage in that wall */
	start = v ? start_y : start_x;
	length = v ? vlength : hlength;
	make_path(m, start+1, length-1, wall_i, v);


/* calculating the divided areas and recursively going to both directions */
	nv = hlength >= vlength;		/* new vertical from the current area */
	nex_lu = v ? wall_i : end_x;	/* new end x - left, up */
	ney_lu = v ? end_y : wall_i;	/* new end y - left, up */
	nsx_rd = v ? wall_i : start_x;	/* new start x - right, down */
	nsy_rd = v ? start_y : wall_i;	/* new start y - right, down */
	Recdiv(m, start_x, nex_lu, start_y, ney_lu, nv);
	Recdiv(m, nsx_rd, end_x, nsy_rd, end_y, nv);
}

/* -------------------------------------------------------------------------- */


int main(void)
{
	Mtx Maze = NULL;
	Uint s = 20;

	srand(time(NULL));

	allocate_maze(&Maze, s);
	maze_frame(Maze, s);
	Recdiv(Maze, 0, s-1, 0, s-1, random(0, 1));	  /* indexing: 0..s-1 */
	print(Maze, s);
	deallocate_maze(Maze, s);
    
	enter();
	return EXIT_SUCCESS;
}
