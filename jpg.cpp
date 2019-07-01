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

#include "jpg.h"

int JPG_dat::load(const char filename[]){
  unsigned long data_size;     // length of the file
  int channels;               //  3 =>RGB   4 =>RGBA 
  unsigned int type;  
  unsigned char * rowptr[1];    // pointer to an array
  struct jpeg_decompress_struct info; //for our jpeg info
  struct jpeg_error_mgr err;          //the error handler

  FILE* file = fopen(filename, "rb");  //open the file

  info.err = jpeg_std_error(& err);     
  jpeg_create_decompress(& info);   //fills info structure

  //if the jpeg file doesn't load
  if(!file) {
     fprintf(stderr, "Error reading JPEG file %s!", filename);
     return 0;
  }

  jpeg_stdio_src(&info, file);    
  jpeg_read_header(&info, TRUE);   // read jpeg file header

  jpeg_start_decompress(&info);    // decompress the file

  //set width and height
  width = info.output_width;
  height = info.output_height;
  channels = info.num_components;
  type = GL_RGB;
  if(channels == 4) type = GL_RGBA;


  arr = (unsigned char *)malloc( height * width * 3);
  while (info.output_scanline < info.output_height) {
    rowptr[0] = (unsigned char *)arr +  // secret to method
            3* info.output_width * info.output_scanline; 

    jpeg_read_scanlines(&info, rowptr, 1);
  }

  jpeg_finish_decompress(&info);   //finish decompressing

/*
	glGenTextures(1,&texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  gluBuild2DMipmaps(GL_TEXTURE_2D,3,width,height,GL_RGB,GL_UNSIGNED_BYTE,arr);
  */
	

  jpeg_destroy_decompress(&info);
  fclose(file);                    //close the file

  return 0;
}

void JPG_dat::draw( int x_off, int y_off ){

	glBegin(GL_POINTS);
	for(int j= 0;j<height;j++){
	for(int i= 0;i<width;i++){
		float r = (float)arr[(j*(width) + i)*3]/256;
		float g = (float)arr[(j*(width) + i)*3 +1]/256;
		float b = (float)arr[(j*(width) + i)*3 +2]/256;
		glColor3f(r,g,b);
		glVertex2i(i + x_off, (height-j) + y_off);
	}}
	glEnd();
}







