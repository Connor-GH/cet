#include <common.h>

char is_any_of(char item, const char *list) {
    for (int i = 0; list[i] != '\0'; i++) {
        if (item == list[i]) return item;
    }
    return 0;
}
