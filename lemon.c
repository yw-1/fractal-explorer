/*
 *  implementation of http://paulbourke.net/fractals/lemon/
 */

#include <math.h>
#include <SDL2/SDL.h>

void cmul(double *u, double *v, double x, double y, double a, double b)
{
	*u = x*a - y*b;
	*v = x*b + y*a;
}

void cdiv(double *u, double *v, double x, double y, double a, double b)
{
	double d = a*a + b*b;

	*u = (x*a + y*b)/d;
	*v = (y*a - x*b)/d;
}

void draw_lemon(SDL_Surface *surface, int w, int h, int iter, double zoom, double x0, double y0, Uint32 *palette, Uint32 palette_mask)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;
	int r, g, b;
	int i, ptr = 0;

	double x, y, u, v, up, vp, uu, vv;
	int j;

	SDL_LockSurface(surface);

	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			x = (xx - w/2)*zoom + x0;
			y = (yy - h/2)*zoom + y0;

			u = y;
			v = x;

			j = 0;

			for (i=0; i<iter; i++) {
				cmul(&uu, &vv, u, v, u, v);
				cmul(&up, &vp, uu, vv, y, x);
				cmul(&up, &vp, up, vp, uu + 1, vv);
				uu--;
				cmul(&uu, &vv, uu, vv, uu, vv);
				cdiv(&up, &vp, up, vp, uu, vv);

				if (up*up + vp*vp > 1e10) {
					i = iter;
					break;
				}

				if (fabs(u - up) < 0.00001 && fabs(v - vp) < 0.00001) 
					j++;
				else
					j = 0;

				if (j > 4) break;

				u = up;
				v = vp;
			}

			if (i >= iter) c = 0x000000;
				else c = palette[i & palette_mask];

			pixmem[ptr++] = c;
		}
	}

	SDL_UnlockSurface(surface);
}

