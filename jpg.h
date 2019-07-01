#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <pthread.h>

#include <jpeglib.h>    
#include <jerror.h>


class JPG_dat{
public:
	int width, height;
	unsigned char *arr;

	unsigned int texture_id;
	

	int load(const char filename[]);
	void draw(int x_off,int y_off);

	JPG_dat(const char filename[]){
		load(filename);
	}
	JPG_dat(){
	}
	~JPG_dat(){
		free (arr);
	}
	
};






