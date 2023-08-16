#include <string>
#include <iostream>

#include <cstdlib>
#include <cstddef>
#include <cctype>
#include <cstring>

#include "tokens.hpp"
/**
 * Returns the index of the first
 * byte of the first token in the
 * string.
 */


static i32 istypetoken(unsigned char c) {
	return isspace(c) || (c == '>') || (c == ')');
}
static struct type_token_vals
token_information(std::string tok)
{
	struct type_token_vals info = { 0, 0, ""};
	size_t is_unsigned = 0;
	size_t is_float = 0;
	size_t i = 0;
	size_t type = 0;
	while (i < tok.length()) {
		switch (tok[i]) {
		case 'i': {
			if (tok[++i]) {
				goto determine_type;
			}
			break;
		}
		case 'u': {
			if (tok[++i]) {
				is_unsigned = INTERNAL_TOKEN_UNSIGNED_OFFSET;
				goto determine_type;
			}
			break;
		}
		case 'f': {
			if (tok[++i]) {
				is_float = INTERNAL_TOKEN_FLOAT_OFFSET;
				goto determine_type;
			}
			break;
		}
		default:
			break;
		}
back_to:;
		i++;
	}
	return info;
determine_type:
	switch (tok[i]) {
	/* determine if a type is spotted -
	 * *32, *64, *8, *16, or *128. */
	case '3': {
		if (tok[++i])
			if (tok[i] == '2')
				if (!tok[++i] || istypetoken(tok[i])) {
					type = TYPE_32 + is_unsigned + is_float;
					info.preparsed_len = 3;
					info.token_location = i - info.preparsed_len;
					switch (type) {
					case TYPE_32:
						info.val = "int32_t";
						break;
					case TYPE_32_U:
						info.val = "uint32_t";
						break;
					case TYPE_32_F:
						info.val = "_Float32";
						break;
					default:
						break;
					}
					return info;
				}
		goto back_to;
	}
	case '6': {
		if (tok[++i])
			if (tok[i] == '4')
				if (!tok[++i] || istypetoken(tok[i])) {
					type = TYPE_64 + is_unsigned + is_float;
					info.preparsed_len = 3;
					info.token_location = i - info.preparsed_len;
					switch (type) {
					case TYPE_64:
						info.val = "int64_t";
						break;
					case TYPE_64_U:
						info.val = "uint64_t";
						break;
					case TYPE_64_F:
						info.val = "_Float64";
						break;
					default:
						break;
					}
					return info;
				}
		goto back_to;
	}
	case '8': {
		if (!tok[++i] || istypetoken(tok[i])) {
			type = TYPE_8 + is_unsigned;
			info.preparsed_len = 2;
			info.token_location = i - info.preparsed_len;
			info.val = (type == TYPE_8) ? "int8_t" : "uint8_t";
			return info;
		}
		goto back_to;
	}
	case '1': {
		if (tok[++i]) {
			if (tok[i] == '2') {
				if (tok[++i])
					if (tok[i] == '8')
						if (!tok[++i] || istypetoken(tok[i])) {
							type = TYPE_128 + is_unsigned + is_float;
							info.preparsed_len = 4;
							info.token_location = i - info.preparsed_len;
							switch (type) {
							case TYPE_128:
								info.val = "__int128";
								break;
							case TYPE_128_U:
								info.val = "__uint128_t";
								break;
							case TYPE_128_F:
								/* Architecture-dependent
								 * as to whether this
								 * even exists. */
								info.val = "__float128";
								break;
							default:
								break;
							}
							return info;
						}
			} else if (tok[i] == '6') {
				if (!tok[++i] || istypetoken(tok[i])) {
					type = TYPE_16 + is_unsigned;
					info.preparsed_len = 3;
					info.token_location = i - info.preparsed_len;
					info.val = (type == TYPE_16) ? "int16_t" : "uint16_t";
					return info;
				}
			}
		}
		goto back_to;
	}
	default: {
		/* Reset these variables
		 * if set due to previous
		 * iterations. */
		is_float = 0;
		is_unsigned = 0;
		goto back_to;
	}
	}
}
/**
 * If a type token is found, returns:
 * - its length
 * - its location in the buffer
 * - its new "to replace with" string
 */
struct type_token_vals
type_token_info(std::string tok)
{
	if (!tok.empty()) {
		return token_information(tok);
	} else {
		struct type_token_vals fail = {0, 0, ""};
		return fail;
	}
}
