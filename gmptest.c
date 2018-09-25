#include <math.h>
#include <gmp.h>
#include <SDL2/SDL.h>

void draw_test(SDL_Surface *surface, int w, int h, int iter, double zoom, double x0, double y0, Uint32 *palette, Uint32 palette_mask)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;
	//int r, g, b;
	int i, ptr = 0;

	//double x, y, u, v, up, vp;
	mpf_t x, y, u, v, up, vp, r;
	/*
	mpf_init2(x,  512);
	mpf_init2(y,  512);
	mpf_init2(u,  512);
	mpf_init2(v,  512);
	mpf_init2(up, 512);
	mpf_init2(vp, 512);
	mpf_init2(r,  512);
	*/
	mpf_init(x);
	mpf_init(y);
	mpf_init(u);
	mpf_init(v);
	mpf_init(up);
	mpf_init(vp);
	mpf_init(r);

	SDL_LockSurface(surface);

	for (int yy=0; yy<h; yy++) {
		for (int xx=0; xx<w; xx++) {
			//x = (xx - w/2)*zoom + x0;
			//y = (yy - h/2)*zoom + y0;
			mpf_set_d(x, (xx - w/2)*zoom + x0);
			mpf_set_d(y, (yy - h/2)*zoom + y0);

			mpf_set_d(u, 0);
			mpf_set_d(v, 0);

			for (i=0; i<iter; i++) {
				//up = u*u - v*v + x;
				//vp = 2*u*v + y;
				mpf_mul(up, u, u);
				mpf_mul(vp, v, v);
				mpf_sub(up, up, vp);
				mpf_add(up, up, x);

				mpf_mul(vp, u, v);
				mpf_mul_ui(vp, vp, 2);
				mpf_add(vp, vp, y);

				//u = (up);
				//v = (vp);
				mpf_set(u, up);
				mpf_set(v, vp);
				//if (u*u + v*v > 4) break;
				mpf_mul(up, u, u);
				mpf_mul(vp, v, v);
				mpf_add(r, up, vp);

				if (mpf_cmp_d(r, 4) > 0) break;
			}
			if (i >= iter) c = 0x000000;
				else c = palette[i & palette_mask];

			pixmem[ptr++] = c;
		}
	}

	SDL_UnlockSurface(surface);

	mpf_clear(x);
	mpf_clear(y);
	mpf_clear(u);
	mpf_clear(v);
	mpf_clear(up);
	mpf_clear(vp);
	mpf_clear(r);

}

