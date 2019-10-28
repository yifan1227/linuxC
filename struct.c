#include <stdio.h>
#include <stdlib.h>

typedef struct s{
	int (*add)(int, int);
	int (*sub)(int, int);
}s_t;

int add(int a, int b)
{
	return a + b;
}

int sub(int a, int b)
{
	return a - b;
}

struct s s2;

void main()
{
/*	struct s s1 = {
		.add = add,
		.sub = sub
	    };
	struct s s1 = {
		add,
		sub
	};	
	struct s *p = &s2;
	p -> add = add;
	p -> sub = sub;
*/
//	printf("%d\n", sizeof(s_t *));
	s_t *S = (s_t *)malloc(sizeof(s_t)*3);
	for(int i = 0; i < 3; i++)
	{
//		printf("OK here");
//		S[i] -> add = add;
//		S[i] -> sub = sub;
		S[i].add = add;
		S[i].sub = sub;
	}


	printf("%d\n", S[0].add(1, 2));
	printf("%d\n", S[1].sub(3, 5));
}
