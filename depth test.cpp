#include <cairo.h>
#include <cairo-pdf.h>

int main(){
	//set up surface
	cairo_surface_t * surface = cairo_pdf_surface_create("depth test.pdf", 8 * 72,	1 * 72); //8" by 1" converted into points
	cairo_t* cr = cairo_create(surface);

	//set up gradient
	cairo_pattern_t* pat;

	pat = cairo_pattern_create_linear(0.0, 0.0, 8*72, 1*72);
	cairo_pattern_add_color_stop_rgba(pat, 1, 0, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba(pat, 0, 1, 1, 1, 1);

	//boundary
	cairo_rectangle(cr, 0, 0, 256, 256);
	cairo_set_source(cr, pat);

	//fill and finish
	cairo_fill(cr);
	cairo_pattern_destroy(pat);
	cairo_show_page(cr);

	return 0;
}