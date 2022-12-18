#include <getopt.h> // for getopt()
#include <stdio.h>

#include "driver.h"

//static void help() {
//    printf("./main --pid=<PID>  (--vid=<vendor id> --did=<device id>)");
//}

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
        printf("Получение структуры proc_pci_dev...\n");
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
        sprintf(buffer, "%s %s", vId, dId); //записали в буффер
        fread(&buffer, 1, sizeof(buffer), file);
        printf("Получаю информацию об айди вендора и девайса...\n");
        printf(buffer);
        fclose(file);
    } else { //if argc == 2
        printf("Получение структуры vm_area...\n");
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
        char buffer[BUFF];
        sprintf(buffer, "%s", pId); //записали в буффер
        fread(&buffer, 1, sizeof(buffer), file);
        printf("Получаю информацию о пиде...\n");
        printf(buffer);
        fclose(file);
    }
    return 0;
}
