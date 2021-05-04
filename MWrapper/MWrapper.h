#define MatIdent(Size) _MATCREATE(Size, 1);
#define MatCreate(Size) _MATCREATE(Size, 0);
#include <time.h>
float *_MATCREATE(int V, int Mask);
float calcDeltaTime(clock_t lastCPUcycle, clock_t curCPUcycle);
struct Loc2d{
	float x;
	float y;
};

struct Object{
	struct Loc2d velocity;
	struct Loc2d accel;
	int colground;
};
