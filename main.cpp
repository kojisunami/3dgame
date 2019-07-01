#include "main.h"


void Point::trans(float x_diff, float y_diff,float z_diff){
	x += x_diff;
	y += y_diff;
	z += z_diff;
}

void Point::rot_xy( float phi){
	float tmp0 = x * cos(phi) - y * sin(phi);
	float tmp1 = x * sin(phi) + y * cos(phi);
	x = tmp0;
	y = tmp1;
}

void Point::rot_xz( float theta){
	float tmp0 = x * cos(theta) - z * sin(theta);
	float tmp1 = z * sin(theta) + z * cos(theta);
	x = tmp0;
	z = tmp1;
}

void Point::perspective(){
		// we add the perspective
		// we measure the distance.
		float p,q,r;
		p = (look_at.x -look_from.x)/
			sqrt( pow(look_at.x-look_from.x,2)+ pow(look_at.y-look_from.y,2)) ;
		q = (look_at.y -look_from.y)/
			sqrt( pow(look_at.x-look_from.x,2)+ pow(look_at.y-look_from.y,2)) ;
		r = 0;
	
		float tmp;
		tmp = p;
		p = q;
		q =  -tmp;

		float s;
		float a, b,c;
		s =(p * (look_from.x-look_at.x) + 
			q * (look_from.y-look_at.y) +
			r * (look_from.z-look_at.z))/ 
				( pow(look_from.x-look_at.x,2) + 
					pow(look_from.y-look_at.y,2) +
					pow(look_from.z-look_at.z,2));
		a = p - s * (look_from.x-look_at.x);
		b = q - s * (look_from.y-look_at.y);
		c = r - s * (look_from.z-look_at.z);
	
		float t;
		float d,e,f;
		t =((x-look_at.x) * (look_from.x-look_at.x) + 
			(y-look_at.y) * (look_from.y-look_at.y) +
			(z-look_at.z) * (look_from.z-look_at.z))/ 
				( pow(look_from.x-look_at.x,2) + 
					pow(look_from.y-look_at.y,2) +
					pow(look_from.z-look_at.z,2));
		d = (x-look_at.x) - t * (look_from.x-look_at.x);
		e = (y-look_at.y) - t * (look_from.y-look_at.y);
		f = (z-look_at.z) - t * (look_from.z-look_at.z);
		float dist;
		dist = sqrt(( pow(d,2) + pow(e,2) + pow(f,2)));
	
		float cos_arg ; // cos
		cos_arg = (a * d + b *e + c *f)/
			(sqrt(pow(a,2)+pow(b,2)+pow(c,2))*sqrt(pow(d,2)+pow(e,2)+pow(f,2)));


	// calculation of sine is a bit complicated
	// we need to make sure if it is positive or not.
	// calculation of X origin(perpendicular to Y and perspective vector(at-from))
		float g,h,j;
		float tmp_arg;
		g = c*(look_from.y-look_at.y)- b*(look_from.z-look_at.z);
		h = a*(look_from.z-look_at.z)- c*(look_from.x-look_at.x);
		j = b*(look_from.x-look_at.x)- a*(look_from.y-look_at.y);

		tmp_arg = (g * d + h *e + j *f)/
			(sqrt(pow(g,2)+pow(h,2)+pow(j,2))*sqrt(pow(d,2)+pow(e,2)+pow(f,2)));

		float sin_arg;
		if( tmp_arg > 0)
			sin_arg = sqrt(1- pow(cos_arg,2));	
		else
			sin_arg = - sqrt(1- pow(cos_arg,2));	


		if( ((look_from.x - d)/(look_from.x-look_at.x) < 0)||
			((look_from.y - e)/(look_from.y-look_at.y) < 0)||
			((look_from.z - f)/(look_from.z-look_at.z) < 0)){
			// this is outrange
			out_range_flag = 1;
		}else{
			out_range_flag = 0;
		}

		float l;
		l = sqrt(pow(look_from.x-x,2)+ pow(look_from.y-y,2)+ pow(look_from.z-z,2));
		dist = 40 * (dist / sqrt(pow(l,2)-pow(dist,2)));

		u = (int)(dist * cos_arg * cube_size) + cor_u_off;// + cor_x * cube_size;
		v = (int)(dist * sin_arg * cube_size) + cor_v_off;// + cor_x * cube_size;
}


void Grid::update(){
	// xy base grid
	for(int i =0;i<200;i++)
		pt[i]   = Point( (float((i/2)-50)), 48*pow(-1,(i%2)), 0);

	for(int i =0;i<200;i++)
		pt[i+200] = Point( 48*pow(-1,i%2),  (float((i/2)-50)), 0);


	// coordinate information
/*		pt[200] = Point( 0.0-2.0,0.0-2.0,0.0);
		pt[201] = Point( 1.0-2.0,0.0-2.0,0.0);

		pt[202] = Point( 0.0-2.0,0.0-2.0,0.0);
		pt[203] = Point( 0.0-2.0,1.0-2.0,0.0);

		pt[204] = Point( 0.0-2.0,0.0-2.0,0.0);
		pt[205] = Point( 0.0-2.0,0.0-2.0,1.0);
		*/

	//view port to two dimension space;
	for(int i =0;i<400;i++){
		float tmp0, tmp1;
		pt[i].trans(cor_x, cor_y, cor_z);
/*
		pt[i].rot_xy(phi);
		pt[i].rot_xz(theta);
		*/

		pt[i].perspective();

	}
}

void Grid::draw_lines(){

	update();
	glColor3d(0.0,1.0,0.0);
	glBegin(GL_LINES);
	for(int i =0;i<400;i+=2){
		glVertex2i(pt[i].u, pt[i].v);
		glVertex2i(pt[i+1].u, pt[i+1].v);
	}
	/*
	glColor3d(1.0,1.0,1.0);
	for(int i =200;i<206;i+=2){
		glVertex2i(pt[i].u, pt[i].v);
		glVertex2i(pt[i+1].u, pt[i+1].v);
	}*/

	glColor3d(1.0,0.0,0.0);
		glVertex2i(pt[100].u, pt[100].v);
		glVertex2i(pt[101].u, pt[101].v);
		glVertex2i(pt[300].u, pt[300].v);
		glVertex2i(pt[301].u, pt[301].v);
	glColor3d(0.0,1.0,0.0);

	/*
	int len;
	char str[6];
	len = sprintf(str, "x");
	put_str(str, len, 1, pt[201].u, pt[201].v);

	len = sprintf(str, "y");
	put_str(str, len, 1, pt[203].u, pt[203].v);

	len = sprintf(str, "z");
	put_str(str, len, 1, pt[205].u, pt[205].v);

	*/
	glEnd();
}

void Cube::update(){
	// define data points
	for(int i =0;i<8;i++)
		pt[i] = Point( 1.0*(float)((i&0x04)>>2), 1.0*(float)((i&0x02)>>1), 1.0*(float)((i&0x01)>>0));
	//view port to two dimension space;
	for(int i =0;i<8;i++){

		pt[i].trans(cor_x, cor_y, cor_z);
		pt[i].trans(-0.5,-0.5,-0.5);

		//pt[i].rot_xy(phi);
		//pt[i].rot_xz(theta);
		pt[i].perspective();

	}
}

void Cube::draw_surface(int i, int j,int k, int l){
	//glColor3d(0.6,0.6,0.6);

	glColor3d( (color_id+1)/4, ((color_id+1)/2)%2, (color_id+1)%2);

	if(pt[i].out_range_flag == 1)
		return;
	if(pt[j].out_range_flag == 1)
		return;
	if(pt[k].out_range_flag == 1)
		return;
	if(pt[l].out_range_flag == 1)
		return;
	glBegin(GL_POLYGON);
	glVertex2i(pt[i].u, pt[i].v);
	glVertex2i(pt[j].u, pt[j].v);
	glVertex2i(pt[k].u, pt[k].v);
	glVertex2i(pt[l].u, pt[l].v);
	glEnd();
}


void Cube::draw_line(int i, int j){
	if(pt[i].out_range_flag == 1)
		return;
	if(pt[j].out_range_flag == 1)
		return;
	glColor3d(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex2i(pt[i].u, pt[i].v);
	glVertex2i(pt[j].u, pt[j].v);
	glEnd();
}
	
void Cube::draw_lines(){
	update();
	for(int i =0;i<4;i++)
		draw_line(i,i+4);

	draw_line(0,1);	
	draw_line(2,3);	
	draw_line(4,5);	
	draw_line(6,7);	
	draw_line(0,2);	
	draw_line(1,3);	
	draw_line(4,6);	
	draw_line(5,7);	
}

void Cube::draw_surfaces(){
	
	char str[16];
	int len;
	float d;
	for(int i =0;i <8;i++){
		d = sqrt( pow(look_from.x - pt[i].x,2) + 
			pow(look_from.y - pt[i].y,2) + pow(look_from.z - pt[i].z,2));
		len = sprintf(str, "%f", d);
		put_str(str, len, 1, pt[i].u, pt[i].v);
	}

	draw_surface(0,1,3,2);
	draw_surface(4,5,7,6);
	draw_surface(0,1,5,4);
	draw_surface(2,3,7,6);
	draw_surface(1,3,7,5);
	draw_surface(0,2,6,4);
	
}

void maze_list_init(){
	for(int i =0;i<CELL_SIZE_X;i++){
	for(int j =0;j<CELL_SIZE_Y;j++){
		maze_list[i][j] = 0;
		maze_search_list[i][j] = 0;
	}}
}


void Maze::gen(){
	int r;
	int next_x;
	int next_y;
	int count = 0;
	while(1){
	r = rand() % 4;
	switch(r) {
		case 0:
			next_x = x+1; next_y = y+0; break;
		case 1:
			next_x = x+0; next_y = y+1; break;
		case 2:
			next_x = x-1; next_y = y+0; break;
		case 3:
			next_x = x+0; next_y = y-1; break;
	}

		if(count > 4)
			return ;
		else
			count++;

		if(maze_list[next_x][next_y] == 1)
			continue;
		if((next_x < 0) || (next_y < 0))
			continue;
		else
			break;
	}
	dir[r] = 1;
	children.push_back( Maze( next_x, next_y, this, r));
}


void Maze::draw_floors(){
	draw_floor();
	for(int i =0;i<children.size();i++)
		children[i].draw_floor();
}

void Maze::draw_lines(){

	//if (id ==0)
		//printf("0: %d 1: %d 2:%d 3:%d\n", dir[0], dir[1], dir[2], dir[3] );



	// draw the side
	if(dir[0] == 0){ // right
		draw_line( Point( (x+1), (y+0), 0),
				   Point( (x+1), (y+1), 0));
	}
	if(dir[1] == 0){ // up
		draw_line( Point( (x+0), (y+1), 0),
				   Point( (x+1), (y+1), 0));
	}
	if(dir[2] == 0){ //left
		draw_line( Point( (x+0), (y+0), 0),
				   Point( (x+0), (y+1), 0));
	}
	if(dir[3] == 0){ //down
		draw_line( Point( (x+0), (y+0), 0),
				   Point( (x+1), (y+0), 0));
	}


	for(int i =0;i<children.size();i++)
		children[i].draw_lines();
}
void Maze::draw_floor(){
	Point pt[4];
	pt[0].x = (x+0);
	pt[0].y = (y+0);
	pt[1].x = (x+1);
	pt[1].y = (y+0);
	pt[2].x = (x+1);
	pt[2].y = (y+1);
	pt[3].x = (x+0);
	pt[3].y = (y+1);

	for(int i=0;i<4;i++){
		pt[i].z = 0;
		pt[i].rot_xy(phi);
		pt[i].rot_xz(theta);
		pt[i].trans(cor_u_off, cor_v_off,0);
		pt[i].perspective();
	}

	switch(property){
		case 0:
			glColor3d(1.0,1.0,0.0); break;
		case 1:
			glColor3d(1.0,0.0,0.0); break;
		case 2:
			glColor3d(0.0,0.0,1.0); break;
	}

	glBegin(GL_POLYGON);
	for(int i=0;i<4;i++)
		glVertex2i(pt[i].u, pt[i].v);
	glEnd();
}

void Maze::draw_line(Point pt0, Point pt1){

	Point pt[4];
	pt[0] = pt0;
	pt[1] = pt1;
	pt[2] = pt0;
	pt[3] = pt1;

	pt[2].trans(0,0,0.3);
	pt[3].trans(0,0,0.3);

	pt[0].trans(0,0,0);
	pt[1].trans(0,0,0);

	if(pt[0].out_range_flag == 1)
		return;
	if(pt[1].out_range_flag == 1)
		return;
	if(pt[2].out_range_flag == 1)
		return;
	if(pt[3].out_range_flag == 1)
		return;

	for(int i =0;i<4;i++){
		pt[i].perspective();

	}
	glColor3d(0.6,0.6,0.6);
	glBegin(GL_POLYGON);
	glVertex2i(pt[0].u, pt[0].v); // tantatively x and y are transposed....
	glVertex2i(pt[1].u, pt[1].v);

	glVertex2i(pt[1].u, pt[1].v);
	glVertex2i(pt[3].u, pt[3].v);

	glVertex2i(pt[3].u, pt[3].v);
	glVertex2i(pt[2].u, pt[2].v);

	glVertex2i(pt[2].u, pt[2].v);
	glVertex2i(pt[0].u, pt[0].v);
	glEnd();


	glColor3d(0.0,0.0,0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2i(pt[0].u, pt[0].v); // tantatively x and y are transposed....
	glVertex2i(pt[1].u, pt[1].v);
	glVertex2i(pt[3].u, pt[3].v);
	glVertex2i(pt[2].u, pt[2].v);
	glEnd();
}


//  pick any object whose height is i from the root, but if it is not unique, it is randomly taken.
Maze* Maze::ith(int i){
	Maze *ret = this;
	for(int l=0;l<i;l++){
		if(ret->children.size() != 0)
			ret = &(ret->children[rand()%children.size()]);
		else
			return ret;
	}
	return ret;
}


void Line::update(){
	pt0->perspective();
	pt1->perspective();
}

void Line::draw(){

	update();
	pt0->perspective();	
	pt1->perspective();	

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex2i(pt0->u, pt0->v);
	glVertex2i(pt1->u, pt1->v);
	glEnd();
}

class Bullet{
public:
	float x;
	float y;
	float z;
	float v_x, v_y, v_z;

	struct timespec ancient_time;

	Polygon *xml_dat;
	Bullet(){
		float l = sqrt( pow(look_from.x-look_at.x,2) +
			pow(look_from.y-look_at.y,2) + pow(look_from.z-look_at.z,2));
		v_x = 30*(look_at.x -look_from.x)/ l;
		v_y = 30*(look_at.y -look_from.y)/ l;
		v_z = 30*(look_at.z -look_from.z)/ l;
		x = look_from.x;
		y = look_from.y;
		z = look_from.z;

		xml_dat = new Polygon("resources/polygon.xml");

		clock_gettime(CLOCK_REALTIME, &ancient_time);
	}

	void draw();
	void hit_test();
	void update(float t);
};


vector<Bullet> bullet;


void Bullet::hit_test(){
}

void Bullet::update(float t){
	x += v_x * t;
	y += v_y * t;
	z += v_z * t;
}

void Bullet::draw(){
	Point pt(x,y,z);
	xml_dat->draw(pt);
}

void bullet_launch(){
	bullet.push_back(Bullet());
}



void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);

	struct timespec newtime, fps_oldtime;


// showing fps
	static int count =0;
	static int fps;

	if(count == 0)
		clock_gettime(CLOCK_REALTIME, &fps_oldtime);
	clock_gettime(CLOCK_REALTIME, &newtime);

	if( time_gap(newtime, fps_oldtime) > 1){
		fps = count;
		count =0;
		fps_oldtime = newtime;
	}
	count ++;

	for(int i = bullet.size() - 1; i >= 0; i--){
		bullet[i].draw();
	}

	static Maze *current_pt;
	static Maze * last_branch;

/*
	static int start_flag = 0;
	if(start_flag == 0){
		current_pt  = maze;
		last_branch = maze;
		start_flag  = 1;
	}else{

		switch( current_pt->children.size()){
			case 0:{
			current_pt = last_branch;
			if(last_branch->children.size() ==0)
				current_pt = maze;
			else
				current_pt->children.erase(current_pt->children.begin());
			break;
			}
			case 1:
				break;
			default:
				last_branch = current_pt;
				break;
		}
		current_pt = &(current_pt->children[0]);
	}
	*/
	//cube.cor_x = current_pt->x;
	//cube.cor_y = current_pt->y;
	

	Grid grid;
	//grid.draw_lines();



	char str[64];
	int len;

	static int start_flag = 0;
	struct  timespec start_oldtime;
	if(game_mode == GAME_START){
		if(start_flag == 0){
			clock_gettime(CLOCK_REALTIME, &start_oldtime);
			start_flag = 1;	
		}
		len = sprintf(str, "Mission Start");
		put_str(str, len, 4, 230, 440);

		if(time_gap (newtime, start_oldtime) > 3){
			printf("hello\n");
			game_mode = GAME_RUN;
		}

	}if((game_mode ==GAME_RUN) || (game_mode == GAME_START)){

	maze->draw_floors();
	maze->draw_lines();

	Point kirby_pt =  look_from;
	float l = 
		sqrt((look_from.x - look_at.x,2)+ (look_from.y - look_at.y,2)+ (look_from.z - look_at.z,2));
	kirby_pt.x = look_from.x + (look_from.x -look_at.x)/l;
	kirby_pt.y = look_from.y + (look_from.y -look_at.y)/l;
	kirby_pt.z = look_from.z + (look_from.z -look_at.z)/l;

	//poly.draw( kirby_pt);
	poly1.draw();
	poly2.draw();



	len = sprintf(str, "Copyright(C) 2019 Koji Sunami");
	put_str(str, len, 1, 230, 40);

	glColor3d(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex2i(700,0);
	glVertex2i(700,1000);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i( cor_u_off-30, cor_v_off+30);
	glVertex2i( cor_u_off+30, cor_v_off-30);
	glVertex2i( cor_u_off-30, cor_v_off-30);
	glVertex2i( cor_u_off+30, cor_v_off+30);
	glEnd();

	len = sprintf(str, "%dfps", fps);
	put_str(str, len, 2, 740, 200);

	len = sprintf(str, "Score: %d\nTime: %d", score, sys_time);
	put_str(str, len, 2, 740, 300);

	len = sprintf(str, "x: %f\ny: %f\nz:%f", look_from.x, look_from.y, look_from.z);
	put_str(str, len, 2, 740, 600);

/*
	len = sprintf(str, "x: %f\ny: %f\nz:%f", look_at.x, look_at.y, look_at.z);
	put_str(str, len, 2, 740, 300);
	*/

	len = sprintf(str, "%dkm/h", (int)round(v*1200));
	put_str(str, len, 2, 740, 800);

	len = sprintf(str, "alt %dm", (int)round(look_from.z*1000/3));
	put_str(str, len, 2, 740, 700);

		//if( v *1200 < 600){
		if( v < .5){
			len = sprintf(str, "  Warning:\n \nFalling Down\n  Speed Up");
			put_str(str, len, 5, 200, 400);
			
		}
	}

	glutSwapBuffers();
	glFlush();
}


void maze_init() {
	maze_list_init();

	// continue until first one has at least length 5
	while(1){
		maze = new Maze();
		if(maze->ith(5)->id == 5)
			break;
	}

	Maze *current_pt;
	
	static int flag = 0;
	int max_id=0;

	while(1){

	if( flag == 0){
		current_pt = maze;
	}else{
		if(current_pt->children.size() == 0){
			static int c=0;
			max_id= max(current_pt->id, max_id);
			int t =  rand()%max_id;
hoge:

			current_pt = maze->ith(t);
		// add branch
		if( (current_pt->x < CELL_SIZE_X) && maze_list[current_pt->x+1][current_pt->y] == 0){
			current_pt->children.push_back( Maze(current_pt->x+1, current_pt->y,current_pt, 0));
			maze_list[current_pt->x+1][current_pt->y] = 1;
		}
		else if( ( current_pt->y < CELL_SIZE_Y) && (maze_list[current_pt->x][current_pt->y+1] == 0)){
			current_pt->children.push_back( Maze(current_pt->x, current_pt->y+1,current_pt, 1));
			maze_list[current_pt->x][current_pt->y+1] = 1;
		}	
		else if( (current_pt->x > 0) &&( maze_list[current_pt->x-1][current_pt->y] == 0)){
			current_pt->children.push_back( Maze(current_pt->x-1, current_pt->y,current_pt, 2));
			maze_list[current_pt->x-1][current_pt->y] = 1;
		}
		else if( (current_pt->y > 0) && (maze_list[current_pt->x][current_pt->y-1] == 0)){
			current_pt->children.push_back( Maze(current_pt->x, current_pt->y-1,current_pt, 3));
			maze_list[current_pt->x][current_pt->y-1] = 1;
		}
		else{
			//printf("hello %d\n",c);
			if(c > 10)
				break;
			//return;
			c++;
			t ++;
			goto hoge;
		}

		}
			if(current_pt->children.size() != 0)
				current_pt = &(current_pt->children[rand()%current_pt->children.size()]);
			else
				printf("this is end of the world\n");
			//current_pt = &(current_pt->children[0]);


			//maze_search_list[current_pt->x][current_pt->y] = 1;
	}
	flag++;
	//printf("flag is %d\n", flag);
	if(flag > 1000)
		break;
	}
}

void perspective_init(){
	look_from.x = -65.0;
	look_from.y = -65.0;
	look_from.z = 2.0;

	look_at.x = -45.0;
	look_at.y = -45.0;
	look_at.z = 0.0;
}


int Polygon::load(const char filename[]){
	polygon_xml = new Xml_Tree(filename);

	int pt_count=0;
	for(int j =0;j<polygon_xml->children.size();j++){
		// get vertecies
		if(str_match(polygon_xml->children[j].xml_list[0].arg[0], (char*)"vertex", 6) == 1){

			int id = atoi(polygon_xml->get_data_by_id((char*)"vertex", pt_count, (char*)"id=" ) );
			if(id != -1){
				float x = 
					atof(polygon_xml->get_data_by_id( (char*)"vertex", pt_count, (char*)"x=") );
				float y = 
					atof(polygon_xml->get_data_by_id( (char*)"vertex", pt_count, (char*)"y=") );
				float z = 
				atof(polygon_xml->get_data_by_id( (char*)"vertex", pt_count, (char*)"z=") );

				pp.push_back(Point(id, x,y,z)); // actually float
				pt_count++;
			}
		}
	}

	int line_count=0;
	for(int j =0;j<polygon_xml->children.size();j++){
		// get lines
		if(str_match(polygon_xml->children[j].xml_list[0].arg[0], (char*)"line", 4) == 1){
			int id = atoi(polygon_xml->get_data_by_id((char*)"line", line_count, (char*)"id=" ) );
			if(id != -1){
				int x = 
					atoi(polygon_xml->get_data_by_id( (char*)"line", line_count, (char*)"x=") );
				int y = 
					atoi(polygon_xml->get_data_by_id( (char*)"line", line_count, (char*)"y=") );
				line.push_back( Line( id,  &pp[x], &pp[y]));
				line_count++;
			}
		}
	}
	return 0;
}

float Polygon::dist(){
	float sum=0;
	for(int i =0;i<pp.size();i++)
		sum += sqrt( pow(pp[i].x-look_from.x,2) + 
			pow(pp[i].y-look_from.y,2) + pow(pp[i].z-look_from.z,2));
	return (sum / pp.size());
}

Point Polygon::center(){
	Point pt;
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;
	for(int i =0;i<pp.size();i++){
		sum_x += pp[i].x;
		sum_y += pp[i].y;
		sum_z += pp[i].z;
	}
	pt.x = sum_x/pp.size();
	pt.y = sum_y/pp.size();
	pt.z = sum_z/pp.size();
	pt.perspective();
	return pt;
}

void Polygon::draw( Point org_pt){
	
	Point pt = center();
	char buf[64];
	int len ;
	float d = dist();
	len = sprintf(buf, "%fkm\n",d/3);
	put_str(buf, len, 1, pt.u, pt.v);

	
	for(int i =0;i<pp.size();i++)
		pp[i].trans(org_pt.x, org_pt.y, org_pt.z);
	for(int i =0;i<line.size();i++){
		line[i].draw();
	}
	for(int i =0;i<pp.size();i++)
		pp[i].trans(-org_pt.x, -org_pt.y, -org_pt.z);
}


void Polygon::draw(){
	
	Point pt = center();
	char buf[64];
	int len ;
	float d = dist();
	len = sprintf(buf, "%fkm\n",d/3);
	put_str(buf, len, 1, pt.u, pt.v);

	for(int i =0;i<line.size();i++)
		line[i].draw();

		float resize = 1000/d;
	if( d < 30){
		glColor3d(1.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(pt.u-resize,pt.v-resize);
		glVertex2i(pt.u-resize,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v-resize);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2i(pt.u       ,pt.v-resize);
		glVertex2i(pt.u-resize,pt.v);
		glVertex2i(pt.u       ,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v);
		glEnd();
	}else if( d < 50){
		glColor3d(0.0,1.0,0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(pt.u-resize,pt.v-resize);
		glVertex2i(pt.u-resize,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v-resize);
		glEnd();
	}else if( d < 100){
		glColor3d(1.0,1.0,1.0);
		glBegin(GL_LINE_LOOP);
		glVertex2i(pt.u-resize,pt.v-resize);
		glVertex2i(pt.u-resize,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v+resize);
		glVertex2i(pt.u+resize,pt.v-resize);
		glEnd();
	}else{
		glColor3d(0.0,1.0,0.0);
		glBegin(GL_POINTS);
		for(int i =0;i<2;i++){
		for(int j =0;j<2;j++){
		glVertex2i(pt.u+10+i,pt.v+0+j);
		glVertex2i(pt.u+3 +i,pt.v+6+j);
		glVertex2i(pt.u-3 +i,pt.v+6+j);
		glVertex2i(pt.u-3 +i,pt.v-6+j);
		glVertex2i(pt.u+3 +i,pt.v-6+j);
		glVertex2i(pt.u-10+i,pt.v+0+j);
		}}
		glEnd();
	}
}

void init(){
	maze_init();

	perspective_init();

	poly.load("resources/any.xml");
	poly1.load("resources/polygon.xml");

	// create a donut, and classify in pixel
	//for(int i=0;i<polygon_xml->children.size();i++)
//		polygon_xml->children[i].show();


	//for(int i =0;i<polygon_xml->children[0].xml_list[0].num_arg;i++){
	//}

/*
	int arr[100][100][100];

	for(int i =0;i<100;i++){
	for(int j =0;j<100;j++){
	for(int k =0;k<100;k++){
		arr[i][j][k] = i;;
	}}}

	for(int i =0;i<30;i++){
		cube_list.push_back(Cube(0.0, 10*cos(2*M_PI*i/30), 10*sin(2*M_PI*i/30)));
	}

	*/


	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
}



void* gl_loop_thread(void* threadid){
	glutMainLoop();
	return NULL;
}

void* length_calc_thread (void *threadid){
	while(1){
		d = sqrt(pow(look_at.x - look_from.x, 2) + pow(look_at.y - look_from.y ,2));
		usleep(20000);

	static int c = 0;
		float angle = M_PI*(c%20)/30 - 5*M_PI/6;
		if((c/20)%2 == 1){
			poly.pp[20].y = .3 * cos(angle);
			poly.pp[20].z = .3 * sin(angle) - .3;
			poly.pp[25].y = .3 * cos(angle);
			poly.pp[25].z = .3 * sin(angle) - .3;
		}else{
			poly.pp[20].y = -.3 * cos(angle);
			poly.pp[20].z =  .3 * sin(angle) - .3;
			poly.pp[25].y = -.3 * cos(angle);
			poly.pp[25].z =  .3 * sin(angle) - .3;
		}
		c++;
	}
	return NULL;
}

void* time_watch_thread(void *threadid){
	struct timespec oldtime, newtime, middletime;

	while(1){ 

		clock_gettime(CLOCK_REALTIME, &newtime);
		
		static int first_time_flag = 0;
		if ( first_time_flag == 0){
			clock_gettime(CLOCK_REALTIME, &middletime);	
			clock_gettime(CLOCK_REALTIME, &oldtime);	
			first_time_flag ++;
		}
	
		float t = time_gap(newtime,middletime);
		middletime = newtime;
	
	
		float x = look_at.x - look_from.x;
		float y = look_at.y - look_from.y;
		float diff = (1/d) * t * v;
		float dx = x * diff;
		float dy = y * diff;
	
		if( lift_flag == 1)
			look_from.z += t;
		else if( lift_flag == -1)
			look_from.z -= t;
	
		/*
		if( proceed_flag == 1 ){
			look_from.x += dx;
			look_from.y += dy;
			look_at.x += dx;
			look_at.y += dy;
		}
			*/
		if(proceed_flag ==1)
			v+=t*0.3;
		else
			v-=t*0.1;
		look_from.x += dx;
		look_from.y += dy;
		look_at.x += dx;
		look_at.y += dy;
	
		if( rotation_flag_theta == 2){
			look_at.x += t * y;
			look_at.y -= t * x;
		}
		else if( rotation_flag_theta == -2){
			look_at.x -= t * y;
			look_at.y += t * x;
		}
		else if( rotation_flag_theta == 1){
			look_at.x += t * y/32;
			look_at.y -= t * x/32;
		}
		else if( rotation_flag_theta == -1){
			look_at.x -= t * y/32;
			look_at.y += t * x/32;
		}

		if( rotation_flag_phi == 1){
			look_at.x += x * t;
			look_at.y += y * t;
		}
		else if( rotation_flag_phi == -1){
			look_at.x -= x * t;
			look_at.y -= y * t;
		}
		
		for(int i =0;i<bullet.size();i++)
			bullet[i].update(t);

		for(int i = bullet.size() - 1; i>=0;i--)
			if( time_gap(newtime, bullet[i].ancient_time) > 2)
				bullet.erase(bullet.begin()+i);

		usleep(20);
	}

	return NULL;
}

void* music_thread(void *threadid){
	while(1){
		//music_play("resources/tetris.wav");
		sleep(1);
	}
	return NULL;
}


void keyboard( unsigned char key, int x, int y){

	switch(key){
		case 27:	
			exit(0);
		case 'r':{
			lift_flag = -1; break;
		}
		case 't':{
			lift_flag = 1; break;
		}
		case 'f':{
			proceed_flag = 1; break;
		case ' ':{
			bullet_launch();
			break;
		}
		case 'j':{
			rotation_flag_theta = -1; break;
		}
		case 'l':{
			rotation_flag_theta = 1; break;
		}
		}
	}
}

void keyboard_up( unsigned char key, int x, int y){
	switch(key) {
		case 'r':{
			lift_flag = 0; break;
		}
		case 't':{
			lift_flag = 0; break;
		}
		case 'f':{
			proceed_flag = 0; break;
		}
		case 'j':{
			rotation_flag_theta = 0; break;
		}
		case 'l':{
			rotation_flag_theta = 0; break;
		}
	}
}

void special_key(int key ,int x , int y){


	switch(key){
		case GLUT_KEY_UP:
			rotation_flag_phi = 1; break;

		case GLUT_KEY_DOWN:
			rotation_flag_phi = -1; break;

		case GLUT_KEY_RIGHT:
			rotation_flag_theta = 2; break;

		case GLUT_KEY_LEFT:
			rotation_flag_theta = -2; break;
	}
}

void special_key_up(int key ,int x , int y){
	switch(key){
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
			rotation_flag_phi = 0;
			break;
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_LEFT:
			rotation_flag_theta = 0;
			break;
	}
}


void idle() {
	glutPostRedisplay();
}



int main(int argc, char *argv[]) {

	srand( (unsigned int) time(&t));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowPosition(400,0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);


	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(special_key);
	glutSpecialUpFunc(special_key_up);
	glutIdleFunc(idle);

	init();

	pthread_t threads[3];
	int rc;

	//rc = pthread_create(&threads[0], NULL, gl_loop_thread, (void*) 0);
	rc= pthread_create(&threads[0], NULL, length_calc_thread, (void*) 0);
	rc= pthread_create(&threads[1], NULL, time_watch_thread, (void*) 1);
	rc= pthread_create(&threads[2], NULL, music_thread, (void*) 2);
	if(rc){
		printf("failed to launch pthread\n");
	}

	glutMainLoop();

	return 0;
}



