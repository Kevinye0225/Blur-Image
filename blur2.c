/**************************************************************

	The program reads an BMP image file and creates a new
	image that is the negative of the input file.

**************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* blur(void* p);

int size = 0;
int interval = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int box_size = 0;
UCHAR pixelsR[62000];
UCHAR pixelsG[62000];
UCHAR pixelsB[62000];
BMP*	bmp;
int width = 0;
int height = 0;

/* Creates a negative image of the input bitmap file */
int main( int argc, char* argv[] )
{
	UCHAR	r, g, b;
    int pos = 0;
	int i = 0;
	int startPos = 0;
	int get_x = 0;
	int get_y = 0;
	int thread_num = 0;


    


	/* Check arguments */
	if ( argc != 5 )
	{
		fprintf( stderr, "Usage: %s <input file> <output file>\n", argv[ 0 ] );
		return 0;
	}
    
	/* Read an image file */
	bmp = BMP_ReadFile( argv[ 1 ] );
	BMP_CHECK_ERROR( stdout, -1 );
	/* Get image's dimensions */
	width = BMP_GetWidth( bmp );
	height = BMP_GetHeight( bmp );
	size = width*height;
	box_size = atoi(argv[3]);
	thread_num = atoi(argv[4]);

	if (box_size <= 0){
		printf("sorry, box size cannot be negative or 0\n");
		return 0;
	}

	if (thread_num <= 0){
		printf("sorry, thread number cannot be negative or 0\n");
		return 0;
	}
	interval = size/(thread_num-1);
	// printf("%d\n", size);
	pthread_t thread[thread_num];
	int inter[thread_num];

	/* Iterate through all the image's pixels */
	for ( get_x = 0 ; get_x < width ; get_x++ )
	{
		for ( get_y = 0 ; get_y < height ; get_y++ )
		{
			/* Get pixel's RGB values */
			BMP_GetPixelRGB( bmp, get_x, get_y, &r, &g, &b );

            pos = get_x + get_y*width;
            pixelsR[pos] = r;
            pixelsG[pos] = g;
            pixelsB[pos] = b;
			// BMP_SetPixelRGB( bmp, x, y, 255 - r, 255 - g, 255 - b );
		}
	}

	while (i < thread_num){ 
		inter[i] = startPos;
		i++;
		startPos = startPos + interval;
	}


    i = 0;
	while (i < thread_num){
		pthread_create(&thread[i], NULL, &blur, &inter[i]);
		i++;
	}
	


	i = 0;
	while (i < thread_num){
		pthread_join(thread[i], NULL);
		i++;
	}

	/* Save result */
	BMP_WriteFile( bmp, argv[ 2 ] );
	BMP_CHECK_ERROR( stdout, -2 );


	/* Free all memory allocated for the image */
	BMP_Free( bmp );

	return 0;
}

void* blur(void* p){
	UCHAR avgR, avgG, avgB;
	int x = 0;
	int y = 0;
	int sumR = 0;
	int sumG = 0;
	int sumB = 0;
	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;
	int count = 0;
	int tempPos = *(int*)p;
    int tempY = 0;
    int pos = 0;
    int nextPos = tempPos + interval;


    while (tempPos < nextPos && tempPos < size){

            count = 0;
            sumR = 0;
            sumG = 0;
            sumB = 0;
            y = tempPos/width;
            x = tempPos%width;


            // check for boundaries
			if (x - box_size < 0){
				startX = 0;
			} else {
				startX = x - box_size;
			}

			if (y - box_size < 0){
				startY = 0;
			} else {
				startY = y - box_size;
			}

			if (x + box_size >= width){
				endX = width - 1;
			} else {
				endX = x + box_size;
			}

			if (y + box_size >= height){
				endY = height - 1;
			} else {
				endY = y + box_size;
			}

			while (startX <= endX){
				tempY = startY;
				while (tempY <= endY){
					pos = startX + tempY*width;
					sumR += pixelsR[pos];
					sumG += pixelsG[pos];
					sumB += pixelsB[pos];
					tempY ++;
					count++;
				}
				startX ++;
			}

			avgR = sumR/count;
			avgG = sumG/count;
			avgB = sumB/count;



            pthread_mutex_lock(&lock);
			BMP_SetPixelRGB( bmp, x, y, avgR, avgG, avgB );
			pthread_mutex_unlock(&lock);

			tempPos++;
    }

    return NULL;


}

