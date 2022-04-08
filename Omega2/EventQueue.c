//
//  EventQueue.c
//  Omega2
//
//  Created by Matt Parsons on 25/03/2022.
//

#include "EventQueue.h"
#include "SDL2/SDL.h"

void SHOULDNOTEXECUTE(EventQueue_t* queue, void* data){
    printf("This is an Unassigned Event!!!");
    return;
}

void InternalNoOperation(EventQueue_t* queue, void* data){
    //Burn a cycle and return
    printf("Do nossing!!");
}

EventQueue_t* CreateQueue(void){

    //Create the queue
    EventQueue_t* queue = malloc(sizeof(EventQueue_t));

    //set up the event pool
    queue->freeList = &queue->eventPool[1];
    queue->lastFree = &queue->eventPool[EVENT_POOL_SIZE - 1];
    
    for(int i=0; i<EVENT_POOL_SIZE; ++i){
        queue->eventPool[i].next = &queue->eventPool[i+1];
        queue->eventPool[i].function = SHOULDNOTEXECUTE;
        
    }
    
    //Create a couple of dummy events as the queue start and finish points
    queue->eventPool[0].queue = queue;
    queue->eventPool[0].next = NULL;
    queue->eventPool[0].prev = NULL;
    queue->eventPool[0].cycle = 0xFFFFFFFFFFFFFFFF; //Last possible cycle... about 73 thousand years in the future, at 8 million cycles per second.
    queue->eventPool[0].type = 0;
    queue->eventPool[0].function = InternalNoOperation;
    queue->eventPool[0].data = NULL;

    
    queue->nextEvent = &queue->eventPool[0];
    queue->currentCycle = 0;
    
    return queue;
}

CycleEvent_t* CreateEvent(EventQueue_t* queue, void* func, void* data, uint64_t cycles){    //The number of cycles until this event should be called
    
    //Allocate a new Event
    CycleEvent_t* event = queue->freeList;
    queue->freeList = event->next;
    
    event->cycle = queue->currentCycle + cycles;
    event->function = func;
    event->data = data;
    event->type = 1;
    
    //Insert event into corrrect position the queue
    CycleEvent_t* checkEvent = queue->nextEvent;

    
    while(checkEvent->next != NULL){
        
        if(event->cycle < checkEvent->cycle){
            break;
        }
        
        checkEvent = (CycleEvent_t*) checkEvent->next;
    };
    
    event->next = checkEvent;
   
    if(checkEvent->prev == NULL){
        checkEvent->prev = event;
        queue->nextEvent = event;
        event->prev = NULL;
        return event;
    }
    
    event->prev = checkEvent->prev;
    checkEvent->prev = event;
    event->prev->next = event;
    
    return event;
}


void ExecuteEvent(EventQueue_t* queue, CycleEvent_t* event){
    event->function(queue, event->data);
    
    queue->nextEvent = event->next;
    queue->nextEvent->prev = NULL;
    
    //insert the retired event at the back of the freelist
    queue->lastFree->next = event;
    queue->lastFree = event;
    event->function = SHOULDNOTEXECUTE;
    event->type = 0;
}

void StartExecution(EventQueue_t* queue){
    queue->startTime = SDL_GetPerformanceCounter();
    AdvanceCycle(queue);
}

void AdvanceCycle(EventQueue_t* queue){
    
    //printf("%llu:",queue->currentCycle);
    
    queue->currentCycle += 1;
    CycleEvent_t* event = queue->nextEvent;
    
    while(event->cycle < queue->currentCycle){
        ExecuteEvent(queue, event);
        event = event->next;
    }
 
    //printf("\n");
    
}
