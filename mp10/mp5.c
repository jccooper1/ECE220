/*									tab:8
 *
 * main.c - skeleton source file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 */
#include "mp5.h"
#define pow(x) ((x)*(x))
/*
	You must write all your code only in this file, for all the functions!
*/



/* 
 *  near_horizontal
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_horizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* This function draws a line that is nearly horizontal between two given points (x_start, y_start) and (x_end, y_end) */
	
	// Define variables to store the coordinates of the start and end points
	int32_t x1 = x_start;
	int32_t y1 = y_start;
	int32_t x2 = x_end;
	int32_t y2 = y_end;
	
	// Calculate the change in x and y between the two points
	int32_t dx = x2 - x1;
	int32_t dy = y2 - y1;
	
	// Define a variable to store the sign of the change in y
	int32_t sign;
	if(dy > 0){
		sign = 1;
	}
	if(dy == 0){
		sign = 0;
	}
	if(dy < 0){
		sign = -1;
	}

	// Loop through each x-coordinate between the start and end points
	int32_t becky = 1;
	if(x1<=x2){
	for(int32_t x = x1; x <= x2; x++){
		// Calculate the corresponding y-coordinate for the current x-coordinate
		int32_t y = y1 + (2 * dy * (x - x1) + dx * sign) / (2 * dx);
		
		// Draw a dot at the current (x, y) coordinate and update the value of 'becky'
		becky &= draw_dot(x, y);
	}
	}
	else if(x1>=x2){
	for(int32_t x = x1; x >= x2; x--){
		// Calculate the corresponding y-coordinate for the current x-coordinate
		int32_t y = y1 + (2 * dy * (x - x1) + dx * sign) / (2 * dx);
		
		// Draw a dot at the current (x, y) coordinate and update the value of 'becky'
		becky &= draw_dot(x, y);
	}
	}
	// Return the value of 'becky' to indicate whether any of the pixels drawn are out of bounds
	return becky;
}

/* 
 *  near_vertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_vertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* This function draws a line that is nearly vertical between two given points (x_start, y_start) and (x_end, y_end) */
	
	// Define variables to store the coordinates of the start and end points
	int32_t x1 = x_start;
	int32_t y1 = y_start;
	int32_t x2 = x_end;
	int32_t y2 = y_end;
	
	// Calculate the change in x and y between the two points
	int32_t dx = x2 - x1;
	int32_t dy = y2 - y1;
	
	// Define a variable to store the sign of the change in x
	int32_t sign;
	if(dx > 0){
		sign = 1;
	}
	if(dx == 0){
		sign = 0;
	}
	if(dx < 0){
		sign = -1;
	}
	
	// Define variables to store the current x-coordinate and the value of 'becky'
	int32_t y;
	int32_t x;
	int32_t becky = 1;
	
	// If the start and end points are the same, draw a dot at that point and update the value of 'becky'
	if(y_end == y_start){
		becky &= draw_dot(x_start, y_start);
		return becky;
	}
	else{
	// Loop through each y-coordinate between the start and end points
	if(y1<y2){
	for(y = y1; y <= y2; y++){
		// Calculate the corresponding x-coordinate for the current y-coordinate
		x = x1 + ((2 * dx * (y - y1)) + dy * sign) / (2 * dy);
		
		// Draw a dot at the current (x, y) coordinate and update the value of 'becky'
		becky &= draw_dot(x, y);
		}
	}
	else if(y1>y2){
	for(y = y1; y >= y2; y--){
		// Calculate the corresponding x-coordinate for the current y-coordinate
		x = x1 + ((2 * dx * (y - y1)) + dy * sign) / (2 * dy);
		
		// Draw a dot at the current (x, y) coordinate and update the value of 'becky'
		becky &= draw_dot(x, y);
		}
	}
	}

	// Return the value of 'becky' to indicate whether any of the pixels drawn are out of bounds
	return becky;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_line(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* This function draws a line between two given points (x_start, y_start) and (x_end, y_end) */
	
	// Define variables to store the coordinates of the start and end points
	int32_t x1 = x_start < x_end ? x_start : x_end;
	int32_t y1 = x_start < x_end ? y_start : y_end;
	int32_t x2 = x_start < x_end ? x_end : x_start;
	int32_t y2 = x_start < x_end ? y_end : y_start;
	
	// Calculate the change in x and y between the two points
	int32_t dx = x2 - x1;
	int32_t dy = y2 - y1;
	
	// Define variables to store the current y-coordinate and the value of 'becky'
	int32_t becky=1;
	
	// Calculate the slope of the line
	float slope = (float)dy / (float)dx;
	if(x_start == x_end){
		becky = near_vertical(x_start, y_start, x_end, y_end);
	}
	else{
	// If the slope is nearly horizontal, call the 'near_horizontal' function to draw the line
		if((-1 <= slope) && (slope <= 1)){
			becky = near_horizontal(x_start, y_start, x_end, y_end);
		}
	// If the slope is nearly vertical, call the 'near_vertical' function to draw the line
		else {
			becky = near_vertical(x_start, y_start, x_end, y_end);
		}
	// If the start and end points have the same x-coordinate, call the 'near_vertical' function to draw the line
	}
	
	// Return the value of 'becky' to indicate whether any of the pixels drawn are out of bounds
	return becky;
}
/* 
 *  draw_rect
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 * OUTPUTS: draws a pixel to every point of the edges of the rectangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

// Function to draw a rectangle with a given top left corner (x, y), width w, and height h
int32_t draw_rect(int32_t x, int32_t y, int32_t w, int32_t h){
	// Initialize a variable to keep track of the drawing status
	int32_t becky = 1;

	// If the width or height is negative, return 0 (failure)
	if(w < 0 || h < 0){
		return 0;
	}

	// Draw the top edge of the rectangle
	becky &= draw_line(x, y, x + w, y);
	// Draw the left edge of the rectangle
	becky &= draw_line(x, y, x, y + h);
	// Draw the right edge of the rectangle
	becky &= draw_line(x + w, y, x + w, y + h);
	// Draw the bottom edge of the rectangle
	becky &= draw_line(x, y + h, x + w, y + h);
	
	// Return the drawing status
	return becky;
}
/* 
 *  draw_triangle
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the triangle
 *         x_B,y_B -- the coordinates of another of the vertices of the triangle
 *         x_C,y_C -- the coordinates of the final of the vertices of the triangle
 * OUTPUTS: draws a pixel to every point of the edges of the triangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_triangle(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	// Initialize status variable
	int32_t	becky=1;

	// Draw the three lines that make up the triangle
	becky&=draw_line(x_A,y_A,x_B,y_B); // Line AB
	becky&=draw_line(x_A,y_A,x_C,y_C); // Line AC
	becky&=draw_line(x_B,y_B,x_C,y_C); // Line BC

	// Return the status
	return becky;
}
/* 
 *  draw_parallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a pixel to every point of the edges of the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	// Initialize status variable
	int32_t becky=1;

	// Calculate the coordinates of the fourth point (D) of the parallelogram
	int32_t	x_D=x_A+x_C-x_B;
	int32_t	y_D=y_A+y_C-y_B;

	// Draw the four lines that make up the parallelogram
	becky&=draw_line(x_A,y_A,x_B,y_B); // Line AB
	becky&=draw_line(x_B,y_B,x_C,y_C); // Line BC
	becky&=draw_line(x_A,y_A,x_D,y_D); // Line AD
	becky&=draw_line(x_C,y_C,x_D,y_D); // Line CD

	// Return the status
	return becky;
}

/* 
 *  draw_circle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

// Function to draw a circle with a given center (x, y), inner radius, and outer radius
int32_t draw_circle (int32_t x, int32_t y, int32_t inner_r, int32_t outer_r) {
	// Initialize a variable to keep track of the drawing status
	int32_t becky= 1;
	// Variables to hold the current point's coordinates
	int32_t x1;
	int32_t y1;
	// Variable to hold the square of the distance from the current point to the center
	int32_t	r_sq;

	// If the inner radius is negative or greater than the outer radius, return 0 (failure)
	if ((inner_r < 0) || (outer_r < inner_r)){
		return 0;
	}						
	
	// Loop over all points in the square with side length 2*outer_r centered at (x, y)
	for (x1 = x - outer_r; x1 <= x + outer_r; x1++) {
		for (y1 = y - outer_r; y1 <= y + outer_r; y1++) {
			// Calculate the square of the distance from the current point to the center
			r_sq = pow(x1 - x) + pow(y1 - y);
			// If the current point is within the circle (between the inner and outer radius), draw a dot at that point
			if ((r_sq>= pow(inner_r)) && (r_sq <= pow(outer_r))){
				becky&= draw_dot(x1, y1);
			}
		}
	}
	// Return the drawing status
	return becky;
}
/**
 *  rect_gradient
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 *         start_color -- the color of the far left side of the rectangle
 *         end_color -- the color of the far right side of the rectangle
 * OUTPUTS: fills every pixel within the bounds of the rectangle with a color
 *	    based on its position within the rectangle and the difference in
 *          color between start_color and end_color
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
rect_gradient(int32_t x, int32_t y, int32_t w, int32_t h, int32_t start_color, int32_t end_color){
	// Initialize variables
	int32_t becky=0;
	int32_t	level;
	int32_t	levelR;
	int32_t	levelG;
	int32_t	levelB;
	int32_t	signR;
	int32_t	signG;
	int32_t	signB;
	int32_t	x_position;
	int32_t	y_position;

	// Extract RGB components from start and end colors
	int32_t	red_st=(0x00FF0000&start_color)>>16;
	int32_t	red_ed=(0x00FF0000&end_color)>>16;
	int32_t	green_st=(0x0000FF00&start_color)>>8;
	int32_t	green_ed=(0x0000FF00&end_color)>>8;
	int32_t	blue_st=0x000000FF&start_color;
	int32_t	blue_ed=0x000000FF&end_color;

	// Determine the direction of color change for red
	if (red_ed-red_st<0){
		signR=-1;
	}
	if (red_ed-red_st==0){
		signR=0;
	}
	if (red_ed-red_st>0){
		signR=1;
	}

	// Determine the direction of color change for green
	if (green_ed-green_st<0){
		signG=-1;
	}
	if (green_ed-green_st==0){
		signG=0;
	}
	if (green_ed-green_st>0){
		signG=1;
	}

	// Determine the direction of color change for blue
	if (blue_ed-blue_st<0){
		signB=-1;
	}
	if (blue_ed-blue_st==0){
		signB=0;
	}
	if (blue_ed-blue_st>0){
		signB=1;
	}
	
	// Check for invalid dimensions
	if(h<0||w<1){
		becky=0;
	}

	// Loop over the rectangle's area
	for(x_position=x;x_position<=x+w;x_position++){
		for (y_position=y;y_position<=y+h;y_position++){
			// Calculate the color level for each RGB component
			levelR=(2*(x_position-x)*(red_ed-red_st)+w*signR)/(2*w)+red_st;
			levelG=(2*(x_position-x)*(green_ed-green_st)+w*signG)/(2*w)+green_st;
			levelB=(2*(x_position-x)*(blue_ed-blue_st)+w*signB)/(2*w)+blue_st;
			// Combine the RGB components into a single color
			level=(levelR<<16)+(levelG<<8)+levelB;
			// Set the color
			set_color(level);
			// Draw the dot and update the status
			becky&=draw_dot(x_position,y_position);
		}
	}
	
	// Return the status
	return becky;
}

/* 
 *  draw_picture
 *	 
 *	 
 *	
 *	
 * INPUTS: none
 * OUTPUTS: alters the image by calling any of the other functions in the file
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

//print hello and cxk 
int32_t
draw_picture(){
	/* Your code goes here! */
	set_color(10712825);
	draw_rect(0,0,30,100);
	draw_rect(30,30,60,30);
	draw_rect(90,0,30,100);
	rect_gradient(130,0,10,100,0xBEC712,0x00FFFF);
	rect_gradient(140,0,80,10,0x003F31,0x0023FF);
	rect_gradient(140,30,80,10,0x203F31,0x432848);
	rect_gradient(140,80,80,20,0x003413,0x0023FF);
	set_color(12345644);
	draw_circle(400,50,10,70);
	set_color(78945644);
	rect_gradient(270,0,15,100,0x446677,0x99EE11);
	set_color(78437822);
	rect_gradient(300,0,15,100,0x446887,0x99Ef21);
	set_color(12312644);
	draw_line(500,50,450,100);
	draw_line(500,50,550,100);
	draw_line(475,75,525,75);
	set_color(12312644);
	draw_line(150, 200, 250, 200); //  ---
	draw_line(150, 200, 100, 250); // /
	draw_line(100, 250, 150, 300); // "\"
	draw_line(150, 300, 250, 300); //  ---
	draw_line(300, 200, 350, 300); // "\"
	draw_line(350, 200, 300, 300); // |
	// Vertical line of "K"
	draw_line(450, 200, 450, 300);

	// Upper diagonal line of "K"
	draw_line(450, 250, 500, 200);

	// Lower diagonal line of "K"
	draw_line(450, 250, 500, 300);
	return 0;	
}
