#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void insertion_sort_int(int *a, int n) {
    int i,j,t;

    for(i = 1; i < n; i++) {
        for(t = a[i], j = i - 1; j > -1 && a[j] > t; j--) {
            a[j + 1] = a[j];
        }
        a[j + 1] = t;
    }
}

/*void pointers (void *) are pointers that can point to anything, the compiler doesn't know what they actually point to; this means that the compiler will NOT allow you to
 dereference them, as it has ZERO clue what the size of the variable would be. It falls entirely on the programmer to keep track of these void * references and to assign them
  properly. */
// p is pointer to data, s size of element in p, n is number of elements, compare is a comparitor of elements of p.
void insertion_sort(void *p, int s, int n, int (*compare)(const void *, const void*)) {
    int i,j;
    void *t; // t needs to point to literally anything, so void pointer.
    char *a = p; // use char because it is single-byte addressable,
                 // if we were to use int or similar, we would only be able to sort types with sizes that are multiples of the variable size, eg. int would be sizeof(int) bytes.
    
    t = malloc(s);

    for(i = 1; i < n; i++) {
        // a + i * s -- address of a plus i * sizeof each element is index
        // of ith element in a.
        for(memcpy(t, a + i * s, s), j = i - 1; j > -1 && compare(a + j * s, t) > 0; j--) {
            memcpy(a + (j + 1) * s,a + j * s, s);
        }
        memcpy(a + (j + 1) * s, t, s);
    }
    free(t);
}

int compare_int(const void *v1,const void *v2) {
    return *((int *) v1) - *((int *) v2);
}

int compare_char(const void *v1,const void *v2) {
    return *((char *) v1) - *((char *) v2);
}

int compare_string(const void *v1,const void *v2) {
    return strcmp(*((char **) v1), *((char **) v2));
}

int main(int argc, char *argv[]) {
    int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    char s[] = "aowdkopawkd89ujiosejfio839m";
    char numbers[] = {
        "one"
        "two"
        "three"
        "four"
        "five"
        "six"
    };

    printf("before: %s", s);

    printf("\n");
    //insertion_sort(s, sizeof(s[0]) ,sizeof(s)/sizeof(s[0]) - 1, compare_char);
    printf("after: %s\n", s);

    qsort(numbers, sizeof(numbers) / sizeof(numbers[0]), sizeof(numbers[0]), compare_string);
    printf("%s",numbers);

    return 0;
}