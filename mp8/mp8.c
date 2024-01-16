#include <stdlib.h>
#include <string.h>

#include "mp8.h"

#define pow(x) ((x)*(x))
//my introduction starts here
//the basic flood fill function is to flood the color from the starting point to the boundary no matter what the color is
//the grey flood fill function is to flood the color from the starting point to the boundary if the color is near white
//the limited flood fill function is to flood the color from the starting point to the boundary if the color is near the starting point and the distance is less than 35

/*
 * basicFlood -- wrapper for flood filling recursively from a point until 
 *               reaching white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicFlood (int32_t width, int32_t height,
		const uint8_t* inRed, const uint8_t* inGreen, 
		const uint8_t* inBlue, 
		int32_t startX, int32_t startY, 
		uint8_t floodR, uint8_t floodG, uint8_t floodB,
		uint8_t* outRed, uint8_t* outGreen, 
		uint8_t* outBlue)
{
	// Initialize RGB indices
	int64_t index_R = 0;
	int64_t index_G = 0;
	int64_t index_B = 0;

	// Set all pixels in the output red channel to 0
	memset(outRed,0,width*height);

	// Call the recursive function to perform the flood fill operation
	basicRecurse(width,height,inRed,inGreen,inBlue,startX,startY,outRed);

	// Loop over each pixel in the image
	for(int32_t i=0;i<height;i++){
		for(int32_t j=0;j<width;j++){
			// Get the RGB values of the current pixel in the input image
			index_R = inRed[i*width+j];
			index_B = inBlue[i*width+j];
			index_G = inGreen[i*width+j];

			// If the pixel was marked for filling
			if(outRed[i*width+j]==1){
				// Set the color of the pixel in the output image to the flood fill color
				outRed[i*width+j] = floodR;
				outGreen[i*width+j] = floodG;
				outBlue[i*width+j] = floodB;
			}
			else{
				// Set the color of the pixel in the output image to the color of the pixel in the input image
				outRed[i*width+j] = index_R;
				outGreen[i*width+j] = index_G;
				outBlue[i*width+j] = index_B;
			}
		}
	}
}


/*
 * colorsWithinDistSq -- returns 1 iff two colors are within Euclidean
 *                       distance squared of one another in RGB space
 * INPUTS: r1 -- red component of color 1
 *         g1 -- green component of color 1
 *         b1 -- blue component of color 1
 *         r2 -- red component of color 2
 *         g2 -- green component of color 2
 *         b2 -- blue component of color 2
 *         distSq -- maximum distance squared for the check
 * RETURN VALUE: 1 if the sum of the squares of the differences in the 
 *               three components is less or equal to distSq; 0 otherwise
 * SIDE EFFECTS: none
 */
int32_t
colorsWithinDistSq (uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t r2, uint8_t g2, uint8_t b2, uint32_t distSq)
{
	if(pow(r1-r2)+pow(g1-g2)+pow(b1-b2)<=distSq)
    return 1;
	else
	return 0;
}


/*
 * greyFlood -- wrapper for flood filling recursively from a point until 
 *              reaching near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyFlood (int32_t width, int32_t height,
	   const uint8_t* inRed, const uint8_t* inGreen, 
	   const uint8_t* inBlue, 
	   int32_t startX, int32_t startY, 
	   uint8_t floodR, uint8_t floodG, uint8_t floodB, uint32_t distSq,
	   uint8_t* outRed, uint8_t* outGreen, 
	   uint8_t* outBlue)
{
	// Initialize RGB indices
	int32_t	index_R=0;
	int32_t index_G=0;
	int32_t index_B=0;

	// Set all pixels in the output red channel to 0
	memset(outRed,0,width*height);

	// Call the recursive function to perform the flood fill operation
	greyRecurse(width,height,inRed,inGreen,inBlue,startX,startY,distSq,outRed);

	// Loop over each pixel in the image
	for(int32_t i=0;i<height;i++){
		for(int32_t j=0;j<width;j++){
			// Get the RGB values of the current pixel in the input image
			index_R = inRed[i*width+j];
			index_G = inGreen[i*width+j];
			index_B = inBlue[i*width+j];

			// If the pixel was marked for filling
			if(outRed[i*width+j]==1){
				// Set the color of the pixel in the output image to the flood fill color
				outRed[i*width+j] = floodR;
				outGreen[i*width+j] = floodG;
				outBlue[i*width+j] = floodB;
			}
			else{
				// Set the color of the pixel in the output image to the color of the pixel in the input image
				outRed[i*width+j] = index_R;
				outGreen[i*width+j] = index_G;
				outBlue[i*width+j] = index_B;
			}
		}
	}
}
/*
 * limitedFlood -- wrapper for flood filling recursively from a point until 
 *                 reaching pixels too different (in RGB color) from the 
 *                 color at the flood start point, too far away 
 *                 (> 35 pixels), or beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         startX -- starting x position of the flood
 *         startY -- starting y position of the flood
 *         floodR -- red component of the flood color
 *         floodG -- green component of the flood color
 *         floodB -- blue component of the flood color
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedFlood (int32_t width, int32_t height,
		  const uint8_t* inRed, const uint8_t* inGreen, 
		  const uint8_t* inBlue, 
		  int32_t startX, int32_t startY, 
		  uint8_t floodR, uint8_t floodG, uint8_t floodB, uint32_t distSq,
		  uint8_t* outRed, uint8_t* outGreen, 
		  uint8_t* outBlue)
{
	// Initialize RGB indices
	int32_t	index_R=0;
	int32_t index_G=0;
	int32_t index_B=0;

	// Set all pixels in the output red channel to 0
	memset(outRed,0,width*height);

	// Call the recursive function to perform the flood fill operation
	limitedRecurse(width,height,inRed,inGreen,inBlue,startX,startY,startX,startY,distSq,outRed);

	// Loop over each pixel in the image
	for(int32_t i=0;i<height;i++){
		for(int32_t j=0;j<width;j++){
			// Get the RGB values of the current pixel in the input image
			index_R = inRed[i*width+j];
			index_G = inGreen[i*width+j];
			index_B = inBlue[i*width+j];

			// If the pixel was marked for filling
			if(outRed[i*width+j]==1){
				// Set the color of the pixel in the output image to the flood fill color
				outRed[i*width+j] = floodR;
				outGreen[i*width+j] = floodG;
				outBlue[i*width+j] = floodB;
			}
			else{
				// Set the color of the pixel in the output image to the color of the pixel in the input image
				outRed[i*width+j] = index_R;
				outGreen[i*width+j] = index_G;
				outBlue[i*width+j] = index_B;
			}
		}
	}
}