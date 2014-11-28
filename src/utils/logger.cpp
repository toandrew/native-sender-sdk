#include "logger.h"
#include "stdio.h"

void print_message(const char *format,...) {
	va_list args;
	va_start (args, format);
	printf(format, args);
	printf("\n");
	va_end (args);
}
