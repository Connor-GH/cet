#ifndef DEBUG_COMMON_H
#define DEBUG_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
#define DBG(...) \
	printf("\033[1m**\033[1;31mDEBUG\033[0m\033[1m**: \033[0m" __VA_ARGS__)
#ifdef __cplusplus
}
#endif

#endif
