#include <png.h>
#include <stdlib.h>
#include "../mp5.h"

typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} pixel_t;

int32_t color;
pixel_t *picture_data;

pixel_t int_to_pixel(int32_t integer){
	pixel_t pixel;
	pixel.red = (unsigned char) ((integer & 0x00FF0000) >> 16);
	pixel.green = (unsigned char) ((integer & 0x0000FF00) >> 8);
	pixel.blue = (unsigned char) (integer & 0x000000FF);
	return pixel;
}

void initialize_and_clear_canvas(){
	picture_data = malloc(WIDTH * HEIGHT * sizeof(pixel_t));
	int32_t i,j;
	// Make the whole thing white
	for(i = 0; i < HEIGHT; i++){
			for(j = 0; j < WIDTH; j++){
		picture_data[i*WIDTH + j] = int_to_pixel(color);
		}
	}


	return;
}

void set_color(int32_t new_color){
	color = new_color;
	return;
}

void free_image_data(){
	free(picture_data);

	return;
}

void make_png(char * str){
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
    int32_t status = -1;
    /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
    */
    int32_t pixel_size = 3;
    int32_t depth = 8;

    fp = fopen (str, "wb");
    if (! fp) {
        //goto fopen_failed;
				fprintf(stderr, "Failed to get file.");
				return;

    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
			fprintf(stderr, "PNG Failure. /n Failed to create PNG write struct.");
			fclose (fp);
	        //fopen_failed:
			return;

			//goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        //goto png_create_info_struct_failed; // Ew GOTOs are supposed to be evil. Even though they were decent here.
				png_destroy_write_struct (&png_ptr, &info_ptr);
		 //png_create_write_struct_failed:
				fclose (fp);
		 //fopen_failed:
				return;
		}

    /* Set up error handling. */

    if (setjmp (png_jmpbuf (png_ptr))) {
        //goto png_failure;
				// If anyone does read this. Remember kids, GOTOs do not exist.
				png_destroy_write_struct (&png_ptr, &info_ptr);
		 //png_create_write_struct_failed:
				fclose (fp);
		 //fopen_failed:
				return;
    }

    /* Set image attributes. */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  WIDTH,
                  HEIGHT,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = png_malloc (png_ptr, HEIGHT * sizeof (png_byte *));
    for (y = 0; y < HEIGHT; y++) {
        png_byte *row =
            png_malloc (png_ptr, sizeof (unsigned char) * WIDTH * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < WIDTH; x++) {
            pixel_t pixel = picture_data[y*WIDTH + x];
            *row++ = pixel.red;
            *row++ = pixel.green;
            *row++ = pixel.blue;
        }
    }

    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status++;

    for (y = 0; y < HEIGHT; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);

 //png_failure:
 //png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 //png_create_write_struct_failed:
    fclose (fp);
 //fopen_failed:
    return;
}

int32_t draw_dot(int32_t x, int32_t y){
	if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
		picture_data[y*WIDTH + x] = int_to_pixel(color);
		return 1;
	}
	else{
		return 0;
	}
}
