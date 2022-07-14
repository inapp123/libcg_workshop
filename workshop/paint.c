#include <cg.h>
#include <util.h>

int main(void){
    printf("Hello, World!\n");
    struct cg_surface_t * surface = cg_surface_create(320, 240);
    struct cg_ctx_t * ctx = cg_create(surface);


    cg_set_source_rgba(ctx, 1.0, 1, 1, 1);
	cg_rectangle(ctx, 0, 0, surface->width, surface->height);
	cg_fill(ctx);

    cg_ellipse(ctx, surface->width/2, surface->height/2, surface->width * 0.2, surface->height * 0.45);
	cg_set_source_rgba(ctx,55/255.0,130/255.0,161/255.0,1.0);
    cg_fill(ctx);

	cg_ellipse(ctx, surface->width/2, surface->height/2, surface->width * 0.2 * 0.2, surface->height * 0.45 * 0.2);
	cg_set_source_rgba(ctx,0/255.0,0/255.0,0/255.0,0.6);
    cg_fill(ctx);

    cg_surface_write_to_png(surface, "../test.png");
    cg_destroy(ctx);
	cg_surface_destroy(surface);

    return 0;
}
