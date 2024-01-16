/*									tab:8
 *
 * main.c - skeleton source file for ECE220 image processing MP6
 *
 * "Copyright (c) 2021 by Steven S. Lumetta
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
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
 * Author:	    Steven S. Lumetta
 * Version:	    1
 * Creation Date:   19 July 2021
 * Filename:	    mp6.h
 * History:
 *	SSL	1	19 July 2021
 *		First written.
 */




#include "mp6.h"
#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)
//
// convert_RGB_to_HSL -- convert red, green, and blue image data into
//   HSL channels.  See documentation for specific equations to be used.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           red -- red pixels (0-255) in column-major order
//           green -- green pixels (0-255) in column-major order
//           blue -- blue pixels (0-255) in column-major order
//   OUTPUTS: H -- hue (0-49151) in column-major order
//            S -- saturation (0-32768) in column-major order
//            L -- luma (0 to 510) in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//

void convert_RGB_to_HSL(int32_t height, int32_t width, const uint8_t* red, 
						const uint8_t* green, const uint8_t* blue,
						uint16_t* H, uint16_t* S, uint16_t* L)
{
	// Declare variables for maximum (M), minimum (N), red (r), green (g), blue (b), chroma (c), saturation (s), lightness (l), and hue (h)
	int32_t M, N, r, g, b, c, s, l, h;

	// Loop over each pixel in the image
	for(int32_t i = 0; i < height * width; i++){
		// Get the red, green, and blue values for the current pixel
		r = red[i];
		g = green[i];
		b = blue[i];

		// Calculate the maximum and minimum of the red, green, and blue values
		M = max(max(r, g), b);
		N = min(min(r, g), b);

		// Calculate the lightness and chroma
		l = M + N;
		c = M - N;

		// Calculate the saturation based on the lightness and chroma
		if(M == 0 || N == 255)
			s = 0;
		else if(l > 0 && l <= 255)
			s = 0x8000 * c / l;
		else
			s = 0x8000 * c / (510 - l);

		// Calculate the hue based on the maximum color component and chroma
		if(c == 0)
			h = 0;
		else if(M == r && c > 0)
			h = 0x2000 * (c + g - b) / c;
		else if(c > 0 && M > r && M == g)
			h = 0x2000 * (3 * c + b - r) / c;
		else
			h = 0x2000 * (5 * c + r - g) / c;

		// Store the hue, saturation, and lightness for the current pixel
		H[i] = h;
		S[i] = s;
		L[i] = l;
	}
}
//
// convert_HSL_to_RGB -- convert HSL channel data into red, green, and 
//   blue image data.  See documentation for specific equations to be used.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           H -- hue (0-49151) in column-major order
//           S -- saturation (0-32768) in column-major order
//           L -- luma (0 to 510) in column-major order
//   OUTPUTS: red -- red pixels (0-255) in column-major order
//            green -- green pixels (0-255) in column-major order
//            blue -- blue pixels (0-255) in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//
void convert_HSL_to_RGB(int32_t height, int32_t width, const uint16_t* H, 
						const uint16_t* S, const uint16_t* L, 
						uint8_t* red, uint8_t* green, uint8_t* blue)
{
	// Loop over each pixel in the image
	for(int32_t i = 0; i < height * width; i++){
		int32_t r, g, b, M, N, T, c, h, s, l, h_major, h_minor;
		
		// Get the hue, saturation, and lightness for the current pixel
		h = H[i];
		s = S[i];
		l = L[i];

		// Calculate chroma (c) based on lightness (l) and saturation (s)
		if(l <= 255){
			c = s * l / 0x8000;
		}
		else{
			c = s * (510 - l) / 0x8000;
		}

		// Calculate minimum (N) and maximum (M) color component values
		N = (l - c) / 2;
		M = N + c;

		// Calculate major and minor hue components
		h_major = h / 0x2000;
		h_minor = h & 0x3FFF;

		// Calculate intermediate value (T) based on minor hue component
		if(h_minor >= 0x2000){
			T = N + c * (h_minor - 0x2000) / 0x2000;
		}
		else{
			T = N + c * (0x2000 - h_minor) / 0x2000;
		}

		// Assign red (r), green (g), and blue (b) values based on major hue component
		switch(h_major){
			case 0: 
				r = M;
				g = N;
				b = T;
				break;
			case 1: 
				r = M;
				g = T;
				b = N;
				break;
			case 2: 
				r = T;
				g = M;
				b = N;
				break;
			case 3: 
				r = N;
				g = M;
				b = T;
				break;
			case 4: 
				r = N;
				g = T;
				b = M;
				break;
			default: 
				r = T;
				g = N;
				b = M;
				break;
		}

		// Store the red, green, and blue values for the current pixel
		red[i] = r;
		green[i] = g;
		blue[i] = b;
	}
}
//
// compute_sobel_kernels -- compute X and Y Sobel edge detection kernels
//   using convolution.  Edge pixels may be ignored, and kernel outputs
//   for these pixels left undefined.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           L -- luma (0-510) in column-major order
//   OUTPUTS: Gx -- Sobel X kernel pixels in column-major order
//            Gy -- Sobel Y kernel pixels in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//
void compute_sobel_kernels (int32_t height, int32_t width, const uint16_t* L, 
							int32_t* Gx, int32_t* Gy)
{
	// Loop over each pixel in the image, excluding the border pixels
	for(int32_t i = 1; i < height - 1; i++){
		for(int32_t j = 1; j < width - 1; j++){
			// Declare variables for the Sobel gradients in the x and y directions
			int32_t gx, gy;

			// Calculate the indices of the current pixel and its neighbors
			int32_t index_i_j = i * width + j;
			int32_t index_i_ad_1_j_sb_1 = (i + 1) * width + j - 1;
			int32_t index_i_ad_1_j_ad_1 = (i + 1) * width + j + 1;
			int32_t index_i_ad_1_j = (i + 1) * width + j;
			int32_t index_i_sb_1_j_sb_1 = (i - 1) * width + j - 1;
			int32_t index_i_sb_1_j = (i - 1) * width + j;
			int32_t index_i_sb_1_j_ad_1 = (i - 1) * width + j + 1;
			int32_t index_i_j_sb_1 = i * width + j - 1;
			int32_t index_i_j_ad_1 = i * width + j + 1;

			// Calculate the Sobel gradient in the x direction
			gx = (L[index_i_ad_1_j_sb_1] - L[index_i_ad_1_j_ad_1]) + 
				 (L[index_i_sb_1_j_sb_1] - L[index_i_sb_1_j_ad_1]) + 
				 2 * (L[index_i_j_sb_1] - L[index_i_j_ad_1]);

			// Calculate the Sobel gradient in the y direction
			gy = (L[index_i_sb_1_j_sb_1] + 2 * L[index_i_sb_1_j] + L[index_i_sb_1_j_ad_1]) -
				 (L[index_i_ad_1_j_sb_1] + 2 * L[index_i_ad_1_j] + L[index_i_ad_1_j_ad_1]);

			// Store the Sobel gradients for the current pixel
			Gx[index_i_j] = gx;
			Gy[index_i_j] = gy;
		}
	}
}
//
// equalize_intensities -- perform histogram equalization to re-map image
//   intensities.
//
//   INPUTS: height -- number of rows in image
//           width -- number of columns in image
//           L -- luma (0-510) in column-major order
//   OUTPUTS: L -- modified luma (0-510) in column-major order
//   RETURN VALUE: none
//   SIDE EFFECTS: none
//

void equalize_intensities(int32_t height, int32_t width, uint16_t* L) {
	// Calculate the total number of pixels in the image
	int64_t l_value = height * width;

	// Declare an array to store the mapping from old to new pixel values
	int64_t R_MAP[512] = {0};

	// Loop over each possible pixel value
	for(int64_t i = 0; i <= 510; i++){
		// Initialize a counter for the number of pixels with a value less than or equal to i
		int64_t k = 0;

		// Loop over each pixel in the image
		for(int64_t j = 0; j < l_value; j++){
			// If the pixel's value is less than or equal to i, increment the counter
			if(L[j] <= i){
				k++;
			}
		}

		// Calculate the new value for pixels with a value of i, based on the cumulative distribution function
		R_MAP[i] = (511 * k + l_value - 1) / l_value - 1;
	}

	// Loop over each pixel in the image
	for(int64_t i = 0; i < l_value; i++){
		// Replace the pixel's value with its new value
		L[i] = R_MAP[L[i]];
	}
}