#include <stdlib.h>
#include <string.h>

#include "mp8.h"

#define pow(x) ((x)*(x))
//my introduction starts here
//the basic recurse function is to interative flood the color from the starting point to the boundary no matter what the color is, and the order is as what the instruction says
//if the color run into the XFFFFFF then it should stop

//the grey recurse function is to interative flood the color from the starting point to the boundary if the color is near white, and the order is as what the instruction says
//and it check the distance between the color and xFFFFFF by using the colordistance function

//the limited recurse function is to interative flood the color from the starting point to the boundary if the color is near the starting point and the distance is less than 35, and the order is as what the instruction says
//it also check the distance between the color and the starting point by using the colordistance function, to see the distance is less than 35 or not
/*
 * basicRecurse -- flood fill recursively from a point until reaching
 *                 white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicRecurse (int32_t width, int32_t height,
		  const uint8_t* inRed, const uint8_t* inGreen, 
		  const uint8_t* inBlue, 
		  int32_t x, int32_t y, 
		  uint8_t* marking)
{
	// Calculate the index of the current pixel
	int32_t index=x+y*width;

	// Check if the current position is within the image boundaries
	if(x<0||x>=width||y<0||y>=height){
		return;
	}

	// Check if the pixel has already been visited
	if(marking[index]==1){
		return;
	}

	// Check if the pixel is white
	if(inRed[index]==255&&inGreen[index]==255&&inBlue[index]==255){
		return;
	}

	// Mark the pixel as visited
	marking[index]=1;

	// Recurse on the adjacent pixels
	if(y>0){
		basicRecurse(width,height,inRed,inGreen,inBlue,x,y-1,marking); // Up
	}
	if(x<width-1){
		basicRecurse(width,height,inRed,inGreen,inBlue,x+1,y,marking); // Right
	}
	if(y<height-1){
		basicRecurse(width,height,inRed,inGreen,inBlue,x,y+1,marking); // Down
	}
	if(x>0){
		basicRecurse(width,height,inRed,inGreen,inBlue,x-1,y,marking); // Left
	}
}
/*
 * greyRecurse -- flood fill recursively from a point until reaching
 *                near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyRecurse (int32_t width, int32_t height,
	     const uint8_t* inRed, const uint8_t* inGreen, 
	     const uint8_t* inBlue, 
	     int32_t x, int32_t y, uint32_t distSq, 
	     uint8_t* marking)
{
	int32_t index=x+y*width;
	int32_t r=inRed[index];
	int32_t g=inGreen[index];
	int32_t b=inBlue[index];
	//the first conditions to check if the current position is within the image boundaries
	if(x<0||x>=width||y<0||y>=height){
		return;
	}
	if(marking[index]==1){
		return;
	}
	//check if the color is near white
	if(colorsWithinDistSq(r,g,b,255,255,255,distSq)){
		return;
	}
	marking[index]=1;
	if(y>0){
		greyRecurse(width,height,inRed,inGreen,inBlue,x,y-1,distSq,marking);//up
	}
	if(x<width-1){
		greyRecurse(width,height,inRed,inGreen,inBlue,x+1,y,distSq,marking);//right
	}
	if(y<height-1){
		greyRecurse(width,height,inRed,inGreen,inBlue,x,y+1,distSq,marking);//down
	}
	if(x>0){
		greyRecurse(width,height,inRed,inGreen,inBlue,x-1,y,distSq,marking);//left
	}
}


/*
 * limitedRecurse -- flood fill recursively from a point until reaching
 *                   pixels too different (in RGB color) from the color at
 *                   the flood start point, too far away (> 35 pixels), or
 *                   beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         origX -- starting x position of the flood
 *         origY -- starting y position of the flood
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedRecurse (int32_t width, int32_t height,
	        const uint8_t* inRed, const uint8_t* inGreen, 
	        const uint8_t* inBlue, 
	        int32_t origX, int32_t origY, int32_t x, int32_t y, 
		uint32_t distSq, uint8_t* marking)
{
	int32_t st_x_r=inRed[origX+origY*width];
	int32_t ed_x_r=inRed[x+y*width];
	int32_t st_x_g=inGreen[origX+origY*width];
	int32_t	ed_x_g=inGreen[x+y*width];
	int32_t st_x_b=inBlue[origX+origY*width];
	int32_t ed_x_b=inBlue[x+y*width];
	//the following is checking the conditions
	if(pow(origX-x)+pow(origY-y)>pow(35)){
		return;
	}
	if(!colorsWithinDistSq(st_x_r,st_x_g,st_x_b,ed_x_r,ed_x_g,ed_x_b,distSq)){
		return;
	}
	if(x<0||x>=width||y<0||y>=height){
		return;
	}
	if(marking[x+y*width]==1){
		return;
	}
	marking[x+y*width]=1;
	//interative flood the color from the starting point to the boundary if the color is near the starting point and the distance is less than 35
	if(y>0){
		limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x,y-1,distSq,marking);//up
	}
	if(x<width-1){
		limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x+1,y,distSq,marking);//right
	}
	if(y<height-1){
		limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x,y+1,distSq,marking);//down
	}
	if(x>0){
		limitedRecurse(width,height,inRed,inGreen,inBlue,origX,origY,x-1,y,distSq,marking);//left
	}
}

