#include <math.h>
#include <SDL2/SDL.h>

void dft (double *x, double *xx, int n)
{
	double *g1 = malloc(sizeof(double)*n/2);
	double *g2 = malloc(sizeof(double)*n/2);
	double sum;

	for (int i=0; i<n/2; i++) {
		sum = 0;
		g1[i] = 0;
		g2[i] = 0;
		for (int j=0; j<n/2; j++) {
			g1[i] += x[2*j]*cos(4.0*M_PI*i*j/n);
			g2[i] += x[2*j + 1]*cos(4.0*M_PI*i*j/n);
		}

		g2[i] *= cos(2.0*M_PI*i/n);
		xx[i] = g1[i] + g2[i];
		xx[i + n/2] = g1[i] - g2[i];
	}

	free(g1);
	free(g2);
}

void draw_test(SDL_Surface *surface, int w, int h, int iter, double zoom, double x0, double y0, Uint32 *palette, Uint32 palette_mask)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;
	int r, g, b;
	int i, ptr = 0;

	double x, y, u, v, up, vp;

	SDL_LockSurface(surface);

	int num = 64;

	double f[num];
	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			x = (xx - w/2)*zoom + x0;
			y = (yy - h/2)*zoom + y0;

			u = x;
			v = y;

			for (i=0; i<iter; i++) {
				up = u*u - v*v + x;
				vp = 2*u*v + y;

				u = fabs(up);
				v = fabs(vp);

				if (u*u + v*v > 4) break;
			}
			if (i >= iter) c = 0x000000;
				else c = palette[i & palette_mask];

			pixmem[ptr++] = c;
		}
	}

	SDL_UnlockSurface(surface);
}

