#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "calc.h"

#define MAXLEN 1024

rpn *makerpn(char *in)
{
	char num[MAXLEN] = "";

	double stack_num[MAXLEN];
	char stack_sym[MAXLEN];

	rpn *ret = malloc(sizeof(rpn)*MAXLEN);
	int j=0;

	int s=0, n=0;

	int last = 0;

	for (int i=0; i<strlen(in); i++) {
		if (in[i] >= '0' && in[i] <= '9' || in[i] =='.') {
			strncat(num, &in[i], 1);
		} else if (tolower(in[i]) >= 'a' && tolower(in[i]) <= 'z' ||
				strchr("+-*/^", in[i])) {
			if (strlen(num)>0) {
				ret[j].sym = '#';
				ret[j].x = atof(num);
				ret[j].y = 0;
				num[0] = 0;

				printf("%lf\n", ret[j].x);

				j++;
			}

			switch (in[i]) {
			case '+': case '-':
				if (s > 0 && strchr("+-*/^", stack_sym[s-1]))
					ret[j++].sym = stack_sym[--s];

				stack_sym[s++] = in[i];
				break;
			case '*': case '/':
				if (s > 0 && strchr("*/^", stack_sym[s-1]))
					ret[j++].sym = stack_sym[--s];

				stack_sym[s++] = in[i];
				break;
			case '^':
				if (s > 0 && strchr("^", stack_sym[s-1]))
					ret[j++].sym = stack_sym[--s];

				stack_sym[s++] = in[i];
				break;
			case '(':
				stack_sym[s++] = in[i];
				break;
			case ')':
				while (s > 0)
					if (stack_sym[--s] != '(') ret[j++].sym = stack_sym[s]; else break;
				break;
			}
		}
	}

	if (strlen(num)>0) {
		ret[j].sym = '#';
		ret[j].x = atof(num);
		ret[j].y = 0;
		num[0] = 0;
		j++;
	}

	//s--;
	while (s > 0) ret[j++].sym = stack_sym[--s];

	ret[j].sym = 0;

	printf("***\n");
	return ret;
}

rpn *evalrpn(rpn *out, double x, double y, double z, double t, double u, double v)
{
	rpn *ret = malloc(sizeof(rpn));

	int j=0;

	while (out[j].sym != 0) {
		if (out[j].sym == '#') {
			printf("%lf + i%lf\n", out[j].x, out[j].y);
		} else {
			printf("%c\n", out[j].sym);
		}
		j++;
	}

	ret->x = 3.1;
	ret->y = 12;
	return ret;
}

