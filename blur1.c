/**************************************************************

	The program reads an BMP image file and creates a new
	image that is the blurred version of the input file.

**************************************************************/

#include "qdbmp.h"
#include <stdio.h>
#include <stdlib.h>

/* Creates a blur image of the input bitmap file */
int main( int argc, char* argv[] )
{
	UCHAR	r, g, b;

	UINT	width, height;
	// UINT	x, y;
	int x = 0;
	int y = 0;
	int sum_r, sum_g, sum_b;
	UCHAR avg_r, avg_g, avg_b;
	int count = 0;
	// 
	int start_x = 0;
	int start_y = 0;
	int end_x = 0;
	int end_y = 0;
	int size = 0;
	int pos = 0;
	int box_size = 0;
	int temp_y = 0;
	int tempPos = 0;

	BMP*	bmp;

	/* Check arguments */
	if ( argc != 4 )
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

	UCHAR pixelsR[size];
	UCHAR pixelsG[size];
	UCHAR pixelsB[size];
	box_size = atoi(argv[3]);
	if (box_size <= 0){
		printf("sorry, box size cannot be negative or 0\n");
		return 0;
	}


	
	/* Iterate through all the image's pixels */
	for ( x = 0 ; x < width ; ++x )
	{
		for ( y = 0 ; y < height ; ++y )
		{
			/* Get pixel's RGB values */
			BMP_GetPixelRGB( bmp, x, y, &r, &g, &b );

            pos = x + y*width;
            pixelsR[pos] = r;
            pixelsG[pos] = g;
            pixelsB[pos] = b;
			// BMP_SetPixelRGB( bmp, x, y, 255 - r, 255 - g, 255 - b );
		}
	}

	for ( x = 0; x < width ; x++){
		
		for (y = 0; y < height; y++){
            count = 0;
            sum_r = 0;
            sum_g = 0;
            sum_b = 0;
            // check for boundaries
			if (x - box_size < 0){
				start_x = 0;
			} else {
				start_x = x - box_size;
			}

			if (y - box_size < 0){
				start_y = 0;
			} else {
				start_y = y - box_size;
			}

			if (x + box_size >= width){
				end_x = width - 1;
			} else {
				end_x = x + box_size;
			}

			if (y + box_size >= height){
				end_y = height - 1;
			} else {
				end_y = y + box_size;
			}

			while (start_x <= end_x){
				temp_y = start_y;
				while (temp_y <= end_y){

                	pos = start_x + temp_y*width;
					sum_r += pixelsR[pos];
					sum_g += pixelsG[pos];
					sum_b += pixelsB[pos];
					temp_y ++;
					count++;
	                
			    }
				start_x ++;
			}
			avg_r = sum_r/count;
			avg_g = sum_g/count;
			avg_b = sum_b/count;

			BMP_SetPixelRGB( bmp, x, y, avg_r, avg_g, avg_b );

		}
	}

	/* Save result */
	BMP_WriteFile( bmp, argv[ 2 ] );
	BMP_CHECK_ERROR( stdout, -2 );


	/* Free all memory allocated for the image */
	BMP_Free( bmp );

	return 0;
}

