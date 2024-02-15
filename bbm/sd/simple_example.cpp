#include<ctime>
#include <stdio.h>
#include<string>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"

class SD 
{
    sd_card_t *pSD;
    FRESULT fr;
    const char* filename;
public:
    SD():
        pSD(sd_get_by_num(0)), 
        fr(f_mount(&pSD->fatfs, pSD->pcName, 1)),
        filename([]{
            char time_str[17];
            std::time_t t = std::time(nullptr);
            std::strftime(time_str, sizeof(time_str), "%Y%m%d_%H%M%S", std::localtime(&t));
            return (std::string("log_") + time_str).c_str();
        }())
    {
        if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    ~SD()
    {
        f_unmount(pSD->pcName);
    }
    void write(std::string& write_str)
    {
        FIL fil;
        fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
        if (FR_OK != fr && FR_EXIST != fr)
            panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
        if (f_printf(&fil, write_str.c_str()) < 0) {
            printf("f_printf failed\n");
        }
        fr = f_close(&fil);
        if (FR_OK != fr) {
            printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
        }
    }
};

// int main() {
//     stdio_init_all();
//     time_init();

//     puts("Hello, world!");

//     // See FatFs - Generic FAT Filesystem Module, "Application Interface",
//     // http://elm-chan.org/fsw/ff/00index_e.html
//     sd_card_t *pSD = sd_get_by_num(0);
//     FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
//     if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
//     FIL fil;
//     const char* const filename = "filename.txt";
//     fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
//     if (FR_OK != fr && FR_EXIST != fr)
//         panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
//     if (f_printf(&fil, "Hello, world!\n") < 0) {
//         printf("f_printf failed\n");
//     }
//     fr = f_close(&fil);
//     if (FR_OK != fr) {
//         printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
//     }
//     f_unmount(pSD->pcName);

//     puts("Goodbye, world!");
//     for (;;);
// }
