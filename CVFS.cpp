#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPEACIAL 2 // Special files : driver files

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeInodes;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char* Buffer;
    int LinkCount;
    int ReferenceCount;
    int permission; // 1   2   3
    struct inode *next;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode; // 1   2   3
    PINODE ptrinode;
}FILETABLE, *PFILETABLE;

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    man
//   Description :      Used to show description and syntax of the command
//   Input arguments :  command
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void man(char* name)
{
    if(name == NULL) return;

    if(strcmp(name, "create") == 0)
    {
        printf("\nDescription : Used to create new regular file\n");
        printf("Usage : create File_name Permission\n");
    }
    else if(strcmp(name, "read") == 0)
    {
        printf("\nDescription : Used to read data from regualr file\n");
        printf("Usage : read File_name No_of_Bytes_To-Read\n");
    }
    else if(strcmp(name, "write") == 0)
    {
        printf("\nDescription : Used to write into regualr file\n");
        printf("\nUsage : write File_name\nAfter this enter the data that we want to write\n");
    }
    else if(strcmp(name, "ls") == 0)
    {
        printf("\nDescription : Used to list all the information of files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(name, "stat") == 0)
    {
        printf("\nDescription : Used to display information of file\n");
        printf("Usage : stat File_name\n");
    }
    else if(strcmp(name, "fstat") == 0)
    {
        printf("\nDescription : Used to display information of file\n");
        printf("Usage : fstat file_descriptor\n");
    }
    else if(strcmp(name, "truncate") == 0)
    {
        printf("\nDescription : Used to remove data from file\n");
        printf("Usage : truncate File_nmae\n");
    }
    else if(strcmp(name, "open") == 0)
    {
        printf("\nDescription : Used to open existing file\n");
        printf("Usage : open File_name mode\n");
    }
    else if(strcmp(name, "close") == 0)
    {
        printf("\nDescription : Used to close opened file\n");
        printf("Usage : close File_name\n");
    }
    else if(strcmp(name, "closeall") == 0)
    {
        printf("\nDescription : Used to close all file\n");
        printf("Usage : closeall\n");
    }
    else if(strcmp(name, "lseek") == 0)
    {
        printf("\nDescription : Used to change file offset\n");
        printf("Usage : lseek File_name ChangeInOffset StartPoint\n");
    }
    else if(strcmp(name, "rm") == 0)
    {
        printf("\nDescription : Used to delete the file\n");
        printf("Usage : rm File_name\n");
    }
    else
    {
        printf("\033[31mERROR : No manual entry available\033[0m\n");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    DisplayHelp
//   Description :      Used to show all commands which are available
//   Input arguments :  void
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayHelp()
{
    printf("\nls : To list out all files\n");
    printf("clear : To clear console\n");
    printf("create : To create a file\n");
    printf("open : To open the file\n");
    printf("close : To close all opened files\n");
    printf("closeall : To close all opened files\n");
    printf("read : To read the contents from files\n");
    printf("write : To write contents into file\n");
    printf("exit : To terminate file system\n");
    printf("stat : To Display information of file using name\n");
    printf("fstat : To Display information of file using file descriptor\n");
    printf("truncate : To remove all data from file\n");
    printf("rm : To delete the file\n");
    printf("man : To provide description and usage related a command\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    GetFDFromName
//   Description :      Used to get file descriptor id 
//   Input arguments :  File Name
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetFDFromName(char* name)
{
    int i = 0;

    while(i < MAXINODE)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
            if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName), name) == 0)
                break;
        i++;
    }

    if(i == 50) return -1;
    else return i;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    Get_Inode
//   Description :      Used to get address of the inode 
//   Input arguments :  File Name
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PINODE Get_Inode(char* name)
{
    PINODE temp = head;
    int i = 0;

    if(name == NULL)
        return NULL;
    
    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0)
            break;
        temp = temp->next;
    }

    return temp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    CreaILB
//   Description :      Creates Disk Inode List Block
//   Input arguments :  void
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn->LinkCount = 0;
        newn->ReferenceCount = 0;
        newn->FileType = 0;
        newn->FileSize = 0;

        newn->Buffer = NULL;
        newn->next = NULL;

        newn->InodeNumber = i;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
        i++;
    }
    printf("DILB created successfully\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    InitialiseSuperBlock
//   Description :      Creates Super Block
//   Input arguments :  void
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitialiseSuperBlock()
{
    int i = 0;
    while(i < MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }

    SUPERBLOCKobj.TotalInodes = MAXINODE;
    SUPERBLOCKobj.FreeInodes = MAXINODE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    CreateFile
//   Description :      Creates a file on primary memory
//   Input arguments :  File name,  File access permission
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CreateFile(char* name, int permission)
{
    int i = 0;
    PINODE temp = head;

    if((name == NULL) || (permission == 0) || (permission > 3))
        return -1;

    if(SUPERBLOCKobj.FreeInodes == 0)
        return -2;

    (SUPERBLOCKobj.FreeInodes)--;

    if(Get_Inode(name) != NULL)
        return -3;

    while(temp != NULL)
    {
        if(temp->FileType == 0)
            break;
        temp = temp->next;
    }

    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable == NULL)
            break;
        i++;
    }

    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = permission;
    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;

    UFDTArr[i].ptrfiletable->ptrinode = temp;

    strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName, name);
    UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
    UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
    UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
    UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
    UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    return i;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    rm_File
//   Description :      removes file from the primary memory
//   Input arguments :  File name
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rm_File(char* name)
{
    int fd = 0;

    fd = GetFDFromName(name);
    if(fd == -1)
        return -1;

    (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

    if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
    {
        UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
        // free(UFDTArr[fd].ptrfiletable->ptrinode->Buffer);
        free(UFDTArr[fd].ptrfiletable);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    ReadFile
//   Description :      reads N number of bytes from the file 
//   Input arguments :  file descriptor,  Buffer, number of bytes you want to read
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ReadFile(int fd, char* arr, int isize)
{
    int read_size = 0;
    if(UFDTArr[fd].ptrfiletable == NULL) return -1;

    if(UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != (READ+WRITE)) return -2;

    if(UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) return -3;

    if(UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR) return -4;

    read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);
    if(read_size < isize)
    {
        strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), read_size);

        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
    }
    else
    {
        strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), isize);

        (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + isize;
    }

    return isize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    WriteFile
//   Description :      Writes N number of data into the file
//   Input arguments :  file descriptor, Buffer,  number of bytes you want to write
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WriteFile(int fd, char* arr, int isize)
{
    if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != (READ + WRITE))) return -1;
    if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != (READ + WRITE))) return -1;
    if((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE) return -2;
    if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR) return -3;

    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset), arr, isize);

    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;

    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;

    return isize; 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    OpenFile
//   Description :      Open file in main memory
//   Input arguments :  File name, file access permission
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int OpenFile(char* name, int mode)
{
    int i = 0;
    PINODE temp = NULL;

    if((name == NULL) || (mode <= 0))
        return -1;

    temp = Get_Inode(name);
    if(temp == NULL)
        return -2;

    if(temp->permission < mode)
        return -3;

    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable == NULL) return -1;
        UFDTArr[i].ptrfiletable->count = 1;
        UFDTArr[i].ptrfiletable->mode = mode;
        if(mode == (READ + WRITE))
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }
        else if(mode == READ)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
        }
        else if(mode == WRITE)
        {
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }
        UFDTArr[i].ptrfiletable->ptrinode = temp;
        (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

        return i;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    CloseFileByName
//   Description :      removes file virtually from the main memory
//   Input arguments :  File descriptor
//   Author :           Jitesh Anil Varade
//   Current State :    Contains error
//   LastModified :     16/02/2024
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseFileByName(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    CloseFileByName
//   Description :      Closes file virtually from main memory
//   Input arguments :  File name
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CloseFileByName(char* name)
{
    int i = 0;
    i = GetFDFromName(name);
    if(i == -1)
        return -1;

    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    CloseAllFile
//   Description :      Closes all files from main memory
//   Input arguments :  void
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CloseAllFile()
{
    int i = 0;
    while(i < 50)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
            break;
        }
        i++;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    LseekFile
//   Description :      Used to set offset of a file
//   Input arguments :  file descriptor, by how much you want ot move the index, index from where you want to set the offset.
//   Author :           Jitesh Varade
//   last modified :    14/02/2024
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int LseekFile(int fd, int size, int from)
{
    if((fd < 0) || (from > 2)) return -1;
    if(UFDTArr[fd].ptrfiletable == NULL) return -1;

    if(((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == (READ + WRITE))))
    {
        if(from == CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) return -1;
            if(((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size;
        }
        else if(from == START)
        {
            if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)) return -1;
            if(size < 0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = size;
        }
        else if(from == END)
        {
            if(((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size) > MAXFILESIZE) return -1;
            if(((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0) return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
    else if(UFDTArr[fd].ptrfiletable->mode == WRITE)
    {
        if(from == CURRENT)
        {
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE) return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0) return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)) 
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
        }
        else if(from == START)
        {
            if(size > MAXFILESIZE) return -1;
            if(size < 0) return -1;
            if(size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size;
            (UFDTArr[fd].ptrfiletable->writeoffset) = size;
        }
        else if(from == END)
        {
            if(((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size) > MAXFILESIZE) return -1;
            if(((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0) return -1;
            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    ls_file
//   Description :      List all the files 
//   Input arguments :  void
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ls_file()
{
    int i = 0;
    PINODE temp = head;

    if(SUPERBLOCKobj.FreeInodes == MAXINODE)
    {
        printf("\033[0mERROR : There are no such files\n");
        return;
    }

    printf("\nFileName\tInode number\tFileSize\tLinkCount\n");
    printf("---------------------------------------------------------\n");
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName, temp->InodeNumber, temp->FileActualSize, temp->LinkCount);
        }
        temp = temp->next;
    }
    printf("----------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    fstat_file
//   Description :      gives statistival information about the file
//   Input arguments :  File descriptor
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int fstat_file(int fd)
{
    PINODE temp = head;
    int i = 0;
    if(fd < 0) return -1;

    if(UFDTArr[fd].ptrfiletable == NULL) return -2;

    temp = UFDTArr[fd].ptrfiletable->ptrinode;

    printf("\n----------Statistical Information about file----------\n");
    printf("File name : %s\n", temp->FileName);
    printf("Inode Number : %d\n", temp->InodeNumber);
    printf("File size : %d\n", temp->FileSize);
    printf("Actual File Size : %d\n", temp->FileActualSize);
    printf("Link Count : %d\n", temp->LinkCount);
    printf("Reference Count : %d\n", temp->ReferenceCount);

    if(temp->permission == 1)
        printf("File Permission : Read only\n");
    if(temp->permission == 2)
        printf("File Permissino : Write\n");
    if(temp->permission == 3)
        printf("File Permission : Read & Write\n");
    printf("--------------------------------------------------------\n");

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    stat_file
//   Description :      gives statistical insformation about the file
//   Input arguments :  File descriptor
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int stat_file(char* name)
{
    PINODE temp = head;
    int i = 0;

    if(name == NULL) return -1;

    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0)
            break;
        temp = temp->next;
    }

    if(temp == NULL) return -2;

    printf("\n----------Statistical Information about file----------\n");
    printf("File name : %s\n", temp->FileName);
    printf("Inode Number : %d\n", temp->InodeNumber);
    printf("File size : %d\n", temp->FileSize);
    printf("Actual File Size : %d\n", temp->FileActualSize);
    printf("Link Count : %d\n", temp->LinkCount);
    printf("Reference Count : %d\n", temp->ReferenceCount);

    if(temp->permission == 1)
        printf("File Permission : Read only\n");
    if(temp->permission == 2)
        printf("File Permissino : Write\n");
    if(temp->permission == 3)
        printf("File Permission : Read & Write\n");
    printf("--------------------------------------------------------\n");

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    truncate_file
//   Description :      Used to delete all data from file
//   Input arguments :  File name
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int truncate_File(char* name)
{
    int fd = GetFDFromName(name);
    if(fd == -1) return -1;

    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer, 0, 1024);
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Function Name :    main
//   Description :      Entry point function
//   Input arguments :  void
//   Author :           Jitesh Anil Varade
//       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    char* ptr = NULL;
    int ret = 0, fd = 0, count = 0;
    char command[4][80], str[80], arr[1024];

    InitialiseSuperBlock();
    CreateDILB();

    printf("Welcome to \n");
    printf("\033[31m ________   ___          ___  _________   __________\n");
    printf("|   _____|  \\  \\        /  /  |   _____|  |  _______|\n");
    printf("|  |         \\  \\      /  /   |  |__      |  |______\n");
    printf("|  |          \\  \\    /  /    |   __|     |______   |\n");
    printf("|  |_____      \\  \\__/  /     |  |         ______|  |\n");
    printf("|________|      \\______/      |__|        |_________|\033[0m\n");
    printf("\nType 'help' for more information\n");

    while(1)
    {
        fflush(stdin);
        strcpy(str,"");

        printf("\n\033[92mCustomised VFS\033[0m:\033[94m~$\033[0m ");

        fgets(str, 80, stdin);
        
        count = sscanf(str,"%s %s %s %s",command[0], command[1], command[2], command[3]);
        if(count == 1)
        {
            if(strcmp(command[0],"ls") == 0)
            {
                ls_file();
                continue;
            }
            else if(strcmp(command[0], "closeall") == 0)
            {
                CloseAllFile();
                printf("All files closed succesfully\n");
                continue;
            }
            else if(strcmp(command[0], "clear") == 0)
            {
                system("clear");
                // printf("\033c");
                continue;
            }
            else if(strcmp(command[0], "help") == 0)
            {
                DisplayHelp();
                continue;
            }
            else if(strcmp(command[0], "exit") == 0)
            {
                printf("Terminating the Customised Virtual File System\n");
                break;
            }
            else
            {
                printf("\n\033[31mERROR : Command not found!!!\033[0m\n");
                continue;
            }
        }
        else if(count == 2)
        {
            if(strcmp(command[0], "stat") == 0)
            {
                ret = stat_file(command[1]);
                if(ret == -1)
                    printf("\033[31mERROR : Incorrect parameters\033[0m\n");
                if(ret == -2)
                    printf("\033[31mERROR : There is no such file\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "fstat") == 0)
            {
                ret = fstat_file(atoi(command[1]));
                if(ret == -1)
                    printf("\033[31mERROR : Incorrect parameter\033[0m\n");
                if(ret == -2)
                    printf("\033[31mERROR : There is no such file\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "close") == 0)
            {
                ret = CloseFileByName(command[1]);
                if(ret == -1)
                    printf("\033[31mERROR : There is no such file\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "rm") == 0)
            {
                ret = rm_File(command[1]);
                if(ret == -1)
                    printf("\033[31mERROR : There is no such file\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "man") == 0)
            {
                man(command[1]);
                continue;
            }
            else if(strcmp(command[0], "write") == 0)
            {
                fd = GetFDFromName(command[1]);
                if(fd == -1)
                {    printf("\033[31mERROR : Incorrect parameter\033[0m\n");
                    continue;
                }
                printf("Enter the data : \n");
                char ch;
                scanf("%[^'\n']s",arr);
                scanf("%c",&ch);

                ret = strlen(arr);
                if(ret == 0)
                {
                    printf("\033[31mERROR : Incorrect parameter\033[0m\n");
                    continue;
                }
                ret = WriteFile(fd,arr,ret);
                if(ret == -1)
                    printf("\033[31mERROR : Permission denied\033[0m\n");
                if(ret == -2)
                    printf("\033[31mERROR : There is no sufficient memory to write\033[0m\n");
                if(ret == -3)
                    printf("\033[31mERROR : It is not regular file\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "truncate") == 0)
            {
                ret = truncate_File(command[1]);
                if(ret == -1)
                    printf("\033[31mERROR : Incorrect parameter\033[0m\n");
                continue;
            }
            else
            {
                printf("\033[31mERROR : Command not found!!!\033[0m\n");
                continue;
            }
        }
        else if(count == 3)
        {
            if(strcmp(command[0], "create") == 0)
            {
                ret = CreateFile(command[1], atoi(command[2]));
                if(ret >= 0)
                    printf("File is successfully created with file descriptor : %d\n", ret);
                if(ret == -1)
                    printf("\n\033[31mERROR : Incorrect parameters\033[0m\n");
                if(ret == -2)
                    printf("\n\033[31mERROR : There are no inodes\033[0m\n");
                if(ret == -3)
                    printf("\n\033[31mERROR : File already exists\033[0m\n");
                if(ret == -4)
                    printf("\n\033[31mERROR : Memory allocation failure\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "open") == 0)
            {
                ret = OpenFile(command[1], atoi(command[2]));
                if(ret >= 0)
                    printf("File is successfully opened with file descriptor : %d\n", ret);
                if(ret == -1)
                    printf("\n\033[31mERROR : Incorrect paramters\033[0m\n");
                if(ret == -2)
                    printf("\n\033[31mERROR : File not present\033[0m\n");
                if(ret == -3)
                    printf("\n\033[31mERROR : Permission denied\033[0m\n");
                continue;
            }
            else if(strcmp(command[0], "read") == 0)
            {
                fd = GetFDFromName(command[1]);
                if(fd == -1)
                {
                    printf("\n\033[31mERROR : Incorrect parameter\033[0m\n");
                    continue;
                }
                ptr = (char*)malloc(sizeof(atoi(command[2])) + 1);
                if(ptr == NULL)
                {
                    printf("\n\033[31mERROR : Memory allocation failed\033[0m\n");
                    continue;
                }
                ret = ReadFile(fd, ptr, atoi(command[2]));
                if(ret == -1)
                    printf("\n\033[31mERROR : File does not exists\033[0m\n");
                if(ret == -2)
                    printf("\n\033[31mERROR : Permission denied\033[0m\n");
                if(ret == -3)
                    printf("\n\033[31mERROR : Reached at the end of file\033[0m\n");
                if(ret == -4)
                    printf("\n\033[31mERROR : It is not a regular file\033[0m\n");
                if(ret == 0)
                    printf("\n\033[31mERROR : File is empty\033[0m\n");
                if(ret > 0)
                {
                    write(2, ptr, ret);
                }
                continue;
            }
            else
            {
                printf("\n\033[31mERROR : Command not found!!!\033[0m\n");
                continue;
            }
        }
        else if(count == 4)
        {
            if(strcmp(command[0], "lseek") == 0)
            {
                fd = GetFDFromName(command[1]);
                if(fd == -1)
                {
                    printf("\n\033[31mERROR : Incorrect parameter\033[0m\n");
                    continue;
                }
                ret = LseekFile(fd, atoi(command[2]), atoi(command[3]));
                if(ret == -1)
                {
                    printf("\n\033[31mERROR : Unable to perform lseek\033[0m\n");
                }
            }
        }
        else
        {
            printf("\n\033[31mERROR : Command not found!!!\033[0m\n");
            continue;
        }
    }

    return 0;
}