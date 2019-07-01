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

#include "bmp.h"



int BMP_dat::read_bmp(const char filename[]){

	int fd = open(filename ,O_RDONLY);
	if(!fd){
		printf("could not find basil.bmp\n");
	return 0;
	}
	
	unsigned char buf[256];
	ssize_t len;
	len = read( fd, buf, 256);

	printf("The file header is: %c %c\n", buf[0], buf[1]);

	file_size  = (buf[2]<<0) + (buf[3]<<8) + (buf[4]<<16) + (buf[5]<<24);
	printf("file size is 0x%x\n", file_size);

	dat_offset = (buf[0xa]<<0) + (buf[0xb]<<8) + (buf[0xc]<<16) + (buf[0xd]<<24);
	printf("data offset is 0x%x\n", dat_offset);

	header_info_size = (buf[0xe]<<0) + (buf[0xf]<<8) + (buf[0x10]<<16) + (buf[0x11]<<24);
	printf(" header info size is %d \n", header_info_size);

	if( header_info_size != 124){
		return 0;
	}

	width  = (buf[0x12]<<0) + (buf[0x13]<<8) + (buf[0x14]<<16) + (buf[0x15]<<24);
	height = (buf[0x16]<<0) + (buf[0x17]<<8) + (buf[0x18]<<16) + (buf[0x19]<<24);
	printf(" size is %d x %d\n", width, height);	

	bit_per_pixel = (buf[0x1c]<<0) + (buf[0x1d]<<8);
	printf("bit_per_pixel %d\n ", bit_per_pixel);

	comp_format = (buf[0x1e]<<0) + (buf[0x1f]<<8) + (buf[0x20]<<16) + (buf[0x21]<<24);
	if(comp_format != 0){
		printf("not bmp, %d\n", comp_format );
		return 0;
	}

	
	int seek = 0;
	//arr = (unsigned char *)malloc(sizeof(unsigned char) * width * height *3);
	arr = new unsigned char[width * height *3 *2];
	//for(int i =0;i<width*height*3;i++)
	//	*(arr+i) =0;

	lseek( fd, dat_offset, SEEK_SET);
	while( 1 ){

		len = pread( fd, buf, 255, dat_offset+seek);
		if(len <= 0){
			printf("data %x is missing\n", seek + dat_offset);
			seek++;
			}

		for(int i = 0; i < len; i++)
			*(arr + i + seek) = buf[i];

		if(seek > file_size - dat_offset){
			printf(" file read is %d\n", seek + dat_offset);
			return 0;
		}
		seek+=len;
	}
	printf("\n\n");
	
	return 0;
}




void BMP_dat::draw_bmp( int x_off, int y_off ){

	glBegin(GL_POINTS);
	for(int j= 0;j<height;j++){
	for(int i= 0;i<width;i++){
		float b = (float)arr[(j*(width+1) + i)*3 + 0]/256;
		float g = (float)arr[(j*(width+1) + i)*3 + 1]/256;
		float r = (float)arr[(j*(width+1) + i)*3 + 2]/256;
		glColor3f(r,g,b);
		glVertex2i(i + x_off, j + y_off);
	}}
	glEnd();
	
}







