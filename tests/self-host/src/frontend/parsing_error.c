#include <stdio.h>
#include <stdlib.h>

#include "parsing_error.h"

void
parsing_err(char warning_level, const char *message, const char *buffer,
			size_t problematic_index, const char *file, size_t line_number)
{
	if (warning_level == ERROR) {
		fprintf(stderr, "%s:%lu:%lu:" RED "error:" CLR " %s\n", file,
				line_number, problematic_index, message);
		fprintf(stderr, "%s\n", buffer);
		while (problematic_index > 0) {
			fputc(' ', stderr);
			problematic_index--;
		}
		fprintf(stderr, RED "^\n" CLR);
		exit(EXIT_FAILURE);
	} else if (warning_level == WARNING) {
		fprintf(stderr, "%s:%lu:%lu:" MAGENTA "warning:" CLR " %s\n", file,
				line_number, problematic_index, message);
		fprintf(stderr, "%s\n", buffer);
		while (problematic_index > 0) {
			fputc(' ', stderr);
			problematic_index--;
		}
		fprintf(stderr, MAGENTA "^\n" CLR);
	}
}
