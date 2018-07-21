#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calc.h"

rpn makerpn(char *in)
{
	rpn ret;

	int i;
	char out[1000];
	int o=0;
	char stack[1000];
	int s=0;
	char num[1000] = "";
	double nums[1000];
	int n=0;

	int last = 1;

	for(i = 0; i <= strlen(in); i++) {
		if((in[i] >= '0' && in[i] <= '9') || in[i] == '.') {
			strncat(num, &in[i], 1);
			continue;
		}
		else if(strlen(num) > 0) {
			sscanf(num, "%lf", &nums[n++]);
			out[o++] = '#';
			num[0] = 0;
			last = 0;
		}

		if(in[i] >= 'a' && in[i] <= 'z') {
			/* functions */
			if(!memcmp("sin", in+i, 3)) {      stack[s++] = 's'; i+=2; }
			else if(!memcmp("cos", in+i, 3)) { stack[s++] = 'c'; i+=2; }
			else if(!memcmp("tan", in+i, 3)) { stack[s++] = 'j'; i+=2; }
			else if(!memcmp("abs", in+i, 3)) { stack[s++] = 'a'; i+=2; }
			else if(!memcmp("sqrt", in+i, 4)){ stack[s++] = 'r'; i+=3; }
			else if(!memcmp("ln", in+i, 2))  { stack[s++] = 'l'; i+=1; }

			/* variables & constants */
			else if(strchr("xyztuv", in[i])) { out[o++] = in[i]; last = 0; }
			else if(in[i] == 'e') { out[o++] = 'e'; last = 0; }
			else if(!memcmp("pi", in+i, 2)) { out[o++] = 'p'; last = 0; }
		}
		else if(strchr("+-", in[i])) {
			if(last) {
				if(in[i] == '-') stack[s++] = '_';	
			}else{
				if(s > 0 && strchr("_+-*/^scjarl", stack[s-1]))
					out[o++] = stack[--s];

				stack[s++] = in[i];
				last = 1;
			}
		}
		else if(strchr("*/", in[i])) {
			if(s > 0 && strchr("_*/^scjarl", stack[s-1]))
				out[o++] = stack[--s];

			stack[s++] = in[i];
			last = 1;
		}
		else if(in[i] == '^') {
			if(s > 0 && strchr("_^scjarl", stack[s-1]))
				out[o++] = stack[--s];

			stack[s++] = in[i];
			last = 1;
		}
		else if(in[i] == '(') {
			stack[s++] = in[i];
		}
		else if(in[i] == ')') {
			while(s > 0) {
				if(stack[--s] != '(') {
					out[o++] = stack[s];
				}
				else{
					break;
				}
			}
		}
	}

	--s;
	while(s > 0) out[o++] = stack[--s];
	out[o] = 0;

	ret.syms = malloc(o+1);
	strncpy(ret.syms, out, o);

	ret.nums = malloc(n * sizeof(double));
	memcpy(ret.nums, nums, n * sizeof(double));

	return ret;
}

double evalrpn(rpn outx, double x, double y, double z, double t, double u, double v)
{
	char *out = outx.syms;
	double *nums = outx.nums;
	int o = strlen(out);

	double numstack[1000];
	int ns=0;
	int n =0;
	int i;

	for(i=0; i < o; i++) {
		switch(out[i]) {
			case '#': numstack[ns++] = nums[n++]; break;

			/* variables & constants */
			case 'x': numstack[ns++] = x; break;
			case 'y': numstack[ns++] = y; break;
			case 'z': numstack[ns++] = z; break;
			case 't': numstack[ns++] = t; break;
			case 'u': numstack[ns++] = u; break;
			case 'v': numstack[ns++] = v; break;
			case 'p': numstack[ns++] = M_PI; break;
			case 'e': numstack[ns++] = M_E; break;

			/* operators */
			case '_': numstack[ns-1] = -numstack[ns-1]; break;
			case '+': numstack[ns-2] += numstack[ns-1];	ns--; break;
			case '-': numstack[ns-2] -= numstack[ns-1];	ns--; break;
			case '*': numstack[ns-2] *= numstack[ns-1];	ns--; break;
			case '/': numstack[ns-2] /= numstack[ns-1];	ns--; break;
			case '^': numstack[ns-2] = pow(numstack[ns-2], numstack[ns-1]);	ns--; break;

			/* functions */
			case 's': numstack[ns-1] = sin(numstack[ns-1]); break;
			case 'c': numstack[ns-1] = cos(numstack[ns-1]); break;
			case 'j': numstack[ns-1] = tan(numstack[ns-1]); break;
			case 'l': numstack[ns-1] = log(numstack[ns-1]); break;
			case 'a': numstack[ns-1] = fabs(numstack[ns-1]); break;
			case 'r': numstack[ns-1] = sqrt(numstack[ns-1]); break;
		}
	}

	return numstack[0];
}

