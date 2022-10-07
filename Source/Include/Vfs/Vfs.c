#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>
#include "Vfs.h"

struct Folder *Folders = NULL;
struct File* Files = NULL;

struct File *CreateFile(char *Name, char *Data, int Size, struct File *Files) {
    struct File *f = MemoryAlloc(sizeof(struct File));
    f->Name = Name;
    f->Data = Data;
    f->Size = Size;
    f->Next = Files;
    Files = f;
    return Files;
}

char *ReadFile(struct File *f){
    return f->Data;
}

int DeleteFile(struct File *f){
    MemoryFree(f->Name);
    MemoryFree(f->Data);
    MemoryFree(f);
    return 0;
}

void WriteFile(struct File *f, char *Data){
    f->Data = Data;
}

struct File *ListFiles(struct File *Files) {
    struct File *f = Files;
    while (f != NULL) {
        TerminalWrite(f->Name);
        TerminalWrite("\n");
        f = f->Next;
    }
    return f;
}

struct Folder *FolderCreateFile(struct Folder *Folders, char *Name){
    struct Folder *f = GoToFolder(Folders, Name);
    if (f == NULL) {
        return NULL;
    }
    struct File *Files = NULL;
    struct File *file = MemoryAlloc(sizeof(struct File));
    file->Name = Name;
    file->Data = NULL;
    file->Size = 0;
    file->Next = Files;
    Files = file;
    f->Files = Files;
    return f;
}

struct Folder *CreateFolder(char *Name){
    struct Folder *f = MemoryAlloc(sizeof(struct Folder));
    f->Name = Name;
    f->SubFolders = NULL;
    f->Files = NULL;
    f->Next = Folders;
    Folders = f;
    return Folders;
}

int DeleteFolder(struct Folder *f){
    MemoryFree(f->Name);
    MemoryFree(f->SubFolders);
    MemoryFree(f->Files);
    MemoryFree(f);
    return 0;
}

struct Folder *ListFolders(struct Folder *Folders){
    struct File *f = Folders;
    while (f != NULL) {
        TerminalWrite(f->Name);
        TerminalWrite("/\n");
        f = f->Next;
    }
    return f;
}

struct Folder *ChangeDirectory(struct Folder *Folders, char *Name){
    struct Folder *f = GoToFolder(Folders, Name);
    if (f == NULL) {
        return NULL;
    }
    return f;
}

struct Folder *GoToFolder(struct Folder *Folders, char *Name){
    struct Folder *f = Folders;
    while (f != NULL) {
        if (CompareString(f->Name, Name) == 0) {
            return f;
        }
        f = f->Next;
    }
    return NULL;
}

int InitVfs(){   
    char *toCreate[4] = {
        "usr", 
        "bin",
        "dev",
        "home",
        "lib"
    };

    for (int i = 0; i < 4; i++) {
        Folders = CreateFolder(toCreate[i]);
        if (Folders == "NULL") {
            return -1;
        }
    }

    /*Folders = CreateFolder("usr");
    if (Folders == "NULL") {
        return -1;
    }

    Folders = CreateFolder("bin");
    if (Folders == NULL) {
        return -1;
    }

    Folders = CreateFolder("dev");
    if (Folders == NULL) {
        return -1;
    }

    Folders = CreateFolder("home");
    if (Folders == NULL) {
        return -1;
    }

    Folders = CreateFolder("lib");
    if (Folders == NULL) {
        return -1;
    }*/
    
    return 0;
}