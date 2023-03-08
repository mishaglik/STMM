
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;

int main();

void _premain(void)
{
    unsigned long *src, *dst;

    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata)
        *(dst++) = *(src++);

    main();
}