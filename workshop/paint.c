#include <cg.h>
#include <util.h>

int main(void){
    printf("Hello, World!\n");
    struct cg_surface_t * surface = cg_surface_create(256, 256);
	struct cg_ctx_t * ctx = cg_create(surface);

	for(int i = 1; i < 16; i++)
	{
		cg_set_line_cap(ctx, CG_LINE_CAP_ROUND);
		cg_set_line_width(ctx, i * 0.5);
		cg_set_source_rgb(ctx, 0, 0, 0);
		cg_move_to(ctx, 16 * i + 8, 16);
		cg_line_to(ctx, 16 * i - 8, 240);
		cg_stroke(ctx);
	}

    cg_surface_write_to_png(surface, "test.png");
    cg_destroy(ctx);
	cg_surface_destroy(surface);

    return 0;
}
