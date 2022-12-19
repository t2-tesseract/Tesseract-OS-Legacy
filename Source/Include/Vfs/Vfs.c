#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/TextMode/TextMode.h>
#include <Include/Common/Common.h>
#include "Vfs.h"

const char* curDir = "";
const char* Resolution = "80x25";
const char* Language = "english";

struct Folder *Folders = NULL;
struct File* Files = NULL;

char *toCreate[4] = {
    "usr", 
    "bin",
    "dev",
    "home",
    "lib"
};

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
    TerminalSetColor(0x0B);
    while (f != NULL) {
        TerminalWrite(Tab);
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
    if (f == NULL) {
        // Error: unable to allocate memory for the new folder
        return NULL;
    }

    f->Name = Name;
    f->SubFolders = NULL;
    f->Files = NULL;
    f->Next = NULL;

    // Add the new folder to the linked list of folders
    f->Next = Folders;
    Folders = f;

    return f;
}

int DeleteFolder(struct Folder *f){
    MemoryFree(f->Name);
    MemoryFree(f->SubFolders);
    MemoryFree(f->Files);
    MemoryFree(f);
    return 0;
}

/*struct Folder *ListFolders(struct Folder *Folders){
    struct File *f = Folders;
    TerminalSetColor(0x0E);
    while (f != NULL) {
        TerminalWrite(Tab);
        TerminalWrite(f->Name);
        TerminalWrite("/\n");
        f = f->Next;
    }
    return f;
}*/

struct Folder *ListFolders(struct Folder* root) {
    // Traverse the linked list of folders starting from the root folder
    struct Folder* current = root;

    TerminalSetColor(0x0E);
    while (current != NULL) {
        TerminalWrite(Tab);
        TerminalWrite(current->Name);
        TerminalWrite("/\n");
        current = current->Next;
    }

    return current;
}

struct Folder *ChangeDirectory(struct Folder *Folders, char *Name){
    struct Folder *f = GoToFolder(Folders, Name);

    curDir = f->Name;
    if (f == NULL) {
        return NULL;
    }

    Folders = NULL;
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
    for (int i = 0; i < 4; i++) {
        Folders = CreateFolder(toCreate[i]);
        if (Folders == "NULL") {
            return -1;
        }
    }
    
    return 0;
}