#ifndef _POINTER_H_
#define _POINTER_H_

#include "Arduino.h"

typedef struct Indexer {
    void (*method)(uint16_t* i, uint16_t* o);
    uint16_t i;
    uint16_t o;
    struct Indexer* next;
} indexer_t;

void initIndexer();
void _addIndexer(indexer_t** p_head, void (*method)(uint16_t*, uint16_t*));
uint16_t _rmIndexer(indexer_t** p_head, uint16_t num);
void addIndexer(void (*method)(uint16_t*, uint16_t*));
uint16_t rmIndexer(uint16_t num);
uint16_t getLastRes(indexer_t** p_head);

void updateIndex(void);
uint8_t updateBufRange(uint16_t start, uint16_t stop);
void nextSaw(uint16_t* i, uint16_t* o);
void nextTriangle(uint16_t* i, uint16_t* o);
void nextSine(uint16_t* i, uint16_t* o);

extern indexer_t* head;

#endif