#include "image.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int h1;
int h2;
int w1;
int w2;
int x1;
int x2;
int rx1;
int ry1;
int lim;
int divi;
int iCurve;

struct Array
{
    int y1;
    int y2;
	ImageData *img;
	ImageData *outimg;
};

int swapval(int *v1,int *v2)
{
	int dum;

	dum= *v1;
	*v1 = *v2;
	*v2 = dum;
}

int interpolate(ImageData *img,double x, double y, Pixel *col)
{
	int px, py;

	px = (int)(x + 0.5);
	py = (int)(y + 0.5);

	return getPixel(img, px, py, col);
}

void* generic(void * arg){
	struct Array *Arr =(struct Array*)arg;
    struct Array A = *Arr;
	Pixel col;
	int x,y,i;
	double fx, fy;
	col.r = col.g = col.b = 128;
	for(y = A.y1; y <= A.y2; y++) {
		for(x = x1; x <= x2; x++) {
			fx = (double)(x - x1) * w1 / (double)w2 + rx1;
			fy = (double)(y) * h1 / (double)h2 + ry1;
			if(interpolate(A.img, fx, fy, &col))	
				setPixel(A.outimg, x, y, &col);
		}
	}
	pthread_exit(0);
}


void* curvecurve(void * arg){
	struct Array *Arr =(struct Array*)arg;
    struct Array A = *Arr;
	Pixel col;
	int x,y;
	double fx, fy;
	int flag=1,i=0,s=0;
	lim = 20;
	col.r = col.g = col.b = 128;
	for(y=A.y1,i=0;y<=A.y2;y++) {
		for(x=x1;x<=x2;x++) {
			fx=(double)(x-x1)*(w1+x-s)/(double)(w2+x2+s)+rx1;
			fy=(double)(y)*h1/(double)h2+ry1;

			interpolate(A.img,fx,fy,&col);
			setPixel(A.outimg,x,y,&col);	
		}

		if(y%4 == 0){ 
			i+=5;
			if(flag == 1) s = i;
			else s = lim-i;
			if(i > (lim-1)){
				i %= lim;
				flag *= -1;
			}
		}
	}
	pthread_exit(0);
}

void* opposite(void * arg){
	struct Array *Arr =(struct Array*)arg;
    struct Array A = *Arr;
	Pixel col;
	int x,y;
	double fx, fy;
	col.r = col.g = col.b = 128;
	for(y = A.y1 ; y < A.y2; y++) {
		for(x = x1; x <=x2; x++) {
			//reverse filled
			fx = x2 - ((double)(x - x1) * w1 / (double)w2 + rx1);
			fy = lim - ((double)(y) * h1 / (double)h2 + ry1);

			if(interpolate(A.img,fx, fy, &col))
				setPixel(A.outimg, x, y, &col);			
		}
	}
	pthread_exit(0);
}

void transform(ImageData *img, ImageData *outimg,int n, int action){
	Pixel col;
	int x,y;
	double fx, fy;
	int y1 = 0;
	int y2 = outimg->height - 1;
	x1 = 0;
	x2 = outimg->width - 1;
	rx1 = 0;
	ry1 = 0;
	h1 = (img->height - 1) - ry1 + 1;
	h2 = y2 - y1 + 1;
	w1 = (img->width - 1) - rx1 + 1;
	w2 = x2 - x1 + 1;

    struct Array A;
    A.y1 = 0;
    A.y2 = outimg->height - 1;
    A.img = img;
    A.outimg = outimg;
	
	pthread_t tids[n-1];
	
	divi = (int)(img->height/n);
	lim = outimg->height-1;
	
	int i;
	iCurve = 0;
    struct Array arr[n-1];
    col.r = col.g = col.b = 128;
	for(i=0;i<n-1;i++){
		pthread_attr_t attr;
		pthread_attr_init(&attr);
        A.y2 = A.y1 + divi;
        arr[i] = A;
        // AQUI SE HACE EL IF CON EL NUMERO DE PARAMETRO        
		if(action == 1){
		pthread_create(&tids[i], &attr, curvecurve, &arr[i]);
		}
		else if(action == 2 || action == 3 || action == 4 || action == 5){
		pthread_create(&tids[i], &attr, generic, &arr[i]);
		}
		else if(action == 6){
		pthread_create(&tids[i], &attr, opposite, &arr[i]);
		}
        A.y1 = A.y2;
	}

	if(action == 1){
		int flag=1,s=0,lim = 20;
		for(y=A.y1,i=0;y < outimg->height;y++) {
			for(x=x1;x<=x2;x++) {
				fx= (double)(x-x1)*(w1+x-s)/(double)(w2+x2+s)+rx1;
				fy= ((double)(y)*h1/(double)h2+ry1);
				interpolate(img,fx,fy,&col);
				setPixel(outimg,x,y,&col);	
			}
			if(y%4 == 0){ 
				i+=5;
				if(flag == 1) s = i;
				else s = lim-i;
				if(i > (lim-1)){
					i %= lim;
					flag *= -1;
				}
			}
		}
	}

	else if(action == 2 || action == 3 || action == 4 || action == 5){/////////////////////////////////////////////
		for(y = A.y1; y < outimg->height; y++) {
			for(x = x1; x <= x2; x++){
				fx = (double)(x - x1) * w1 / (double)w2 + rx1;
				fy = (double)(y) * h1 / (double)h2 + ry1;
				if(interpolate(img, fx, fy, &col))	
					setPixel(outimg, x, y, &col);
			}
		}
	}

	else if(action == 6){
		for(y = A.y1; y < outimg->height; y++) {
			for(x = x1; x <= x2; x++) {
				fx = x2 - ((double)(x - x1) * w1 / (double)w2 + rx1);
				fy = lim - ((double)(y) * h1 / (double)h2 + ry1);
				if(interpolate(img, fx, fy, &col))
					setPixel(outimg, x, y, &col);
			}
		}
	}

	for(i=0;i<n-1;i++){
		pthread_join(tids[i], NULL);
	}
}

int main(int ac, char *av[])
{
    ImageData *img,*outimg;

	int res,n;
	int nx, ny;

	//av[1] ~ image input
	//av[2] ~ image output
	//av[3] ~ number of threads
	//av[4] ~ number of action

	if(ac < 5) {
		printf("invalid parameters!");
		return 0;
	}

	res = readBMPfile(av[1], &img);
	if(res < 0) {
		printf("image not found!");
		return 0;
	}

	int action = atoi(av[4]);

	if(action == 2){
		nx = img->width;
		ny = (img->height / 2 + 0.5);
	}

	else if(action == 3){
		nx = img->width * 2;
		ny = img->height;
	}

	else if(action == 4){
		nx = img->width;
		ny = img->height * 2;
	}

	else if(action == 5){
		nx = (img->width/2 + 0.5);
		ny = img->height;
	}

	else{
		nx = img->width;
		ny = img->height;
	}

	outimg = createImage(nx, ny, 24);
 
	n = atoi(av[3]);

	transform(img,outimg,n,action);
	
	writeBMPfile(av[2], outimg);

	disposeImage(img);
	disposeImage(outimg);
	  	
	return 0;
}