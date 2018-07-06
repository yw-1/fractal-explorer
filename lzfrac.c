#include <math.h>
#include <SDL2/SDL.h>

void draw_lzfrac(SDL_Surface *surface, int w, int h, int iter, double zoom, double x0, double y0, Uint32 *palette, Uint32 palette_mask)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;
	int r, g, b;
	int i, ptr = 0;

	double p = 28, s = 10, bb = 8/3, zz = 0.356, hh = 0.01;
	double x, y, z;

	SDL_LockSurface(surface);

	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			x = (xx - w/2)*zoom + x0;
			y = (yy - h/2)*zoom + y0;
			z = zz;

			for (i=0; i<=iter; i++) {
				x = x + hh*s*(y - x);
				y = y + hh*(x*(p - z) - y);
				z = z + hh*(x*y - bb*z);

				if (x*x + y*y < 2.0) break;
			}

			if (i >= iter) c = 0x000000;
				else c = palette[i & palette_mask];

			pixmem[ptr++] = c;
		}
	}

	SDL_UnlockSurface(surface);
}

