#include <math.h>
#include <SDL2/SDL.h>

void cmul2(double *u, double *v, double u1, double v1, double u2, double v2)
{
	*u = u1*u2 - v1*v2;
	*v = u1*v2 - u2*v1;
}

void cexp2(double *u, double *v, double x, double y, double a, double b)
{
	double mag, arg, lnz, argz;

	lnz = x*x + y*y;

	if (lnz == 0) {
		*u = 0;
		*v = 0;
	}else{
		lnz = 0.5 * log(lnz);
		argz = atan2(y, x);

		mag = exp(a*lnz - b*argz);
		arg = a*argz + b*lnz;

		*u = mag * cos(arg);
		*v = mag * sin(arg);
	}
}

void draw_trfrac(SDL_Surface *surface, int w, int h, int iter, double zoom, double x0, double y0, Uint32 *palette, Uint32 palette_mask)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;
	int r, g, b;
	int i, ptr = 0;
	double z = 1;

	double x, y, u, v, up, vp;

	SDL_LockSurface(surface);

	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			x = (xx - w/2)*zoom + x0;
			y = (yy - h/2)*zoom + y0;

			u = exp(-1);
			v = 0;

			for (i=0; i<iter; i++) {
				cexp2(&u, &v, u, v, u, v);
				cexp2(&u, &v, u, v, x, y);

				if (u*u + v*v > 4.0) break;
			}

			if (i >= iter) c = 0x000000;
			else c = palette[i & palette_mask];

			pixmem[ptr++] = c;
		}
	}

	SDL_UnlockSurface(surface);
}

