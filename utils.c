#include <ctype.h>
#include <string.h>

// custom comparator to sort filenames 
int filename_comparator(const void *a, const void *b) {
    const char *name1 = *(const char **)a;
    const char *name2 = *(const char **)b;

    char c1 = name1[0];
    char c2 = name2[0];

    int c1_is_digit = isdigit((unsigned char)c1);
    int c2_is_digit = isdigit((unsigned char)c2);

    if (c1_is_digit && !c2_is_digit) return -1;
    if (!c1_is_digit && c2_is_digit) return 1;

    if (c1_is_digit && c2_is_digit) {
        return strcmp(name1, name2);
    }

    char c1_upper = toupper((unsigned char)c1);
    char c2_upper = toupper((unsigned char)c2);

    if (c1_upper != c2_upper)
        return (c1_upper < c2_upper) ? -1 : 1;

    if (c1 != c2) {
        if (isupper((unsigned char)c1) && islower((unsigned char)c2))
            return -1;
        if (islower((unsigned char)c1) && isupper((unsigned char)c2))
            return 1;
    }

    return strcmp(name1, name2);
}

int is_alphanumeric(char c) {
    return ( (c >= 'A' && c <= 'Z') ||
             (c >= 'a' && c <= 'z') ||
             (c >= '0' && c <= '9') );
}
