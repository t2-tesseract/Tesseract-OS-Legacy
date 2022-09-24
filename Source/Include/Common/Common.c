#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Common.h"

#define NULL_POINTER ((void*)0)
#define DYNAMIC_MEM_TOTAL_SIZE 4*1024
#define DYNAMIC_MEM_NODE_SIZE sizeof(DynamicMemNode_t) // 16

typedef struct DynamicMemNode {
    uint32_t size;
    bool used;
    struct DynamicMemNode *next;
    struct DynamicMemNode *prev;
} DynamicMemNode_t;

static uint8_t DynamicMemArea[DYNAMIC_MEM_TOTAL_SIZE];
static DynamicMemNode_t *DynamicMemStart;

void InitDynamicMem(){
    DynamicMemStart = (DynamicMemNode_t *) DynamicMemArea;
    DynamicMemStart->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
    DynamicMemStart->next = NULL_POINTER;
    DynamicMemStart->prev = NULL_POINTER;
}

unsigned char Inb(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void Outb(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void Outw(unsigned short port, unsigned short value){
    asm volatile ("outw %%ax,%%dx": :"dN"(port), "a"(value));
} 

static inline void IoWait(void){
    Outb(0x80, 0);
}

void Delay(uint16_t Ms){
    for(long long int i=0; i<5000*(uint16_t)Ms / 2; i++)
        IoWait();
}

void *MemoryCopy(char *dst, char *src, int n){
	int i;
    for (i = 0; i < n; i++) {
        *(dst + i) = *(src + i);
    }
}

void* MemorySet(void * ptr, int value, uint16_t num){
    uint8_t* u8Ptr = (uint8_t *)ptr;

    for (uint16_t i = 0; i < num; i++)
        u8Ptr[i] = (uint8_t)value;

    return ptr;
}

int MemoryCompare(const void* ptr1, const void* ptr2, uint16_t num){
    const uint8_t* u8Ptr1 = (const uint8_t *)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t *)ptr2;

    for (uint16_t i = 0; i < num; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

void *FindBestMemBlock(DynamicMemNode_t *DynamicMem, size_t size) {
    DynamicMemNode_t *BestMemBlock = (DynamicMemNode_t *) NULL_POINTER;
    uint32_t BestMemBlockSize = DYNAMIC_MEM_TOTAL_SIZE + 1;

    DynamicMemNode_t *CurrentMemBlock = DynamicMem;
    while (CurrentMemBlock) {
        if ((!CurrentMemBlock->used) &&
            (CurrentMemBlock->size >= (size + DYNAMIC_MEM_NODE_SIZE)) &&
            (CurrentMemBlock->size <= BestMemBlockSize)) {
            BestMemBlock = CurrentMemBlock;
            BestMemBlockSize = CurrentMemBlock->size;
        }
        CurrentMemBlock = CurrentMemBlock->next;
    }
    return BestMemBlock;
}


void *MemoryAlloc(size_t size) {
    DynamicMemNode_t *BestMemBlock =
            (DynamicMemNode_t *) FindBestMemBlock(DynamicMemStart, size);

    if (BestMemBlock != NULL_POINTER) {
        BestMemBlock->size = BestMemBlock->size - size - DYNAMIC_MEM_NODE_SIZE;

        DynamicMemNode_t *MemNodeAllocate = (DynamicMemNode_t *) (((uint8_t *) BestMemBlock) +
                                                                        DYNAMIC_MEM_NODE_SIZE +
                                                                        BestMemBlock->size);
        MemNodeAllocate->size = size;
        MemNodeAllocate->used = true;
        MemNodeAllocate->next = BestMemBlock->next;
        MemNodeAllocate->prev = BestMemBlock;

        if (BestMemBlock->next != NULL_POINTER) {
            BestMemBlock->next->prev = MemNodeAllocate;
        }
        BestMemBlock->next = MemNodeAllocate;

        return (void *) ((uint8_t *) MemNodeAllocate + DYNAMIC_MEM_NODE_SIZE);
    }
    return NULL_POINTER;
}

void *MergeCurrentNoteIntoCurrent(DynamicMemNode_t *CurrentMemNode) {
    DynamicMemNode_t *NextMemNode = CurrentMemNode->next;
    if (NextMemNode != NULL_POINTER && !NextMemNode->used) {
        CurrentMemNode->size += CurrentMemNode->next->size;
        CurrentMemNode->size += DYNAMIC_MEM_NODE_SIZE;

        CurrentMemNode->next = CurrentMemNode->next->next;
        if (CurrentMemNode->next != NULL_POINTER) {
            CurrentMemNode->next->prev = CurrentMemNode;
        }
    }
    return CurrentMemNode;
}

void *MergeCurrentNoteIntoPrevious(DynamicMemNode_t *CurrentMemNode) {
    DynamicMemNode_t *PreviousMemNode = CurrentMemNode->prev;
    if (PreviousMemNode != NULL_POINTER && !PreviousMemNode->used) {
        PreviousMemNode->size += CurrentMemNode->size;
        PreviousMemNode->size += DYNAMIC_MEM_NODE_SIZE;

        PreviousMemNode->next = CurrentMemNode->next;
        if (CurrentMemNode->next != NULL_POINTER) {
            CurrentMemNode->next->prev = PreviousMemNode;
        }
    }
}

void MemoryFree(void *p){
    if (p == NULL_POINTER) {
        return;
    }

    DynamicMemNode_t *CurrentMemNode = (DynamicMemNode_t *) ((uint8_t *) p - DYNAMIC_MEM_NODE_SIZE);

    if (CurrentMemNode == NULL_POINTER) {
        return;
    }

    CurrentMemNode->used = false;

    CurrentMemNode = MergeCurrentNoteIntoCurrent(CurrentMemNode);
    MergeCurrentNoteIntoPrevious(CurrentMemNode);
}

const char* StringChar(const char* str, char chr){
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}

uint32_t StringLength(const char *String){
    int i = 0;
    while (String[i] != (char)0) {
        ++i;
    }
    return i;
}

bool Backspace(char Buffer[]){
    int Len = StringLength(Buffer);
    if (Len > 0) {
        Buffer[Len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

void Append(char s[], char n){
    int Len = StringLength(s);
    s[Len] = n;
    s[Len + 1] = '\0';
}

void InitPic(void){
	Outb(0x20, 0x11);
	Outb(0xA0, 0x11);

	Outb(0x21, 0x20);
	Outb(0xA1, 0x70);

	Outb(0x21, 0x04);
	Outb(0xA1, 0x02);

	Outb(0x21, 0x01);
	Outb(0xA1, 0x01);

	Outb(0x21, 0x0);
	Outb(0xA1, 0x0);
}

bool IsLower(char chr){
    return chr >= 'a' && chr <= 'z';
}

char Toupper(char chr){
    return IsLower(chr) ? (chr - 'a' + 'A') : chr;
}
