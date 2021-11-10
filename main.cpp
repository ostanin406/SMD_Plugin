#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include "smd.h"

void Error(char* error, ...)
{
	va_list arg;
	va_start(arg, error);
	vprintf(error, arg);
	va_end(arg);

	system("pause");
	exit(1);
}

int main(int argc, char* argv[])
{
	CSmd smd;

	if (smd.Load(argv[1]) == 0)
		Error("[ERROR] open file: %s\n", argv[1]);

	if (smd.Save("123.smd") == 0)
		Error("[ERROR] save file: %s\n", argv[1]);

	printf("Done.\n");

	system("pause");
	return 0;
}