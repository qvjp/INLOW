#include <stddef.h>
#include <stdint.h>

int i = 0;
__attribute__ ((constructor)) void foo(void)
{
    uint8_t* video = (uint8_t*) 0xB8000;
    const char* hello = "This is constructor";
    for (size_t j = 0; j < 19; j++,i++)
    {
        video[2 * i] = hello[j];
        video[2 * i + 1] = 0x07;
    }
    i++;
}
__attribute__ ((destructor)) void boo(void)
{
    uint8_t* video = (uint8_t*) 0xB8000;
    const char* hello = "This is destructor";
    for (size_t j = 0; j < 18; j++,i++)
    {
        video[2 * i] = hello[j];
        video[2 * i + 1] = 0x07;
    }
    i++;
}

void kernel_main()
{
    uint8_t* video = (uint8_t*) 0xB8000;
    const char* hello = "Hello World! From INLOW";
    for (size_t j = 0; j < 23; j++,i++)
    {
        video[2 * i] = hello[j];
        video[2 * i + 1] = 0x07;
    }
    i++;
}
