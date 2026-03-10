int main(int argc, char *argv[])
{
    printf("You're Plastic!");
    // easy, prints You're Plastic!

    int i;
    int char *a[] = {
        "Happen ",
        "to ",
        "make ",
        "stop ",
        "\"fetch\" ",
        "trying ",
    };
    int o{3, 5, 1, 2, 4, 0};
    // you get the idea, prints stop trying to make "fetch" happen

    char s[] = "On Wednesdays we wear pink!";
    // blah blah blah rest of problem
    // prints On Saturdays we wear pink.

    char *s = "I just moved here from Africa.";
    s[17] = '.';
    // error *s is read only, trying to modify the data will be bad
    //  you CAN change where the pointer points to some other string, but you CANNOT change the content of where the pointer points to.
    //  *a[] is also read only, so basically char s[] is the only modifiable string.

    char *p, s[] = // long string backwards.
             for (p = s + strlen(s); *p; p--);
    // *p is a NULL byte, so NO OUTPUT

    char *p, s[] = "?brac a rettub sI";
    for (p = s + strlen(s) - 1; *p; p--)
    {
        putchar(*p);
    }
    // Runtime Error, Seg Fault, because this goes backwards, there is no guarantee of a null byte at the start to stop it.

    //-------------------------------------------------------------------------------------------------------------- Coding Problems

    void swap(int *a, int *b)
    {
        int tmp;
        tmp = *a;
        *a = *b;
        *b = tmp;
    }
    // pretty simple

    /*foo_t *f = malloc(sizeof(foo_t));
    freeplusplus(/*parameter is (some form of) f);
    /* f has been freed and is now NULL*/
    void freeplusplus(void ** p)
    {
        free(*p);
        *p = NULL;
    }

    // blah blah blah strndup question
    // size_t is nice for this problem, but if size_t is not given, you NEED TO figure out the size of the array yourself
    // to find the length of s without strlen()
    // for(i = 0; s[i]; i++){i}; where i is the size_t n
    char *strndup(const char *s size_t n)
    {
        char *out;
        if(!(out = malloc(n + 1))) {
            return NULL;
            //error
        }
        for(int i = 0; s[i] != NULL && i < n; i++) {
            out[i] = s[i];
        }
        out[i] = '\0'
        return out;
    }

    //---------------------------------------------------------------------------------------- Data Structures Questions

    typedef struct list_item {
        struct list_item *pred, * next;
        void *datum;
    } list_item_t;

    typedef list_item_t * list_iterator_t;

    typedef struct list {
        list_item_t *head, *tail;
        uint32_t length;
        int32_t (*compare)
        void (*datum_delete)(void *)
    } list_t;

    list_t l;
    list_item_t *li;
    list_iterator_t it;

    l.head // list_item_t *
    l.compare("Cady", "Cody") // int32_t
    &l.length //uint32_t *
    *l.head->next //list_item_t 
    *it // list_item_t
    (it == li) //int
    l.compare // int32_t (*) (const void *, const void *)
    (*it).next->datum //void *
    (&i)[7] //list_t (even though this is an error, we don't care, we just need what it technically could be)
    *((char *) li->datum) // char, void pointer from datum, cast to char *, * at start to dereference.
}
