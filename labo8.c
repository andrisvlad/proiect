#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>




int main(int argc, char* argv[])
{
    if(argc<2 || argc>6)//verif suf.arg
    {
        fprintf(stderr,"utilizam: %s dir1 dir2 dir3\n", argv[0]);
        return 1;
    }

        for(int i=1;i<argc;i++)//parc arg l.comanda si creem proces pt fiec.director

    {
        DIR *dir;
        if((dir=opendir(argv[i]))==NULL)
        {
            perror("eroare: nu de director\n");
            continue;
        }
        pid_t pid=fork();
        if(pid==-1){
            perror("eroare la fork");//eroare la creeare proces
            return 1;
        }
        else if(pid==0){
            printf("listam director %s:\n",argv[i]);//procesul copil
            execlp("ls","ls","-l",argv[i],NULL);//execul esueaza daca ajunge aici
            perror("executie ls esuata");
            return 1;
        }
        else
        {
            wait(NULL);//asteptam procesul copil sa se termine
        }
    }
    return 0;
}