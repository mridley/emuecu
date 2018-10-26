#include <stdarg.h>
#include <stdio.h>

int logmsgf(const char *fmt, ...)
{
	va_list ap;
	int i;

    fprintf(stdout, "{\"log\":{\"msg\":\"");
	va_start(ap, fmt);
	i = vfprintf(stdout, fmt, ap);
	va_end(ap);
    fprintf(stdout, "\"}}\n");

	return i;
}

int dump_array_int16_t(const char* name, int16_t* array, uint8_t n)
{
  int c = printf("\"%s\":[", name);
  for (uint8_t i=0; i < n;) {
    c += printf("%d", array[i]);
    if (++i < n) {
      printf(",");
    }
  }
  printf("]");
  return c;
}
