#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* image;
static int x, y, w, h, n;

static void Put(int i, int j, char c) {
    char *p = image + j * w + i;
    if (*p == ' ' || *p == '_')
        *p = c;
}

static void KochCurve(int n, int dir) {
    if (n == 0)
        switch (dir % 6) {
            case 0: Put(x++, y  , '_' );
                    Put(x++, y,   '_' ); break;
            case 1: Put(x++, y--, '/' ); break;
            case 2: Put(--x, y--, '\\'); break;
            case 3: Put(--x,   y, '_' );
                    Put(--x,   y, '_' ); break;
            case 4: Put(--x, ++y, '/' ); break;
            case 5: Put(x++, ++y, '\\'); break;
        }
    else {
        KochCurve(n - 1, dir    );
        KochCurve(n - 1, dir + 1);
        KochCurve(n - 1, dir + 5);
        KochCurve(n - 1, dir    );
    }
}


int main() {
    w = 25;
    h = 15;
	n = 2;
    image = (char*)malloc(w * h);
    memset(image, ' ', w * h);

    x = 5;
    y = h / 4 - 1;
    KochCurve(n, 0);
    KochCurve(n, 4);
    KochCurve(n, 2);

	printf("\e[H\e[2J");
	printf("\n");
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++)
            putchar(image[j * w + i]);
        putchar('\n');
    }

    w *= 3;
    h = n == 0 ? 4 : h * 3;
    free(image);
	printf("INLOW 0.0.2 SNOW\n\n");
}

/*Thanks for Milo Yip*/
