
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
clock_t clock(void);

// List some plausible cache sizes
size_t sz[] = {
    1024,
    2048,
    2048 + 1024,
    4096,
    4096 + 1024,
    4096 + 2048,
    4096 + 2048 + 1024,
    8192,
    8192 + 2048,
    8192 + 4096,
    8192 + 4096 + 2048,
   16384,
   16384 + 8192,
   32768,
   32768 + 16384,
   65536,
   65536 + 32768,
  131072,
  131072 + 65536,
  262144,
  262144 + 131072,
  524288,
 1048576,
 1048576 + 524288,
 2097152,
 2097152 + 524288,
 2097152 + 1048576,
 2097152 + 1048576 + 524288,
 4194304,
 4194304 + 1048576,
 4194304 + 2097152,
 4194304 + 2097152 + 1048576,
 8388608,
 8388608 + 2097152,
 8388608 + 4194304,
 8388608 + 4194304 + 2097152,
 14680064 + 8388608,
 14680064 + 4194304+ 8388608,
};

// A conservative alignment for the cache buffers
#define CACHE_ALIGN (1<<10)
#define MAX_SIZE (4000000)

int
main(int argc, char* argv[])
{
	int print_flag = 0; 
    size_t memrefs = argc>1 ? atol(argv[1]) : 100000000;
    size_t lo = 1024;
    size_t hi = 16*1024*1024;
	long old_diff = 100000;
    for (int i = 0; i<sizeof(sz)/sizeof(sz[0]); i++) {

        size_t s = sz[i];

        // over-allocate to permit alignment
        char *buffer = (char*)malloc(s + CACHE_ALIGN);
        //uint8_t buffer[MAX_SIZE];
        // clear the bottom bits of the address to force alignment
        char *p = (char*)( (intptr_t)buffer & ~(CACHE_ALIGN-1) );

        // warm the cache line under test
        for (int i=0; i<s; i++) {
            p[i] = 0;
        }

        // run the test for lots of memory references
        clock_t start = clock();
        int ndx = s;
        for (int i=0; i<memrefs; i++) {
            ndx -= 37;
            if (ndx<0)
                ndx += s;
            p[ndx] = i;
        }
        long diff  = clock() - start;
       	printf("For size %8ld, time diff = %8ld\n", s, diff);
		float percent = (float) diff/(old_diff+1);
		if (percent > 1.5) {
        	//printf("Diff multiplier = %lf\n", (float) diff/(old_diff+1));
			print_flag = 1;
		}
		if (print_flag){
        	printf("Cache size< %8ld\n", s);
			printf("meow");
			print_flag = 0;
		}

		old_diff = diff;
    }
	printf("Now, now, now, now once upon a time in the west. Mad man lost his damn mind in the west Loveless, givin up a dime, nothin' less Now I must put his behind to the test (can you feel me) Then through the shadows, in the saddle, ready for battle Bring all your boys in, here come the poison Behind my back, all the riffin' ya did, Front and center, now where your lip at kid?  Who dat is? A mean brotha, bad for your health Lookin damn good though, if I could say it myself Told me Loveless is a mad man, but I don't fear that Got mad weapons too, ain't tryin to hear that Tryin to bring down me, this champion When y'all clowns gon' see that it can't be done Understand me son, I'm the slickest they is, I'm the quickest as they is, did I say I'm the slickest they is So if you barking up the wrong tree we comin, don't be startin nothin Me and my partner gonna test your chest, Loveless Can't stand the heat then get out the Wild Wild West. With a cat that says 'meow meow mow'\n"); return 0;
}
