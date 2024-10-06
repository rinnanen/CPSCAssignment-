#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// structs
struct Page
{
    int pageNum;
    int dirtyBit;
};

struct Queue {
    int size;
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct QueueNode {
    struct Page page;
    struct QueueNode* next;
};

struct Queue* new_queue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

struct QueueNode* new_node(struct Page page) {
    struct QueueNode* new = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    new->page = page;
    new->next = NULL;
    return new;
}

// function prototypes
void enqueue(struct Queue* queue, struct Page page);
struct Page dequeue(struct Queue* queue);
void FIFO(struct Page *references, int ref_length, int frame);

// constants
int ref_length = 150; // make it 150 for now so the data is manageable
int frame_count = 5;
char cmp_string[] = "FIFO";

int main()
//int argc, char *argv[]
{
    //if (argc < 2)
    //{
        //printf("Usage: %s <input_file>\n", argv[0]);
        //return 1;
    //}

    FILE *file = fopen("input_file.csv", "r");
    if (!file)
    {
        //printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    struct Page references[15050]; // 15052 entries
    int i = 0;
    int pageNum, dirtyBit;
    char header[100];

    // read header (assuming first line is the header)
    fgets(header, sizeof(header), file);

    // read each row from the input file
    while (fscanf(file, "%d,%d\n", &pageNum, &dirtyBit) == 2)
    {
        references[i].pageNum = pageNum;
        references[i].dirtyBit = dirtyBit;
        i++;
        if (i >= 15052)
            break; // stop once array is full
    }

    if (strcmp(cmp_string, "FIFO") == 0) {
        //argv[1]
        FIFO(references, ref_length, frame_count);
    }

    fclose(file);
    return 0;
}


//Algos

// queue algorithms - enqueue and dequeue
void enqueue(struct Queue* queue, struct Page page) {
    struct QueueNode* new = new_node(page);
    if (queue->rear == NULL) {
        queue->front = new;
        queue->rear = new;
    } else {
        queue->rear->next = new;
        queue->rear = new;
    }
    queue->size++;
}

struct Page dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        printf("Error - empty queue.");
    }

    struct QueueNode* temp_node = queue->front;
    struct Page page = temp_node->page;
    queue->front = queue->front->next;

    if (queue->front == NULL) { queue->rear = NULL; }

    free(temp_node);
    queue->size--;
    return page;
}

//FIFO: replaces the page that has been in memory the longest

void FIFO(struct Page *references, int ref_length, int frame) {
    for (int i = 0; i < ref_length; i++) {
        struct Page *this_page = &references[i];
        int pn = this_page->pageNum;
        int db = this_page->dirtyBit;
        printf("pn: %d, db:%d", pn, db);
    }
}

//LRU: replaces the page that has not been used for the longest period of time

//Optimal:  replaces the page that will not be used for the longest period in the future