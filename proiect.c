#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>




int main(int argc, char *argv[])
{   DIR *dirp;
    if(argc!=2)
    {
        printf("numarul de argumente este gresit");
       
       
    }
     if((dirp=opendir(argv[1]))==NULL)
        printf("nu am primit un director bun");

    struct stat st;
    stat(argv[1], &st);

    return 0;

}