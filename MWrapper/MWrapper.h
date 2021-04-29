#define MatIdent(Size) _MATCREATE(Size, 1);
#define MatCreate(Size) _MATCREATE(Size, 0);
float *_MATCREATE(int V, int Mask);
