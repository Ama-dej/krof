#include <stdio.h>
#include <math.h>
#ifdef linux
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#define WIDTH 64
#define HEIGHT 32

#define TORUS_DISTANCE 16.0
#define SCREEN_DISTANCE 1.0

#define TORUS_INNER_RADIUS 5.0
#define TORUS_OUTER_RADIUS 2.0

#define TORUS_MIN_Z (TORUS_DISTANCE - TORUS_INNER_RADIUS - TORUS_OUTER_RADIUS / 2)
#define TORUS_MAX_Z (TORUS_DISTANCE + TORUS_INNER_RADIUS + TORUS_OUTER_RADIUS / 2)

double depth[HEIGHT][WIDTH];
#define SHADE_COUNT 7
char shades[SHADE_COUNT] = "Wwli:,.";

int main()
{
	for (double f = 0; 1 == 1; f += M_PI / 128) {
		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j < WIDTH; j++)
				depth[i][j] = 999999999; // ok

		printf("\e[1;1H\e[2J");

		double cf = cos(f);
		double sf = sin(f);

		for (double a = 0; a < 2 * M_PI; a += M_PI / 128) {
			double ca = cos(a);
			double sa = sin(a);

			// okoli y osi
			double tmp = TORUS_INNER_RADIUS * ca;

			double ax = tmp * cf; 
			double ay = TORUS_INNER_RADIUS * sa;
			double az = tmp * sf;

			/*
			// okoli x osi
			double tmp = TORUS_INNER_RADIUS * sa;

			double ax = TORUS_INNER_RADIUS * ca;
			double ay = tmp * cf;
			double az = -tmp * sf;
			*/

			for (double g = 0; g < 2 * M_PI; g += M_PI / 64) {
				double x = ax + TORUS_OUTER_RADIUS * cos(g);
				double y = ay + TORUS_OUTER_RADIUS * sin(g);
				double z = az + TORUS_DISTANCE;

				double dx = (x * SCREEN_DISTANCE) / z;
				double dy = (y * SCREEN_DISTANCE) / z;
				dx = (dx + 0.5) * WIDTH;
				dy = (dy + 0.5) * HEIGHT;

				//printf("(%f, %f, %f), (%f, %f)\n", x, y, z, dx, dy);

				int idx = (int)(dx + 0.5);
				int idy = (int)(dy + 0.5);

				double d = sqrt(x * x + y * y + z * z);

				if (depth[idy][idx] > d)
					depth[idy][idx] = d;
			}
		}

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				double z = depth[i][j];

				if (z <= TORUS_MAX_Z) {
					z = (z - TORUS_MIN_Z) / (TORUS_MAX_Z - TORUS_MIN_Z);
					int n = z * SHADE_COUNT;
					printf("%c", shades[n]);	
				} else {
					printf(" ");
				}
			}

			printf("\n");
		}

		#ifdef linux
		usleep(1000 * 16);
		#endif
		#ifdef _WIN32
		Sleep(16);
		#endif
	}

	return 0;
}
