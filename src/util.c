#include <util.h>

void png_save(const char * filename, int width, int height, void * pixels)
{
	static const unsigned t[] = {
		0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	};
	FILE * fp = fopen(filename, "wb");
	if(!fp || !pixels)
		return;
	unsigned a = 1, b = 0, c, p = width * 4 + 1, x, y, i;
	unsigned char * data = (unsigned char *)pixels;
	#define PNG_U8A(ua, l) for (i = 0; i < l; i++) fputc((ua)[i], fp);
	#define PNG_U32(u) do { fputc((u) >> 24, fp); fputc(((u) >> 16) & 255, fp); fputc(((u) >> 8) & 255, fp); fputc((u) & 255, fp); } while(0)
	#define PNG_U8C(u) do { fputc(u, fp); c ^= (u); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while(0)
	#define PNG_U8AC(ua, l) for (i = 0; i < l; i++) PNG_U8C((ua)[i])
	#define PNG_U16LC(u) do { PNG_U8C((u) & 255); PNG_U8C(((u) >> 8) & 255); } while(0)
	#define PNG_U32C(u) do { PNG_U8C((u) >> 24); PNG_U8C(((u) >> 16) & 255); PNG_U8C(((u) >> 8) & 255); PNG_U8C((u) & 255); } while(0)
	#define PNG_U8ADLER(u) do { PNG_U8C(u); a = (a + (u)) % 65521; b = (b + a) % 65521; } while(0)
	#define PNG_BEGIN(s, l) do { PNG_U32(l); c = ~0U; PNG_U8AC(s, 4); } while(0)
	#define PNG_END() PNG_U32(~c)
	PNG_U8A("\x89PNG\r\n\32\n", 8);
	PNG_BEGIN("IHDR", 13);
	PNG_U32C(width);
	PNG_U32C(height);
	PNG_U8C(8);
	PNG_U8C(6);
	PNG_U8AC("\0\0\0", 3);
	PNG_END();
	PNG_BEGIN("IDAT", 2 + height * (5 + p) + 4);
	PNG_U8AC("\x78\1", 2);
	for(y = 0; y < height; y++)
	{
		PNG_U8C(y == height - 1);
		PNG_U16LC(p);
		PNG_U16LC(~p);
		PNG_U8ADLER(0);
		for(x = 0; x < p - 1; x++, data++)
			PNG_U8ADLER(*data);
	}
	PNG_U32C((b << 16) | a);
	PNG_END();
	PNG_BEGIN("IEND", 0);
	PNG_END();
	fclose(fp);
}

void cg_surface_write_to_png(struct cg_surface_t * surface, const char * filename)
{
	unsigned char * data = surface->pixels;
	int width = surface->width;
	int height = surface->height;
	int stride = surface->stride;
	unsigned char * image = malloc((size_t)(stride * height));
	for(int y = 0; y < height; y++)
	{
		uint32_t * src = (uint32_t *)(data + stride * y);
		uint32_t * dst = (uint32_t *)(image + stride * y);
		for(int x = 0; x < width; x++)
		{
			uint32_t a = src[x] >> 24;
			if(a != 0)
			{
				uint32_t r = (((src[x] >> 16) & 0xff) * 255) / a;
				uint32_t g = (((src[x] >> 8) & 0xff) * 255) / a;
				uint32_t b = (((src[x] >> 0) & 0xff) * 255) / a;
				dst[x] = (a << 24) | (b << 16) | (g << 8) | r;
			}
			else
			{
				dst[x] = 0;
			}
		}
	}
	png_save(filename, width, height, image);
	free(image);
}
