#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>


#define MAX_SIZE 1000

/*
void count_letters(FILE *file, int *counts)
{
    int c;
    while((c=fgetc(file))!=EOF){
        if(isalpha(c)){
            counts[tolower(c)-'a']++;
        }
    }
}



void analyze_file(const char *file_path, int *counts){
if(file==NULL){
    printf("nu putem deschide fisierul: %s\n",file_path);
    return;
}

count_letters(file,counts);
fclose(file);
}


void parcurgem_directory(const char *dir_path, int *counts)
{
    DIR *dir=opendir(dir_path);
    if(dir==NULL)
    {
        printf("nu putem parcurge directoarele: %s\n",dir_path);
        return;
    }

}


struct dirent *entry;

while((entry=readdir(dir))!=NULL){
    if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0 ){
        continue;
    }
}





int main()
{


    return 0;
}
*/

void count_letters(FILE *file, int *counts) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
        counts[tolower(c) - 'a']++;
        }
    }   
}

void analyze_file(const char *file_path, int *counts) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
    printf("Could not open file: %s\n", file_path);
    return;
}

count_letters(file, counts);
fclose(file);
}

void traverse_directory(const char *dir_path, int *counts) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Could not open directory: %s\n", dir_path);
    return;
}

struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
    continue;
}

char entry_path[MAX_SIZE];
snprintf(entry_path, sizeof(entry_path), "%s/%s", dir_path, entry->d_name);

if (entry->d_type == DT_DIR) {
    traverse_directory(entry_path, counts);
} else if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
        analyze_file(entry_path, counts);
    }
}

closedir(dir);
}

int main(int argc, char *argv[]) {
if (argc != 2) {
    printf("Usage: %s \n", argv[0]);
    return 1;
}

int counts[26] = {0}; 
traverse_directory(argv[1], counts);

printf("Letter frequencies:\n");
for (int i = 0; i < 26; i++) {
    printf("%c : %d\n", 'a' + i, counts[i]);
}

return 0;
}