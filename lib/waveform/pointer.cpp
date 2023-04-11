#include "pointer.h"
#include "waveform.h"
#include "flags.h"

uint16_t idx;
indexer_t x;
indexer_t* head = NULL;

const double omega = (2*PI)/(f_end_pos-f_start_pos);

void initIndexer(){
    head = &x;
    head->method = nextSaw;
    head->i = 0;
    head->o = 0;
    head->next = NULL;
}

void updateIndex(void){
    if(idx == (f_end_pos-f_start_pos) - 1){
        idx = f_start_pos;
    }
    else{
        idx++;
    }
}

uint8_t updateBufRange(uint16_t start, uint16_t stop){
    if(stop < start){
        return 1;
    }
    f_start_pos = start;
    f_end_pos = stop;
    idx = f_start_pos;
    return 0;
}

void nextSaw(uint16_t* i, uint16_t* o){
    *o = *i;
}

void nextTriangle(uint16_t* i, uint16_t* o){
    if(*i < (f_end_pos-f_start_pos)/2){
        *o = 2 * (*i);
    }
    else{
        *o = 2 * ((f_end_pos-f_start_pos) - *i);
    }
}

void nextSine(uint16_t* i, uint16_t* o){
    *o = (uint16_t)(sin(omega * (*i)) * (double)(((f_end_pos-f_start_pos)+1)/2 - 1)) + ((f_end_pos-f_start_pos)+1)/2;
}


void _addIndexer(indexer_t** p_head, void (*method)(uint16_t*, uint16_t*)) {
    indexer_t* new_indexer = (indexer_t*)malloc(sizeof(indexer_t));
    new_indexer->method = method;
    new_indexer->i = 0;
    new_indexer->o = 0;
    new_indexer->next = *p_head; 
    *p_head = new_indexer;
}

void addIndexer(void (*method)(uint16_t*, uint16_t*)){
    _addIndexer(&head, method);
}

uint16_t _rmIndexer(indexer_t** p_head, uint16_t num) {
    indexer_t* current = *p_head;
    indexer_t* temp;

    if(current->next == NULL){  // hinder user from deleting the base function
        return 0;
    }

    if (num == 0) { // delete the newest indexer
        *p_head = current->next;
        free(current);
        return 0;
    }
    int i;
    for (i = 0; current != NULL && i < num - 1; i++) {
        current = current->next;
    }
    if (current == NULL || current->next == NULL) { // requested indexer does not exist
        return i;
    }

    temp = current->next;
    current->next = temp->next;
    free(temp);
    return 0;
}

uint16_t rmIndexer(uint16_t num){
    return _rmIndexer(&head, num);
}

uint16_t getLastRes(indexer_t** p_head) {
    indexer_t* current = *p_head;
    current->i = idx;
    
    while(1){
        current->method(&(current->i), &(current->o));
        if (current->next == NULL){
            break;
        }
        current->next->i = current->o;
        current = current->next;
    }
    updateIndex();
    return current->o;
}
