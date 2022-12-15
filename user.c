#include <getopt.h> // for getopt()
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
//#include <proc_fs.h>

#include "driver.h"

static void help() {
    printf("./main --pid=<PID>  (--vid=<vendor id> --did=<device id>)");
}

//TODO: что-то с файлом, нужно проверить корректность записи, а то но ворчит
int main(int argc, char **argv) {
    if (argc != 3 && argc != 2) {
        printf("Неверное число аргументов\n");
        return -1;
    }
    const char *shortOptions = "v:d:p:";
    char *vId = 0;
    char *dId = 0;
    char *pId = 0;
    struct option longOptions[] = {
            {"vid", 1, 0,    'v'},
            {"did", 1, 0,    'd'},
            {"pid", 1, 0,    'p'},
            {NULL,  0, NULL, 0}
    };

    FILE *file = fopen("/proc/OS2lab/out", "r+");
    if (file == NULL) {
        printf("Не можу открыть файл\n");
        return -1;
    }
    clearerr(file);

    int rez;
    int opIdx;
    if (argc == 3) {
        printf("Получение структуры pci_dev...\n");
        while ((rez = getopt_long(argc, argv, shortOptions, longOptions, &opIdx)) != -1) {
            switch (rez) {
                case 'v': {
                    vId = optarg; /* optarg - pointer to argument of current option  */
                    break;
                }
                case 'd': {
                    dId = optarg;
                    break;
                }
                case '?': {
                    printf("Неизвестная опция\n");
                    break;
                }
                default:
                    printf("Опций не обнаружено\n");
                    break;
            }
        }
        char buffer[BUFF];
        fprintf(file, "Vendor:%s, Device:%s", vId, dId);
        while (true) {
            char *msg = fgets(buffer, BUFF, file);
            //TODO: для дебага
            printf("%s\n", msg);
            if (msg == NULL) {
                if (feof(file)) break;
                fprintf(stderr, "Pci_dev struct reading failed with errno code: %d\n", errno);
                return -1;
            } else {
                printf("%s", msg);
            }
        }

    } else { //if argc == 2
        printf("Получение структуры ppp_channel...\n");
        rez = getopt_long(argc, argv, shortOptions, longOptions, &opIdx);
        switch (rez) {
            case 'p': {
                pId = optarg;
                break;
            }
            case '?': {
                printf("Неизвестная опция\n");
                break;
            }
            default:
                printf("Опций не обнаружено\n");
                break;
        }
        printf("ПИД:%s\n", pId);
    }
    fclose(file);
    return 0;
}