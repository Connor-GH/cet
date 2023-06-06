#ifndef TOKENS_H
#define TOKENS_H
#include <stddef.h>
#ifdef __cplusplus
#include <cstddef>
extern "C" {
#else
#include <stddef.h>
#endif
#define TOKEN_u8 "u8"
#define TOKEN_i8 "i8"
#define TOKEN_u16 "u16"
#define TOKEN_i16 "i16"
#define TOKEN_u32 "u32"
#define TOKEN_i32 "i32"
#define TOKEN_u64 "u64"
#define TOKEN_i64 "i64"
#define TOKEN_u128 "u128"
#define TOKEN_i128 "i128"
#define TOKEN_f32 "f32"
#define TOKEN_f64 "f64"
#define TOKEN_f128 "f128"

#define INTERNAL_TOKEN_UNSIGNED_OFFSET 1
#define INTERNAL_TOKEN_FLOAT_OFFSET 314

#define TYPE_8 (8)
#define TYPE_8_U (TYPE_8 + INTERNAL_TOKEN_UNSIGNED_OFFSET)
#define TYPE_16 (16)
#define TYPE_16_U (TYPE_16 + INTERNAL_TOKEN_UNSIGNED_OFFSET)
#define TYPE_32 (32)
#define TYPE_32_U (TYPE_32 + INTERNAL_TOKEN_UNSIGNED_OFFSET)
#define TYPE_32_F (TYPE_32 + INTERNAL_TOKEN_FLOAT_OFFSET)
#define TYPE_64 (64)
#define TYPE_64_U (TYPE_64 + INTERNAL_TOKEN_UNSIGNED_OFFSET)
#define TYPE_64_F (TYPE_64 + INTERNAL_TOKEN_FLOAT_OFFSET)
#define TYPE_128 (128)
#define TYPE_128_U (TYPE_128 + INTERNAL_TOKEN_UNSIGNED_OFFSET)
#define TYPE_128_F (TYPE_128 + INTERNAL_TOKEN_FLOAT_OFFSET)

struct type_token_vals {
	size_t token_location;
	size_t preparsed_len;
	char *val;
};
size_t
is_there_token(const char *tok);
struct type_token_vals
type_token_info(const char *tok);
#ifdef __cplusplus
}
#endif
#endif
