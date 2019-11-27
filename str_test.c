#include <stdio.h>

int main()
{
	char msg1[] = "hello";		// Equals to char msg1[] = {'h', 'e', 'l', 'l', 'o'};
	char *msg2 = "hello";

	msg1[1] = 'o';
	printf("%s\n", msg1);
	printf("%s\n", msg2);
	printf("the last character of the string is %d\n", msg1[5]);
	*(msg2 + 1) = 'o';	// Error here, "hello" is a const string which can't be modified.
	return 1;
}
