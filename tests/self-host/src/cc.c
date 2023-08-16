#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <parsing.h>

static void __attribute__((noreturn)) die(char **argv)
{
	fprintf(stderr,
			"Overview: C dialect parser\n"
			"Usage: %s [options] file...\n\n"
			"Options:\n"
			"  %-20s %s\n"
			"  %-20s %s\n"
			"  %-20s %s\n",
			argv[0], "-v", "Print version info.", "-h",
			"Print this help message.", "-o [FILE]",
			"Translate code to stdout. If FILE is '-', read stdin.");
	exit(EXIT_FAILURE);
}
static void
open_file_to_compile(const char *file)
{
	if (file == NULL) {
		exit(1);
	} else {
		enter_block_and_parse(file);
	}
}
int
main(int argc, char *argv[])
{
	/* read in user input */
	if (argc > 1) {
		if (argv[1][0] == '-') {
			int file = 0;
			switch (argv[1][1]) {
			case 'v':
				printf("%s (%s) version %s\n", _PACKAGE_NAME, LANGNAME_STRING,
					   _PACKAGE_VERSION);
				break;
			case 'o':
				if (argc > 3) {
					if (strstr(argv[3], ".cb") != NULL) {
						file = 3; // argv[2]
						set_LANGNAME_file();
					} else if (strstr(argv[3], ".c") != NULL) {
						file = 3;
					} else {
						die(argv);
					}
				} else if (argc > 2) {
					if (strstr(argv[2], ".cb") != NULL) {
						file = 2; // argv[2]
						set_LANGNAME_file();
					} else if (strstr(argv[2], ".c") != NULL) {
						file = 2;
					} else if (strcmp(argv[2], "-") == 0) {
						open_file_to_compile("stdin");
						break;
					} else {
						die(argv);
					}
				} else {
					die(argv);
				}
				open_file_to_compile(argv[file]);
				break;
			case 'f':
				file = 2;
				set_LANGNAME_file();
				open_file_to_compile(argv[file]);
				break;
			case 'h': /* fall through */
			default:
				die(argv);
			}
		}
	} else {
		die(argv);
	}
	return 0;
}
