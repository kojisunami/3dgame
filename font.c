#include "font.h"

#define TEXT_WIDTH   8
#define TEXT_HEIGHT 16



void put_letter(char letter, int size, int x_off, int y_off){

	glColor3d(1.0,1.0,1.0);

	for(int j =0;j<TEXT_HEIGHT;j++){
	for(int i =0;i<TEXT_WIDTH;i++){

		glBegin(GL_POINTS);
		int flag = hankaku[j+letter*16]&(1<<i);
		if( flag != 0){
			for( int loop_x = 0; loop_x<size;loop_x++)
			for( int loop_y = 0; loop_y<size;loop_y++)
				glVertex2i(x_off+-(i-8)*size+loop_x,y_off-(j-8)*size+loop_y);
		}
		glEnd();
	}}
}

void put_str( const char str[] , int len ,int size, int x_off, int y_off){
	int x_pt = 0;
	int y_pt = 0;
	for(int i=0;i< len; i++){
		if( str[i] == '\n'){
			y_pt--;
			x_pt = 0;
			i++;
		}
		put_letter( str[i], size, x_off+x_pt*size*8, y_off+y_pt*size*18);
		x_pt++;
	}
}
