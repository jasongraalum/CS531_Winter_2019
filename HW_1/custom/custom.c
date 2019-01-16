#include <stdio.h>
#include <stdlib.h>
int funcA(int);
int funcB(int);

void loop(int c)
{
	int x = 0;
	while (c--) { x++; }
	return;
}

int funcA(int a) {
	if (a > 0) {
		loop(a);
		return funcB(--a);
	}
	else
		return 0;
}

int funcB(int b) {
	if (b > 0) {
		loop(b);
		return funcA(--b);
	}
	else
		return 0;
}

int main() {

	int count = 100;
	printf("Enter loop count: ");
	scanf("%d", &count);
	funcA(count);

	return 0;

}

