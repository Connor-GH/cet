#include <stddef.h>
#include <ctype.h>
#include <tokens.h>
#include <string.h>

static char all_type_tokens[13][5] = {
    TOKEN_u8,	TOKEN_i8,	TOKEN_u16, TOKEN_i16,
	TOKEN_u32,	TOKEN_i32,	TOKEN_u64, TOKEN_i64,
	TOKEN_u128, TOKEN_i128, TOKEN_f32, TOKEN_f64,
	TOKEN_f128 };

/**
 * Returns the index of the first
 * byte of the first token in the
 * string.
 * Returns -1 if no token found.
 */
static size_t
type_info(const char *tok)
{
	_Bool is_unsigned = 0;
	size_t is_float = 0;
	size_t i = 0;
	for (i; i < strlen(tok); i++) {
		switch (tok[i]) {
		case 'i': {
			if (tok[++i]) {
				goto determine_type;
			}
			break;
		}
		case 'u': {
			if (tok[++i]) {
				is_unsigned = 1;
				goto determine_type;
			}
			break;
		}
		case 'f': {
			if (tok[++i]) {
				is_float = 314;
				goto determine_type;
			}
			break;
		}
		default:
			break;
		}
back_to:;
	}
	return 0;
determine_type:
	switch (tok[i]) {
	/* determine if a type is spotted -
     * i32, i64, i8, or i128. */
	case '3': {
		if (tok[++i])
			if (tok[i] == '2') {
				if (tok[++i])
					if (isspace(tok[i]) || !isalnum(tok[i]))
						return TYPE_32 + is_unsigned + is_float;
			}
		goto back_to;
	}
	case '6': {
		if (tok[++i])
			if (tok[i] == '4')
				if (tok[++i])
					if (isspace(tok[i]) || !isalnum(tok[i]))
						return TYPE_64 + is_unsigned + is_float;
		goto back_to;
	}
	case '8': {
		if (tok[++i])
			if (isspace(tok[i]) || !isalnum(tok[i]))
				return TYPE_8 + is_unsigned;
		goto back_to;
	}
	case '1': {
		if (tok[++i]) {
			if (tok[i] == '2') {
				if (tok[++i])
					if (tok[i] == '8')
						if (tok[++i])
							if (isspace(tok[i]) || !isalnum(tok[i]))
								return TYPE_128 + is_unsigned + is_float;
            } else if (tok[i] == '6') {
                if (tok[++i]) {
                    if (isspace(tok[i]) || !isalnum(tok[i]))
                        return TYPE_16 + is_unsigned;
                }
            } else {
                break;
            }
        }
		goto back_to;
	}
	default: {
		goto back_to;
	}
	}
    return 0;
}
static struct type_token_vals
token_information(const char *tok)
{
    struct type_token_vals info = {.token_location = 0, .preparsed_len = 0, .val = NULL};
	_Bool is_unsigned = 0;
	size_t is_float = 0;
	size_t i = 0;
    size_t type = 0;
	for (i; i < strlen(tok) - 1; i++) {
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
back_to:
        continue;
	}
	return info;
determine_type:
	switch (tok[i]) {
	/* determine if a type is spotted -
     * *32, *64, *8, *16, or *128. */
	case '3': {
		if (tok[++i])
			if (tok[i] == '2')
				if (tok[++i])
					if (isspace(tok[i]) || !isalnum(tok[i])) {
                        type = TYPE_32 + is_unsigned + is_float;
                        info.preparsed_len = 3;
                        info.token_location = i - info.preparsed_len;
                        switch (type) {
                        case TYPE_32: info.val = "int32_t"; break;
                        case TYPE_32_U: info.val = "uint32_t"; break;
                        case TYPE_32_F: info.val = "_Float32"; break;
                        default: break;
                        }
                        return info;
                    }
		goto back_to;
	}
	case '6': {
		if (tok[++i])
			if (tok[i] == '4')
				if (tok[++i])
					if (isspace(tok[i]) || !isalnum(tok[i])) {
                        type = TYPE_64 + is_unsigned + is_float;
                        info.preparsed_len = 3;
                        info.token_location = i - info.preparsed_len;
                        switch (type) {
                        case TYPE_64: info.val = "int64_t"; break;
                        case TYPE_64_U: info.val = "uint64_t"; break;
                        case TYPE_64_F: info.val = "_Float64"; break;
                        default: break;
                        }
                        return info;
                        }
		goto back_to;
	}
	case '8': {
		if (tok[++i])
			if (isspace(tok[i]) || !isalnum(tok[i])) {
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
						if (tok[++i])
							if (isspace(tok[i]) || !isalnum(tok[i])) {
                                type = TYPE_128 + is_unsigned +
                                    is_float;
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
                                    /* Architecture-dependant
                                     * as to whether this
                                     * even exists. */
                                    info.val = "__float128";
                                    break;
                                default: break;
                                }
                                return info;
                            }
            } else if (tok[i] == '6') {
				if (tok[++i])
					if (isspace(tok[i]) || isalnum(tok[i])) {
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
 * If a type token is found, returns
 * the index of the first byte of it in the string.
 * Else, returns -1.
 */
size_t
is_there_token(const char *tok)
{
	if (tok == NULL)
		return 0;
	return type_info(tok);
}
struct type_token_vals type_token_info(const char *tok) {
    if (tok != NULL)
        return token_information(tok);
    else
        return (struct type_token_vals){.token_location = 0,
            .preparsed_len = 0, .val = NULL};
}
