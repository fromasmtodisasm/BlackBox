#include <stdio.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define NUM_GLYPHS 0x4ff - 0x400
#define BEGIN_GLYPH 0x400

struct glyph_info {
	int x0, y0, x1, y1;	// coords of glyph in the texture atlas
	int x_off, y_off;   // left & top bearing when rendering
	int advance;        // x advance when rendering
} info[NUM_GLYPHS];

int main(int argc, char** argv){
	argc = 3;
	if(argc < 3){
		printf("usage: %s <font> <size>\n", argv[0]);
		return 1;
	}

	FT_Library ft = NULL;
	FT_Face    face = NULL;

	FT_Init_FreeType(&ft);
	FT_New_Face(ft, "arial.ttf", 0, &face);
	FT_Set_Char_Size(face, 0, atoi("16") << 6, 96, 96);

	// quick and dirty max texture size estimate

	int max_dim = (int)((1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(NUM_GLYPHS)));
	int tex_width = 1;
	while(tex_width < max_dim) tex_width <<= 1;
	int tex_height = tex_width;

	// render glyphs to atlas
	
	char* pixels = (char*)calloc(tex_width * tex_height, 1);
	int pen_x = 0, pen_y = 0;

	for(int i = BEGIN_GLYPH; i < (BEGIN_GLYPH + NUM_GLYPHS); ++i){
		FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if(pen_x + bmp->width >= (unsigned)tex_width){
			pen_x = 0;
			pen_y += ((face->size->metrics.height >> 6) + 1);
		}

		for(int row = 0; row < (int)bmp->rows; ++row){
			for(int col = 0; col < (int)bmp->width; ++col){
				int x = pen_x + col;
				int y = pen_y + row;
				pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		// this is stuff you'd need when rendering individual glyphs out of the atlas

		info[i].x0 = pen_x;
		info[i].y0 = pen_y;
		info[i].x1 = pen_x + bmp->width;
		info[i].y1 = pen_y + bmp->rows;

		info[i].x_off   = face->glyph->bitmap_left;
		info[i].y_off   = face->glyph->bitmap_top;
		info[i].advance = face->glyph->advance.x >> 6;

		pen_x += bmp->width + 1;
	}

	FT_Done_FreeType(ft);

	// write png

	char* png_data = (char*)calloc(tex_width * tex_height * 4, 1);
	for(int i = 0; i < (tex_width * tex_height); ++i){
		png_data[i * 4 + 0] |= pixels[i];
		png_data[i * 4 + 1] |= pixels[i];
		png_data[i * 4 + 2] |= pixels[i];
		png_data[i * 4 + 3] = 0xff;
	}

	stbi_write_png("font_output.png", tex_width, tex_height, 4, png_data, tex_width * 4);

	free(png_data);
	free(pixels);

	return 0;
}

