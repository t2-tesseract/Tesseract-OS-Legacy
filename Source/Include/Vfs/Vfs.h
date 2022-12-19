#pragma once

#include <stdint.h>
#include <stdbool.h>

extern struct Folder *Folders;
extern struct File* Files;

extern const char* curDir;
extern const char* Resolution;
extern const char* Language;

struct File{
    char *Name;
    char *Data;
    int Size;
    char *Next;
};

struct Folder {
    char *Name;
    struct Folder *SubFolders;
    struct File *Files;
    char* Next;
};

struct File *CreateFile(char *Name, char *Data, int Size, struct File *Files);
char *ReadFile(struct File *f);
int DeleteFile(struct File *f);
void WriteFile(struct File *f, char *Data);
struct File *ListFiles(struct File *Files);
struct Folder *FolderCreateFile(struct Folder *Folders, char *Name);
struct Folder *CreateFolder(char *Name);
int DeleteFolder(struct Folder *f);
struct Folder *ListFolders(struct Folder* root);
struct Folder *ChangeDirectory(struct Folder *Folders, char *Name);
struct Folder *GoToFolder(struct Folder *Folders, char *Name);
int InitVfs();