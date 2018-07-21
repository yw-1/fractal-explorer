//z = x + iy
typedef struct rpnstruct {
	char sym;
	double x;
	double y;
} rpn;

rpn *makerpn(char *in);
rpn *evalrpn(rpn *out, double x, double y, double z, double t, double u, double v);
