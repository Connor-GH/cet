#include <string>
#include <fstream>
#include <iostream>

#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "tokens.hpp"
#include "parsing.h"
#include "parsing_error.h"
#include "classes.hpp"

static i32 is_in_comment = 0;
static i32 in_codeblock = 0;
static size_t line_no = 0;


static constexpr const char *begin_comment = "/" "*";
static constexpr const char *end_comment = "*" "/";

void set_LANGNAME_file(void) {
	in_codeblock = 1;
}

static std::string
remove_portion_of_buffer_and_merge(std::string initbuf, size_t start,
								   size_t end)
{
	std::string start_portion, end_portion;

	if ((initbuf.length() < start) || (initbuf.length() < end))
		return initbuf;
	if ((start == 0) && (end == initbuf.length())) {
		initbuf.clear();
		return initbuf;
	}

	start_portion = initbuf.substr(0, (initbuf.find(begin_comment) != std::string::npos) ? start - 1 : start);

	end_portion = initbuf.substr(end + 1, initbuf.length() - 1);
	initbuf.clear();
	initbuf = start_portion + end_portion;

	return initbuf;
}

static size_t
has_end_comment(std::string buf, const char *file)
{
	size_t idx_of_end_comment = 0;
	if (buf.empty())
		return buf.length()+1;

	idx_of_end_comment = buf.find(end_comment);
	if (buf.rfind(end_comment) != idx_of_end_comment)
		parsing_err(ERROR, "Nested comments are not supported.", buf.c_str(),
					buf.rfind(end_comment), file, line_no);
	if (idx_of_end_comment != std::string::npos) {
		is_in_comment = 0;
		return idx_of_end_comment + 1;
	}
	return buf.length()+1;
}
static size_t
has_begin_comment(std::string buf, const char *file)
{
	size_t idx_of_begin_comment = 0;
	if (buf.empty())
		return buf.length()+1;

	idx_of_begin_comment = buf.find(begin_comment);
	if (buf.rfind(begin_comment) != idx_of_begin_comment)
		parsing_err(ERROR, "Nested comments are not supported.", buf.c_str(),
					buf.rfind(begin_comment), file, line_no);
	if (idx_of_begin_comment != std::string::npos) {
		is_in_comment = 1;
		return idx_of_begin_comment + 1;
	}
	return buf.length()+1;
}

static i32
has_LANGNAME_begin_block(std::string buf, const char *file)
{
	std::string tmp = "<<" LANGNAME_STRING "_begin>>";
	if (in_codeblock && (buf.find(tmp) != std::string::npos))
		parsing_err(WARNING, "Duplicate begin block", buf.c_str(),
				buf.find(tmp), file, line_no);

	if (buf.empty())
		return 0;
	if (buf.find(tmp) != std::string::npos) {
		return 1;
	} else {
		return 0;
	}
}
static i32
has_LANGNAME_end_block(std::string buf, const char *file)
{
	std::string tmp = "<<" LANGNAME_STRING "_end>>";
	if (!in_codeblock && (buf.find(tmp) != std::string::npos))
		parsing_err(WARNING, "Duplicate end block", buf.c_str(),
				buf.find(tmp), file, line_no);

	if (buf.empty())
		return 0;
	if (buf.find(tmp) != std::string::npos)
		return 1;
	else
		return 0;
}

static bool
has_LANGNAME_type(std::string buf)
{
	if (buf.empty())
		return false;
	return type_token_info(buf).val != "";
}

static std::string
parse_type_token(std::string tok)
{
	std::string replaced_type_token;
	struct type_token_vals vals = type_token_info(tok);

	tok = remove_portion_of_buffer_and_merge(tok, vals.token_location,
			vals.token_location-1 + vals.preparsed_len);
	replaced_type_token =
		tok.substr(0, vals.token_location) +
		vals.val + tok.substr(vals.token_location);
	return replaced_type_token;
}


void
enter_block_and_parse(const char *file)
{
	std::ifstream fp;
	size_t start = 0, end = 0;
	std::string initbuf;
	std::string tmp_buf;
	i32 error_num;
	if (strcmp(file, "stdin") != 0) {
		fp.open(file);
		if (!fp.is_open()) {
			error_num = errno;
			std::cerr << "failed to open " << file << ": "
				<< strerror(error_num) << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}
	while (std::getline((strcmp(file, "stdin") == 0) ? std::cin : fp, initbuf)) {
		line_no++;
		if (has_LANGNAME_end_block(initbuf, file) == 1) {
			in_codeblock = 0;
			continue;
		}
		if (has_LANGNAME_begin_block(initbuf, file) == 1) {
			in_codeblock = 1;
			continue;
		}
		if (in_codeblock) {
			start = has_begin_comment(initbuf, file);
			end = has_end_comment(initbuf, file);

			/* check for classes */

			// should only be used for debug, is being worked on
			/*if ((has_class_identifier(initbuf, file, line_no) < initbuf.length()) && !is_in_comment) {
				std::cout << "Class found!\n";
				locate_class_name(initbuf, fp, file, line_no);
				continue;
			}*/

			/* parse comments */
			if (start < initbuf.length() && end < initbuf.length()) {
				initbuf =
					remove_portion_of_buffer_and_merge(initbuf, start, end);
			} else if (start < initbuf.length() && !(end < initbuf.length())) {
				initbuf = remove_portion_of_buffer_and_merge(
					initbuf, start, initbuf.length() - 1);
			} else if (!(start < initbuf.length()) && end < initbuf.length()) {
				initbuf = remove_portion_of_buffer_and_merge(initbuf, 0,
						end);
		   } else if (is_in_comment) {
				initbuf = remove_portion_of_buffer_and_merge(
				initbuf, 0, initbuf.length() - 1);
			} else if (!(start < initbuf.length()) && !(end < initbuf.length())) {
					if (!has_LANGNAME_begin_block(initbuf, file) &&
							!has_LANGNAME_type(initbuf)) {
						std::cout << initbuf << std::endl;
						continue;
					}
			}

			/* parse type info */
			if (has_LANGNAME_type(initbuf) && !is_in_comment) {
				/* While loop to change types.
				 * Time complexity: Linear. The entire
				 * buffer has to be read every time a type
				 * is (still) seen inside of it. */
				do {
					tmp_buf = parse_type_token(initbuf);
					initbuf.clear();
					initbuf = tmp_buf.substr(0);
				} while (has_LANGNAME_type(initbuf));

				std::cout << initbuf << std::endl;
				continue;
			}
		}
		if (!in_codeblock && has_LANGNAME_begin_block(initbuf, file) != 1) {
			std::cout << initbuf << std::endl;
		}
	}
	if (strcmp(file, "stdin") != 0) {
		fp.close();
	}
}
