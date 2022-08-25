#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>

// struct File {
//     char* Data;
//     char* Name;
//     char* Extention;

//     void Read(){
//         return Data;
//     }

//     void Write(char* NewData){
//         Data = NewData;
//     }

//     void Append(char* NewData){
//         Data = StringNCat(Data, NewData, StringLength(NewData));
//     }

//     void Rename(char* NewName, char* NewExtention){
//         Name = NewName;
//         Extention = NewExtention;
//     }

// };

// struct Directory {
//     struct Directory* childDir;
//     int childdir;
//     int childfile;
//     struct File childFile[100];
//     char* fileName;
//     int currentDirIndex;
//     // currentDirIndex = -1;
//     struct File emptyFile;
// };

// void ChangeDirectory(char* Name) {
//         if (Name == "..")
//             currentDirIndex = -1; // -1 is root
//         else {
//             if (currentDirIndex == -1) {
//                 for (int i = 0; i < 100; i++)
//                     if (childDir[i].fileName == Name)
//                         currentDirIndex = i;
//             } else
//                 childDir[currentDirIndex].ChangeDirectory(Name);
//         }
//     }
