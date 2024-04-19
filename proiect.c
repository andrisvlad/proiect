#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PATH_LEN 1024
#define MAX_DIRS 10

typedef struct {
    char nume[MAX_PATH_LEN];
    int inode;
    off_t dimensiune;
    time_t ultima_modificare;
} MetadateIntrare;

void actualizeaza_metadate(const char *cale, MetadateIntrare *metadate) {
    struct stat stat_fisier;
    if (lstat(cale, &stat_fisier) == 0) {
        strcpy(metadate->nume, cale);
        metadate->inode = stat_fisier.st_ino;
        metadate->dimensiune = stat_fisier.st_size;
        metadate->ultima_modificare = stat_fisier.st_mtime;
    }
}

void creaza_snapshot(const char *cale_dir, const char *director_iesire) {
    DIR *dir;
    struct dirent *intrare;

    if (!(dir = opendir(cale_dir)))
        return;

    MetadateIntrare snapshot[1000];
    int dimensiune_snapshot = 0;

    while ((intrare = readdir(dir)) != NULL) {
        char cale[MAX_PATH_LEN];
        snprintf(cale, sizeof(cale), "%s/%s", cale_dir, intrare->d_name);

        if (intrare->d_type == DT_DIR) {
            if (strcmp(intrare->d_name, ".") == 0 || strcmp(intrare->d_name, "..") == 0)
                continue;

            
            pid_t pid = fork();// facem procesul copil pentru fiecare director

            if (pid == 0) {
                creaza_snapshot(cale, director_iesire);
                exit(0);
            }
        } else {
            actualizeaza_metadate(cale, &snapshot[dimensiune_snapshot]);
            dimensiune_snapshot++;
        }
    }
    closedir(dir);    //terminarea tuturor proceselor copil


    int status;
    while (wait(&status) > 0) {}

    
    char cale_snapshot[MAX_PATH_LEN]; //salvarea snapshot in director iesire
    snprintf(cale_snapshot, sizeof(cale_snapshot), "%s/snapshot.txt", director_iesire);

    FILE *f = fopen(cale_snapshot, "w");
    if (f == NULL) {
        printf("eroare deschidere fisier de iesire.\n");
        return;
    }

    for (int i = 0; i < dimensiune_snapshot; i++) {
        fprintf(f, "Nume: %s\n", snapshot[i].nume);
        fprintf(f, "Inode: %d\n", snapshot[i].inode);
        fprintf(f, "Dimensiune: %ld bytes\n", (long)snapshot[i].dimensiune);
        fprintf(f, "Ultima modificare: %s", ctime(&snapshot[i].ultima_modificare));
        fprintf(f, "\n");
    }

    fclose(f);
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > MAX_DIRS + 5) {
        printf(" utilizam%s -o director_iesire director_intrare1 director_intrare2 ... %d directoare de intrare)\n", argv[0], MAX_DIRS);
        return 1;
    }

    char *director_iesire = NULL;
    char *directoare_intrare[MAX_DIRS];
    int numar_directoare = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc)
                director_iesire = argv[++i];
            else {
                printf("director de iesire fara argument\n");
                return 1;
            }
        } else {
            if (numar_directoare < MAX_DIRS)
                directoare_intrare[numar_directoare++] = argv[i];
            else {
                printf("depasire nrmax de directoarre\n");
                return 1;
            }
        }
    }

    if (director_iesire == NULL) {
        printf("fara director de iesire\n");
        return 1;
    }

    for (int i = 0; i < numar_directoare; i++) {
        creaza_snapshot(directoare_intrare[i], director_iesire);
    }

    return 0;
}
