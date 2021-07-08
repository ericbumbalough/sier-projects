#define _USE_MATH_DEFINES
#include <cairo.h>
#include <cairo-pdf.h>
#include <cmath>

//#include <iostream> //for debugging
//using namespace std;

/*******NOTES********
could eliminate the "lower" coordinate and infer that from iteration number

*/

void cairo_polygon(cairo_t* cr, int n, const double verticies[][2]) {
	/*given a cairo context, cr, and a matrix of the form {{x1, y1}, {x2, y2},... {xn, yn}} adds a polygon defined by the
	matrix points to the cairo path*/

	for (int i = 0; i <= n - 1; i++) { //loop through the matrix
		cairo_line_to(cr, verticies[i][0], verticies[i][1]);
	}
	cairo_close_path(cr);
}

double inch_to_points(double inches) {
	//converts give inch measurements to points
	return inches * 72;
}

double xp_to_x(int xp, int yp, bool lower, double triangle_spacing, double triangle_edge) {
	/* Given xp and yp coordinates of a triangle, and length of the triangle edge
	returns the x-location of the given triangle*/
	return ((double)xp + (double)yp) * (triangle_edge / 2 + sqrt(3) / 2 * triangle_spacing); //I don't understand why this spacing offset seems to work
}

double yp_to_y(int xp, int yp, bool lower, double triangle_spacing, double triangle_height, int i) {
	/* Given xp and yp coordinates of a triangle, upper (false) or lower (true), height of the triangle, and iteration i
	returns the x-location of the given triangle*/
	double correction[] = { 0.0, 0.0, 0.5, -0.25, 0.625, -0.25 - 1.0 / 16.0, .625+1.0/32.0, -5.0/16.0-1.0/64.0, 0.0, 0.0, 0.0, 0.0 };//includes iteration based correction that I cdon't undersatnd
	return ((double)xp - (double)yp) * (triangle_height + 1.5 * triangle_spacing) - lower * (triangle_spacing + triangle_height) + correction[i] * triangle_height;
}

int main() {
	//dimensions in inches and other constants
	const double width_in = 92 / 25.4;
	const double height_in = 140.5 / 25.4;
	const double edge_pad_in = 0.25; //padding outside of the page dimension
	const double corner_radius_in = 6 / 25.4;
	const double hole_to_edge_dist_in = 5.6 / 25.4; //distance from vertical edge to center of holes
	const double hole_dia_in = 0.15;
	const double hole_pitch_in = 8 / 25.4 * 1.07; //with correction factor
	const double triangle_edge_in = 1;
	const double triangle_spacing_in = 0.05; //was 0.05 //space between triangle iterations
	const int    num_holes = 15;
	const double darkness[] = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 , 0.7}; //values for rgb for the given iteration
	//const double darkness[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //all black for troubleshooting
	const int    num_iter = 7; //number of iterations. 0 is the center triangle
	const int    num_triangles_in_iter[] = { 1, 3, 6, 9, 12, 15, 18, 21, 24, 27 };// nth element is the number of full size triangles that are iter n,
	const int* ptr_triangle_coords[10] = {}; //MUST ALSO UPDATE num_iter above and saving memory location below. nth eleement is pointer to an array holding the coordinates of all triangles in the nth iteration
	const int    coords0[1][3] = { {0, 0, 0} }; //each row is {xp, yp, lower}
	const int    coords1[3][3] = { {0, -1, 1},
								   {1, 0, 1},
								   {0, 0, 1} };
	const int    coords2[6][3] = { {0,-1, 0},
								   {1, 0, 0},
								   {1, 1, 0},
								   {0, 1, 0},
								   {-1, 0, 0},
								   {-1, -1, 0} };
	const int	 coords3[9][3] = { {0, 1, 1},
								   {-1, 0, 1},
								   {-1, -1, 1},
								   {-1, -2, 1},
								   {0, -2, 1},
								   {1, -1, 1},
								   {2, 0, 1},
								   {2, 1, 1},
								   {1, 1, 1} };
	const int    coords4[12][3] = { {2, 0, 0},
									{2,1,0},
									{2,2,0},
									{1,2,0},
									{0,2,0},
										{-1, 1, 0},
										{-2, 0,0},
										{-2, -1, 0},
										{-2, -2, 0},
										{-1, -2, 0},
										{0, -2, 0},
										{1, -1, 0} };
	const int coords5[15][3] = { {3,0,1}, {3,1,1}, {3,2,1}, {2,2,1}, {1,2,1}, {0,2,1}, {-1,1,1}, {-2,0,1},{-2,-1,1},{-2,-2,1},{-2, -3, 1},{-1, -3, 1},{0,-3,1},
		{1,-2,1},{2,-1,5} };
	const int coords6[18][3] = { {3,0,0},{3,1,0},{3,2,0},{3,3,0},{2,3,0},{1,3,0},{0,3,0},{-1,2,0},{-2,1,0},
		{-3,0,0},{-3,-1,0},{-3,-2,0},{-3,-3,0},{-2,-3,0},{-1,-3,0},{0,-3,0},{1,-2,0},{2,-1,0} };
	const int coords7[21][3] = { {4,0,1},{4,1,1},{4,2,1},{4,3,1},{3,3,1},{2,3,1},{1,3,1},{0,3,1},{-1,2,1},{-2,1,1},
		{-3,0,1},{-3,-1,1},{-3,-2,1},{-3,-3,1},{-3,-4,1},{-2,-4,1},{-1,-4,1},{0,-4,1},{1,-3,1},{2,-2,1},{3,-1,1} };

	//convert dimensions and generate constants
	const double width = inch_to_points(width_in);
	const double height = inch_to_points(height_in);
	const double edge_pad = inch_to_points(edge_pad_in); //padding outside of the page dimension
	const double corner_radius = inch_to_points(corner_radius_in);
	const double hole_to_edge_dist = inch_to_points(hole_to_edge_dist_in);
	const double hole_dia = inch_to_points(hole_dia_in);
	const double hole_pitch = inch_to_points(hole_pitch_in);
	const double triangle_edge = inch_to_points(triangle_edge_in);
	const double triangle_spacing = inch_to_points(triangle_spacing_in);
	const double triangle_height = sqrt(3.0) / 2.0 * triangle_edge; //height of the triangle, a common constant
	const double tri_vert[3][2] = { //verticies of the first triangle, relative to triangle centroid at (0,0)
		{-triangle_edge / 2, -triangle_height / 3},
		{triangle_edge / 2, -triangle_height / 3},
		{0.0, triangle_height * 2 / 3} };

	//save memory locations into pointer
	ptr_triangle_coords[0] = &coords0[0][0];
	ptr_triangle_coords[1] = &coords1[0][0];
	ptr_triangle_coords[2] = &coords2[0][0];
	ptr_triangle_coords[3] = &coords3[0][0];
	ptr_triangle_coords[4] = &coords4[0][0];
	ptr_triangle_coords[5] = &coords5[0][0];
	ptr_triangle_coords[6] = &coords6[0][0];
	ptr_triangle_coords[7] = &coords7[0][0];

	//PDF initialize surface and context
	cairo_surface_t* surface = cairo_pdf_surface_create("cover.pdf",
		width + 2 * edge_pad,
		height + 2 * edge_pad);
	cairo_t* cr = cairo_create(surface); //outline
	cairo_t* tri = cairo_create(surface); //triangles
	cairo_set_line_width(cr, 0.1);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

	//cover vector outline
	cairo_move_to(cr, edge_pad, edge_pad); //bottom left corner
	cairo_arc(cr, //bottom corner. will automatically add bottom edge
		edge_pad + width - corner_radius, //xc
		edge_pad + corner_radius, //yc
		corner_radius, //radius
		3.0 * M_PI / 2.0, //starting angle in radians - bottom
		0.0); //ending angle in radians - right
	cairo_arc(cr, //top corner. will automatically add right edge
		edge_pad + width - corner_radius, //xc
		edge_pad + height - corner_radius, //yc
		corner_radius,
		0.0,  //starting angle in radians - right
		M_PI / 2.0); //ending angle in radians - top
	cairo_line_to(cr, edge_pad, edge_pad + height); //top edge
	cairo_close_path(cr); //finish outl1]ine with left edge

	//binding holes
	double bottom_hole_yc = height / 2 + edge_pad - ((double)num_holes - 1) / 2 * hole_pitch; //center - half the height of all the holes
	for (int i = 0; i <= num_holes - 1; i++) { //loop through each hole
		cairo_new_sub_path(cr); //prevent lines between all the holes
		cairo_arc(cr,
			edge_pad + hole_to_edge_dist, //xc
			bottom_hole_yc + i * hole_pitch, //yc = bottom + how many holes higher
			hole_dia / 2, //radius
			0.0, //start angle
			2 * M_PI); //end angle
	}

	//clip
	cairo_path_t* temp = {}; //temporary variable to hold the path
	temp = cairo_copy_path(cr); //save book outline for clipping
	cairo_append_path(tri, temp); //append path in tri context
	cairo_clip(tri);
	cairo_new_path(tri); //just in case clip didn't clear the path

	//set up triangle parameters
	cairo_set_line_width(tri, 0.01);
	cairo_set_source_rgb(tri, 0.0, 0.0, 0.0);
	cairo_scale(tri, 1.0, -1.0); //y-axis mirror to make a normal coordinate system
	cairo_translate(tri, 0.0, -2.0 * edge_pad - height); //shift down to complete transform to normal coordinate system


	//first triangle
	cairo_translate(tri, edge_pad + width / 2, edge_pad + height / 2); //translate to center
	cairo_polygon(tri, 3, tri_vert); //add triangle to path

	//trace lines
	cairo_stroke(cr);
	cairo_stroke_preserve(tri); //outline for vector cut
	//cairo_fill_preserve(tri); //fill for asthetics. need to remove for production to prevent wasteful rastering


	//iterate
	for (int i = 1; i <= num_iter; i++) { //once for each iteration

		//build next sierpinski iteration
		int flip = 1; //store whether the coordinate system has been flipped. 1 = not flipped, -1 = flipped
		if (i % 2 == 1) { //odd iteration implies flipped
			flip = -1;
		}

		cairo_save(tri); //save full scale, centered
		temp = cairo_copy_path(tri); //copy the path
		cairo_translate(tri, triangle_edge * pow(2, i - 1), 0); //move to bottom right triangle
		cairo_append_path(tri, temp); //add triangle(s)			 
		cairo_translate(tri, -triangle_edge / 2 * pow(2, i - 1), triangle_height * pow(2, i - 1)); //move to top triangle
		cairo_append_path(tri, temp); //add triangle(s)
		temp = cairo_copy_path(tri); //copy path with all triangles
		cairo_restore(tri); //restore full scale, centered
		cairo_new_path(tri); //clear current path

		//place new triangles
		cairo_set_source_rgb(tri, darkness[i], darkness[i], darkness[i]);
		cairo_save(tri); //save full scale, centered 
		cairo_scale(tri, pow(2.0, -i), flip * pow(2.0, -i)); //scale down based on current iteration
		int upscale = pow(2, i); //scale up translations to compenstate for scaled down coordinates

		for (int j = 0; j <= num_triangles_in_iter[i] - 1; j++) { //for each triangle of the ith iterations 
			cairo_save(tri); //save current scale, centered
			cairo_translate(tri, //move to new location. pow to scale up and move further since the coordinate system has been scaled down
				xp_to_x(ptr_triangle_coords[i][j * 3], //xp.
					ptr_triangle_coords[i][j * 3 + 1], //yp.
					ptr_triangle_coords[i][j * 3 + 2], //lower.
					triangle_spacing, triangle_edge) * upscale,
				yp_to_y(ptr_triangle_coords[i][j * 3], //xp.
					ptr_triangle_coords[i][j * 3 + 1], //yp.
					ptr_triangle_coords[i][j * 3 + 2], //lower.
					triangle_spacing, triangle_height, i) * flip * upscale);
			cairo_append_path(tri, temp); //add triangle(s)
			cairo_restore(tri); //back to current scale, centered
		}
		cairo_fill(tri);
		cairo_restore(tri); //restore full scale, centered
		cairo_append_path(tri, temp); //return to path with one set for this iteration
	}



	//send to pdf
	cairo_show_page(cr);
	cairo_show_page(tri);

	//destroy surface and context
	cairo_destroy(cr);
	cairo_destroy(tri);
	cairo_surface_destroy(surface);
}
