#include "image.h"

ImageData* createImage(int width,int height,int depth)
{
  ImageData *newimg;
  int byte_per_pixel;

  if(width<0 || height<0) return NULL;
  if(depth!=8 && depth!=24) return NULL;
  
  newimg=malloc(sizeof(ImageData));
  if(newimg==NULL) return NULL;

  byte_per_pixel=depth/8;

  newimg->pixels=malloc(sizeof(BYTE)*byte_per_pixel*width*height);
  if(newimg->pixels==NULL) {
    free(newimg);
    return NULL;
  }

  newimg->width=width;
  newimg->height=height;
  newimg->depth=depth;
  return newimg;
}

void disposeImage(ImageData *img)
{
  if(img->pixels!=NULL) free(img->pixels);
  free(img);
  return;
}

int getPixel(ImageData *img,int x,int y,Pixel *pix)
{
  int ret=1;
  int adr;  
  int dep,val;
  BYTE *pixels;

  if(img==NULL) return -1;
  if(img->pixels==NULL) return -1;

  if(x<0) {
    x=0;
    ret=0;
  }
  if(x >= img->width ) {
    x=img->width -1;
    ret=0;
  }
  if(y<0) {
    y=0;
    ret=0;
  }
  if(y >= img->height ) {
    y=img->height -1;
    ret=0;
  }
  dep=img->depth;
  adr=x + y*img->width;
  pixels=img->pixels;
  if(dep==8) {  
    val=pixels[adr];
    pix->r=val;
    pix->g=val;
    pix->b=val;
  }
  else if(dep==24) {
    pixels+=(adr*3);
    pix->r=(*pixels);
    pixels++;
    pix->g=(*pixels);
    pixels++;
    pix->b=(*pixels);
  }
  else {
    return -1;
  }
  return ret;
}

int correctValue(int val,int max)
{
  if(val<0) return 0;
  if(val>max) return max;
  return val;
}

int setPixel(ImageData *img,int x,int y,Pixel *pix)
{
  int adr;  
  int dep,val;
  BYTE *pixels;

  if(img==NULL) return -1;
  if(img->pixels==NULL) return -1;

  if(x<0 || x >= img->width || y<0 || y >= img->height ) {
    return 0;
  }
  dep=img->depth;
  adr=x + y*img->width;
  pixels=img->pixels;
  if(dep==8) {
    pixels[adr]=correctValue(pix->r,PIXELMAX);
  }
  else if(dep==24) {
    pixels+=(adr*3);
    (*pixels)=correctValue(pix->r,PIXELMAX);
    pixels++;
    (*pixels)=correctValue(pix->g,PIXELMAX);
    pixels++;
    (*pixels)=correctValue(pix->b,PIXELMAX);
  }
  else {
    return -1;
  }
  return 1;
}