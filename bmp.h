#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <fcntl.h>
#include <sys/stat.h>

class BMP_dat{
public:
	// meta data
	int width, height;
	int file_size;
	int dat_offset;
	int header_info_size;
	int bit_per_pixel;
	int comp_format;
	
	// pixel data
	unsigned char *arr;

	int read_bmp(const char filename[]);
	void draw_bmp( int x_off, int y_off );

	BMP_dat(const char filename[]){
		read_bmp(filename);		
	}
	/*~BMP_dat(){
		delete arr;
	}*/

};









