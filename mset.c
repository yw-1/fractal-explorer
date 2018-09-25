#include <math.h>
#include <SDL2/SDL.h>

void draw_mset(SDL_Surface *surface, int w, int h, int iter, double zoom, double x0, double y0, Uint32 *palette, Uint32 palette_mask)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;
	int r, g, b;
	int i, ptr = 0;

	double x, y, u, v, up, vp;

	SDL_LockSurface(surface);

	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			x = (xx - w/2)*zoom + x0;
			y = (yy - h/2)*zoom + y0;

			u = 0;
			v = 0;

			for (i=0; i<iter; i++) {
				up = u*u - v*v + x;
				vp = 2*u*v + y;

				u = up;
				v = vp;

				if (u*u + v*v > 4) break;
			}

			if (i >= iter) c = 0x000000;
				else c = palette[i & palette_mask];

			pixmem[ptr++] = c;
		}
	}

	SDL_UnlockSurface(surface);
}

