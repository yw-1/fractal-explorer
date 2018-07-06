#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

void draw_box(SDL_Surface *surface, int x, int y, int w, int h)
{
	Uint32 c;
	Uint32 *pixmem = surface->pixels;

	int xx, yy, ww, hh;

	xx = x;
	yy = y;
	ww = w;
	hh = h;

	if (x < 0) {
		xx = 0;
		ww = w + x;
	}
	if (y < 0) {
		yy = 0;
		hh = h + y;
	}
	if (x + w > surface->w) ww = surface->w - x;
	if (y + h > surface->h) hh = surface->h - y - 1;

	SDL_LockSurface(surface);
	for (int i=0; i<ww; i++) {
		if (y >= 0) {
			c = pixmem[yy*(surface->pitch>>2) + xx + i];
			pixmem[yy*(surface->pitch>>2) + xx + i] = ~c;
		}

		if (y + h <= surface->h - 1) {
			c = pixmem[(yy + hh)*(surface->pitch>>2) + xx + i];
			pixmem[(yy + hh)*(surface->pitch>>2) + xx + i] = ~c;
		}
	}
	for (int i=1; i<hh; i++) {
		if (x >= 0) {
			c = pixmem[(yy + i)*(surface->pitch>>2) + xx];
			pixmem[(yy + i)*(surface->pitch>>2) + xx] = ~c;
		}

		if (x + w <= surface->w) {
			c = pixmem[(yy + i)*(surface->pitch>>2) + xx + ww - 1];
			pixmem[(yy + i)*(surface->pitch>>2) + xx + ww - 1] = ~c;
		}
	}
	SDL_UnlockSurface(surface);
}

void set_palette(Uint32 *palette, Uint32 palette_mask) {
	int r, g, b;

	for (int i=0; i<=palette_mask; i++) {
		r = i*palette_mask/300;
		g = i*0.5*(palette_mask*sin(0.01*i)+palette_mask)/300;
		b = i*0.5*(palette_mask*cos(0.01*i)+palette_mask)/300;
		palette[i] = (r & 0xff)<<16 | (g & 0xff)<<8 | (b & 0xff);
	}
}

int main(int argc, char *argv[])
{	
	SDL_Window *window;
	SDL_Surface *surface, *surface2;
	SDL_Event event;

	int sw = 640, sh = 480;
	int end = 0, redraw = 1;
	int mx, my;

	double x0 = 0, y0 = 0;
	double zoom = 0.01, zoom_rate = 0.1;
	int iter = 100;

	Uint32 palette[512];
	Uint32 palette_mask = 0x1ff;

	set_palette(palette, palette_mask);

	char title[255];
	snprintf(title, 255, "(%f, %f), zoom=%f, iter=%d", x0, y0, zoom, iter);

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			sw, sh, SDL_WINDOW_RESIZABLE);

	surface = SDL_GetWindowSurface(window);
	surface2 = SDL_CreateRGBSurface(0, sw, sh, 32, 0, 0, 0, 0);


	while (!end) {
		if (redraw) {
			draw_mset(surface2, sw, sh, iter, zoom, x0, y0, palette, palette_mask);
			//draw_trfrac(surface2, sw, sh, iter, zoom, x0, y0, palette, palette_mask);
			//draw_lzfrac(surface2, sw, sh, iter, zoom, x0, y0, palette, palette_mask);
			//draw_lemon(surface2, sw, sh, iter, zoom, x0, y0, palette, palette_mask);

			SDL_BlitSurface(surface2, 0, surface, 0);
			SDL_UpdateWindowSurface(window);
			redraw = 0;
		}

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				end = 1;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					sw = event.window.data1;
					sh = event.window.data2;

					SDL_FreeSurface(surface);
					SDL_FreeSurface(surface2);

					surface = SDL_GetWindowSurface(window);
					surface2 = SDL_CreateRGBSurface(0, sw, sh, 32, 0, 0, 0, 0);

					redraw = 1;
					break;
				case SDL_WINDOWEVENT_LEAVE:
					SDL_BlitSurface(surface2, 0, surface, 0);
					SDL_UpdateWindowSurface(window);
					break;
				default:
					SDL_UpdateWindowSurface(window);
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					end = 1;
					break;
				case SDLK_KP_PLUS:
					iter += 100;
					redraw = 1;
					break;
				case SDLK_KP_MINUS:
					iter -= 100;
					redraw = 1;
					break;
				}

				snprintf(title, 255, "(%f, %f), zoom=%f, iter=%d", x0, y0, zoom, iter);
				SDL_SetWindowTitle(window, title);

				break;

			case SDL_MOUSEBUTTONDOWN:
				x0 += (event.button.x - sw/2)*zoom;
				y0 -= (sh/2 - event.button.y)*zoom;

				if (event.button.button == 1)
					zoom *= zoom_rate;
				else
					zoom /= zoom_rate;

				snprintf(title, 50, "(%f, %f), zoom=%f, iter=%d", x0, y0, zoom, iter);
				SDL_SetWindowTitle(window, title);

				redraw = 1;
				break;

			case SDL_MOUSEMOTION:
				SDL_BlitSurface(surface2, 0, surface, 0);
				draw_box(surface, event.motion.x - (int)(sw*zoom_rate)/2,
						event.motion.y - (int)(sh*zoom_rate)/2, sw*zoom_rate, sh*zoom_rate);
				SDL_UpdateWindowSurface(window);
				break;

			case SDL_MOUSEWHEEL:
				if (zoom_rate + event.wheel.y*0.01 > 0.01) {
					zoom_rate += event.wheel.y*0.01;

					SDL_GetMouseState(&mx, &my);
					SDL_BlitSurface(surface2, 0, surface, 0);
					draw_box(surface, mx - (int)(sw*zoom_rate)/2,
							my - (int)(sh*zoom_rate)/2, sw*zoom_rate, sh*zoom_rate);
					SDL_UpdateWindowSurface(window);
				}
				break;
			}

			SDL_FlushEvent(event.type);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

