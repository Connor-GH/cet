#ifndef PARSING_ERROR_H
#define PARSING_ERROR_H
#ifdef __cplusplus
extern "C" {
#include <cstddef>
#else
#include <stddef.h>
#endif
#define ERROR 127
#define WARNING 1
#define RED "\033[1;31m"
#define MAGENTA "\033[1;35m"
#define WHITE "\033[1m"
#define CLR "\033[0m"
void
parsing_err(char warning_level, const char *message, const char *buffer,
			size_t problematic_index, const char *file, size_t line_number);

#ifdef __cplusplus
}
#endif
#endif
