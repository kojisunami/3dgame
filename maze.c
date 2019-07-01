
#define CELL_SIZE_X 40 
#define CELL_SIZE_Y CELL_SIZE_X

void maze_gen();


int maze_list[CELL_SIZE_X][CELL_SIZE_Y];

void maze_list_init(){
	for(int i =0;i<CELL_SIZE_X;i++)
	for(int j =0;j<CELL_SIZE_Y;j++)
		maze_list[i][j] = 0;
}

class Maze{
public:
	int x , y, z;

	int id;

	vector<Maze> children;

	Maze(int src_x, int src_y, int src_id){
		x = src_x;
		y = src_y;
		z = src_z;
		maze_list[x][y] = 1;
		id = src_id + 1 ;
	}
	Maze(){
		x=0;
		y= 0;
		id =0;
	}

	int connect_cell();
	int gen();

	void draw();
};


void Maze::gen(){
	int r = rand() % 4;
	next_x = x + (int)(2*((r%2)-0.5));
	next_y = y + (int)(2*((r/2)-0.5));
	children.push_back( Maze( next_x, next_y));
}


void Maze::draw(){

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
	glVertex2i((x+0)*CELL_SIZE_X, (y+0)*CELL_SIZE_Y);
	glVertex2i((x+0)*CELL_SIZE_X, (y+1)*CELL_SIZE_Y);
	glVertex2i((x+1)*CELL_SIZE_X, (y+1)*CELL_SIZE_Y);
	glVertex2i((x+1)*CELL_SIZE_X, (y+0)*CELL_SIZE_Y);
	glEnd();
	/*
	glBegin(GL_LINES);
//up
	if(up ==NULL){
		glVertex2i((x+0)*CELL_SIZE_X, (y+1)*CELL_SIZE_Y);
		glVertex2i((x+1)*CELL_SIZE_X, (y+1)*CELL_SIZE_Y);
	}else
		up.draw();
//down
	if(down == NULL){
		glVertex2i((x+0)*CELL_SIZE_X, (y+0)*CELL_SIZE_Y);
		glVertex2i((x+1)*CELL_SIZE_X, (y+0)*CELL_SIZE_Y);
	}else
		down.draw();
//right
	if(right == NULL){
	glVertex2i((x+1)*CELL_SIZE_X, (y+0)*CELL_SIZE_Y);
	glVertex2i((x+1)*CELL_SIZE_X, (y+1)*CELL_SIZE_Y);
	}else
		right.draw();
//left
	if(left == NULL){
		glVertex2i((x+0)*CELL_SIZE_X, (y+0)*CELL_SIZE_Y);
		glVertex2i((x+0)*CELL_SIZE_X, (y+1)*CELL_SIZE_Y);
	}else
		left.draw();
	glEnd();
	*/
}

