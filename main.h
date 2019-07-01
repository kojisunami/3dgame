#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#include <vector>

#include "bmp.h"
#include "jpg.h"
#include "font.h"
#include "audio.h"
#include "xml.h"

#define WIDTH_SIZE  10
#define HEIGHT_SIZE 20

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000


#define GAME_TOP    1
#define GAME_INIT   2
#define GAME_START  3
#define GAME_END    4
#define GAME_RUN  100
#define GAME_MENU 101
#define GAME_TALK 102


using namespace std;

int game_mode = GAME_START;


float phi = M_PI*0.60, theta = -M_PI*0.75;
//int cor_u_off = 0;
//int cor_v_off = WINDOW_HEIGHT/2;
int cor_u_off = WINDOW_WIDTH/2;
int cor_v_off = WINDOW_HEIGHT/6;
int cube_size;


int score = 0;
int sys_time = 0;
int level = 1;
int num_match_total=0;

time_t t;



void keyboard( unsigned char key, int x, int y);
void special_key(int key ,int x , int y);
void special_key_up(int key ,int x , int y);

void display(void);
void idle();
void init();

void maze_init();

void* gl_loop_thread(void* threadid);
void* time_watch_thread(void *threadid);
void* music_thread(void *threadid);




struct Point{
	int id;
	float x , y, z; // actual coordinate
	int u,v ; // correspond position in display

	int out_range_flag ;

	Point(float src_x, float src_y, float src_z){
		x = src_x;
		y = src_y;
		z = src_z;

		out_range_flag = 0;
	}
	Point(int src_id, float src_x, float src_y, float src_z){
		id = src_id;
		x = src_x;
		y = src_y;
		z = src_z;

		out_range_flag = 0;
	}
	Point(){
		x = 0;
		y = 0;
		z = 0;
		out_range_flag = 0;
	}

	void trans(float x_diff, float y_diff,float z_diff);
	void rot_xy( float phi);
	void rot_xz( float theta);

	void perspective();
};

class Line{
public:
	Point *pt0;
	Point *pt1;
	int id;
	int x ,y;

	Line(int src_id, Point *src_pt0, Point *src_pt1){
		id = src_id;
		pt0 = src_pt0;
		pt1 = src_pt1;
	}
	Line(Point *src_pt0, Point *src_pt1){
		pt0 = src_pt0;
		pt1 = src_pt1;
	}
	Line(){
	}

	void update();
	void draw();
};

class Grid{
public:

	int x , y ,z;
	int cor_x, cor_y, cor_z;

	Point pt[406];

	Grid(int src_x , int src_y){
		cor_x = src_x;
		cor_y = src_y;
		cor_z = 0;

		update();
	}
	Grid(){
		cor_x = 0;
		cor_y = 0;
		cor_z = 0;
		update();
	}
	void draw_line(int i, int j);
	void draw_lines();
	void update();
};

class Cube{
private:
	void draw_line(int i , int j);
	void draw_surface(int i, int j, int k, int l);
	void update();
public:
	Point pt[8];

	float v_x, v_y;

	int color_id;
	float cor_x, cor_y, cor_z;

	void draw_lines();
	void draw_surfaces();

	Cube(){
		cube_size =40;
		update();
		v_x = 0;
		v_y = 0;
	}

	Cube(int src_x, int src_y, int src_z){
		cube_size =40;
		cor_x = src_x;
		cor_y = src_y;
		cor_z = src_z;
		v_x = 0;
		v_y = 0;

		update();
	}
};
	



#define CELL_SIZE_X 40 
#define CELL_SIZE_Y CELL_SIZE_X

int maze_list[CELL_SIZE_X][CELL_SIZE_Y];
int maze_search_list[CELL_SIZE_X][CELL_SIZE_Y];

void maze_list_init();

void perspective_init();

class Maze{
public:
	int x, y;

	int id;

	int property; // 0 normal, 1 start , 2 end
	int dir[4];

	Maze *parent;

	vector<Maze> children;

	Maze(int src_x, int src_y, Maze *src_parent, int parent_dir){
		x = src_x;
		y = src_y;
		parent = src_parent;

		for(int i=0;i<4;i++)
			dir[i] = 0;
		dir[(parent_dir+2)%4] = 1;

		maze_list[x][y] = 1;
		id = parent->id + 1;
		property = 0;

		if(id < 40*40)
			gen();
	}
	Maze(){
		x=0;
		y= 0;
		id =0;
		maze_list[x][y] = 1;
		for(int i=0;i<4;i++)
			dir[i] =0;
		property = 1;
		gen();
	}

	void gen();

	void draw();
	void draw_lines();
	void draw_floors();

	void draw_floor();
	void draw_line(Point pt0, Point pt1);

	Maze* ith(int i);
};

class Polygon{
public:

	vector<Point> pp;
	vector<Line> line;
	Xml_Tree *polygon_xml;

	Polygon(const char filename[]){
		load(filename);
	}

	Polygon(){
	}

	int load(const char filename[]);

	float dist();
	Point center();

	void draw();
	void draw(Point org_pt);
};


int num_cube = 0;
Cube cube;
Maze *maze;
vector<Cube> cube_list;

Xml_Tree *polygon_xml;


Point look_from;
Point look_at;

int proceed_flag = 0;
int rotation_flag_phi = 0;
int rotation_flag_theta = 0;
int lift_flag = 0;

float len;
float d;
float h;
float v=3; // real velocity is : v*1200km/h

Polygon poly;
Polygon poly1;
Polygon poly2;



#define time_gap( t1, t0) ( \
	(float)(t1.tv_sec - t0.tv_sec) + (float)(t1.tv_nsec - t0.tv_nsec)/1000000000 ) 
