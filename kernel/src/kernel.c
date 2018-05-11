#include <inlow/kernel/print.h> /* printf() */


void kernel_main()
{
    char c = 'i';
    char* s = "I'm INLOW";
    printf("Hello World! I'm INLOW\n");
    printf("This is Char: %c\n", c);
    printf("This is String: %s\n", s);
    printf("%u\n", 4294967295);
    printf("0x%x\n", 10);
    printf("This is Number: 0x%x %d %d %u\n", 10, -2147483647, 2147483647, 4294967295);
    printf("This is %%\n");
    printf("This is \0 INLOW");
    printf("This is Long INLOW This is Long INLOW This is Long INLOW This is Long INLOW This is Long INLOW This is Long INLOW This is Long INLOW ");
}
