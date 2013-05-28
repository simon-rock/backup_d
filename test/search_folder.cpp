//#include "unistd.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define USER_LIST_FILE "./UserInfo.txt"
#define USER_LIST_PHOTODIR "./user-photo"
void GetUserList(char *file, char* folder)
{
    FILE* fp = NULL;
    DIR*  pDir = NULL;
    fp = fopen(file,"r");
    if (fp==NULL)
    {
        printf("file open fail \r\n");
        //return;
    }
	fclose(fp);

    pDir = opendir(folder);
    if (pDir==NULL)
    {
        printf("dir open fail \r\n");
        return;
    }
     
    struct dirent *pDirent;
    while((pDirent = readdir(pDir)) != NULL)
    {
        if (pDirent->d_type == DT_REG)//普通文件
        {    
        }
		else if (pDirent->d_type == DT_DIR)
		{
		}
        printf("d_type:%d,d_name: %s\n",pDirent->d_type,pDirent->d_name);
    }
	closedir(pDir);

}
int main(int argc, char **argv)
{
    GetUserList(argv[1], argv[2]);
    return 0;
}