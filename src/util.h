#include <cg.h>

#ifndef __UTIL_H__
    #define __UTIL_H__

    void png_save(const char * filename, int width, int height, void * pixels);
    void cg_surface_write_to_png(struct cg_surface_t * surface, const char * filename);
    
#endif