//Mohammed Saada
//Project 4 - First Semester 2024/2025
//Network packet Routing System
//using BFS and Dijkstra Algorithms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct node{
    int index;
    char router;
    struct node* next;
};
struct queue{
char token;
struct queue* next;
struct queu* front;
struct queue* rear;
};

//struct for each cell in the heap
struct Cell{
    char router;
    int cost;
};
typedef struct Cell cell;

//struct for the Heap [minHeap]
struct minHeap{
    int size;
    int max_size;
    cell* heapArray;
};
typedef struct minHeap* Heap;

struct path{
    int total_cost;
    char* shortest_path;
    int nodes;
};
typedef struct path Path;
typedef struct queue* ptq;
typedef struct node* list;


//function prototype
list createLinkedList();
int searchInList(char m,list L);
void insertToList(char m, list L);
list StoreNodes(char* input);
int FindNumOfNodes(list L);
int findIndex(char, list);
void buildGraph(char* input, list L, int array[FindNumOfNodes(L)][FindNumOfNodes(L)]);
ptq createQueue();
void EnQueue(ptq Q, char tok);
ptq DeQueue(ptq Q);
void findAdjacent(int node, int size, int matrix[size][size], int adjacent[size]);
char FindNode(int index, list L);
Path* BFS(int size, int graph[size][size], list L, char source, char distination);
Heap CreateHeap(int maxSize);
int parent(int pos);
int rightChild(int pos);
int isLeaf(int pos, Heap H);
Heap swap(int pos1, int pos2, Heap H);
Heap insertToHeap(char router, int distance, Heap H);
cell deleteFromHeap(Heap H);
Heap minHeapify(int pos, Heap H);
Heap updateCost(char router, int newCost, Heap H);
Path* dijkstra(int size, int graph[size][size], list L, char source, char destination);
void saveTofile(char source,char destination, Path* dijkstra_Path,Path *BFS_Path);
int check_If_exist(char m, list L);
void menu();

int main()
{
    printf("Network Routing packet system ...\n\n");
    int choice, size;
    int fileRead = 0;
    list L;
    char source = NULL;
    char destination, temp_source, temp_destination;
    menu();
    scanf("%d",&choice);
    L = StoreNodes("routers.txt");
    size = FindNumOfNodes(L);
    int matrix[size][size];
    Path* dijkstra_Path = NULL;
    Path* BFS_Path = NULL;
    while(choice != 4){
        switch(choice){
            case 1:
                    buildGraph("routers.txt",L, matrix);
                    printf("the File Loaded Successfully..\n");
                    fileRead = 1;
                    break;
            case 2: if(fileRead == 0){
                    printf("You should Load the data from file before this step!\n");
                    }
                    else{
                        printf("Enter the source router please\n");
                        getchar();
                        scanf("%c",&source);
                        if(check_If_exist(source,L) == -1){
                            printf("This Router isn't exist, try again!!\n");
                            source = NULL;
                        }
                    }
                    break;
            case 3: if(fileRead == 0){
                    printf("You should Load the data from file before this step!\n");
                    }
                    else if(source == NULL)
                        printf("You should Enter 2 to choose the source router before this step!\n");
                    else{
                        printf("Enter the destination router please\n");
                        getchar();
                        scanf("%c",&destination);
                        if(check_If_exist(destination,L) == -1){
                            printf("This Router isn't exist, try again!!\n");
                        }
                        else{
                            dijkstra_Path = dijkstra(size,matrix,L,source,destination);
                            BFS_Path = BFS(size,matrix,L,source,destination);
                            temp_source = source;
                            temp_destination = destination;
                        }
                    }
                    break;
            default: printf("Invalid Choice, try again!!\n");
        }
        menu();
        while(scanf("%d",&choice)!= 1){
                getchar();
            printf("Invalid input, try again!!\n");
            menu();
        }
    }
        if(dijkstra_Path == NULL){
                printf("There is No Data to Save to the file, because you does not choose 3 previously\n");
                printf("Exit from the Program ...\n");
        }
        else{
                saveTofile(temp_source,temp_destination,dijkstra_Path,BFS_Path);
                printf("Saving data to the file...\n");
                printf("Exit from the Program ...\n");
        }

        list t;
        while(L != NULL){
            t = L->next;
            free(L);
            L = t;
        }

        free(dijkstra_Path->shortest_path);
        free(dijkstra_Path);
        free(BFS_Path->shortest_path);
        free(BFS_Path);
    return 0;
}

//function to display menu
void menu(){
    printf("**** Menu: ****\n");
    printf("1. Load routers\n");
    printf("2. Enter source router\n");
    printf("3. Enter destination router\n");
    printf("4. Exit and store the result of 3 in output file (shortest_distance.txt)\n");
}

int check_If_exist(char m, list L){
    list ptr = L->next;
    while(ptr!=NULL){
        if(ptr->router == m)
            return 1;
        ptr = ptr->next;
    }
    return -1;
}

void saveTofile(char source,char destination, Path* dijkstra_Path,Path *BFS_Path){
    FILE* file = fopen("shortest_distance.txt","w");
    if(file == NULL){
        printf("Error to open the output file to write!\n");
        return;
    }
    fprintf(file,"Shortest Path From router %c to router %c:\n",source,destination);
    fprintf(file,"Dijkstra Algorithm:\n");
    if(dijkstra_Path->total_cost == INT_MAX || dijkstra_Path->nodes == 0)
        fprintf(file,"NO PATH !!!\n");
    else{
        int i =0;
        for(i=0; i< dijkstra_Path->nodes -1 ; i++)
            fprintf(file,"%c -> ",dijkstra_Path->shortest_path[i]);
        fprintf(file,"%c\n",dijkstra_Path->shortest_path[i]);
        fprintf(file,"with total Cost of: %d\n",dijkstra_Path->total_cost);
    }
    fprintf(file,"-------------------------------------------------\n");
    fprintf(file,"BFS Algorithm:\n");
    if(BFS_Path->total_cost == INT_MAX || BFS_Path->nodes == 0)
        fprintf(file,"NO PATH !!!\n");
    else{
        int i =0;
        for(i=0; i< BFS_Path->nodes -1 ; i++)
            fprintf(file,"%c -> ",BFS_Path->shortest_path[i]);
        fprintf(file,"%c\n",BFS_Path->shortest_path[i]);
        fprintf(file,"with total Cost of: %d\n",BFS_Path->total_cost);
    }
}
//function to create linked list
list createLinkedList(){
    list head = (list)malloc(sizeof(struct node));
    if(head == NULL){
        printf("Out of Memory Space..\n");
    }
    else{
        head->index = -1;
        head->next = NULL;
    }
    return head;
};

//function to search in a linked list
int searchInList(char m,list L){
    list ptr = L->next;
    while(ptr != NULL){
        if(m == ptr->router)
            return ptr->index;
        ptr = ptr->next;
    }
    return -1;
}

//function to insert to the linked list
void insertToList(char m, list L){
    list ptr = L;
    while(ptr->next != NULL)
        ptr = ptr->next;
    list newNode = (list)malloc(sizeof(struct node));
    newNode->index = ptr->index + 1;
    newNode->router = m;
    newNode->next = NULL;
    ptr->next = newNode;
}

//function to store the nodes from the input file
list StoreNodes(char* input){
    FILE* file = fopen(input,"r");
    if(file == NULL){
        printf("The file does not exist\n");
        return -1;
    }
    list L = createLinkedList();
    char ch[10];
    char* From;
    char* To;
    while(fgets(ch,sizeof(ch),file)){
        From = strtok(ch,"-");
        To = strtok(NULL,"-");
        if(searchInList(From[0],L) == -1)
            insertToList(From[0],L);
        if(searchInList(To[0],L) == -1)
            insertToList(To[0],L);
    }
    fclose(file);
    return L;
}

//function to find the number of nodes
int FindNumOfNodes(list L){
    list ptr = L;
    while(ptr->next != NULL)
        ptr = ptr->next;
    return ptr->index + 1;
}

//function to find the index of the node in adjacency matrix
int findIndex(char m, list L){
    list ptr = L->next;
    while(ptr != NULL){
        if(m == ptr->router)
            return ptr->index;
        ptr = ptr->next;
    }
    return -1;
}

//function to build the graph
void buildGraph(char* input, list L, int array[FindNumOfNodes(L)][FindNumOfNodes(L)]){
    int size = FindNumOfNodes(L);
    FILE* file = fopen(input,"r");
    if(file == NULL){
        printf("The file does not exist\n");
        return -1;
    }
    //initialize the adjacency matrix with infinity
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++)
            array[i][j] = INT_MAX;
    }
    char ch[10];
    char From;
    char To;
    char* dist;
    int Latency, i, j;
    while(fgets(ch,sizeof(ch),file)){
        From = strtok(ch,"-")[0];
        To = strtok(NULL,"-")[0];
        dist = strtok(NULL,"-");
        Latency = atoi(dist);
        i = findIndex(From,L);
        j = findIndex(To,L);
        array[i][j] = Latency;
        array[j][i] = Latency;
    }
    fclose(file);
}

//functions for queue
//function to create Queue
ptq createQueue(){
    ptq T = (ptq)malloc(sizeof(struct queue));
    if(T==NULL){
        printf("Queue can not be defined..\n");
        return NULL;
        }
    else {
        T->next = NULL;
        T->front = T->next;
        T->rear = T->next;
        return T;
        }
}

//function to enqueue node to queue
void EnQueue(ptq Q, char tok){
    if(Q == NULL)
        printf("Queue not exist..\n");
    else {
            ptq New = (ptq)malloc(sizeof(struct queue));
            New->token = tok;
            New->front = NULL;
            New->rear = NULL;
            ptq temp = Q->rear;
            if(temp != NULL){
                temp->next = New;
                New->next = NULL;
                Q->rear = New;
                Q->front = Q->next;
            }
            else {
                Q->next = New;
                New->next = NULL;
                Q->front = New;
                Q->rear = New;
            }
    }

}

//function to dequeue node from queue
ptq DeQueue(ptq Q){
    if(Q == NULL){
        printf("Queue not exist..\n");
        return NULL;
    }
    else {
        if(Q->front == NULL){
            printf("Queue is Empty..\n");
            return NULL;
        }
        else {
            ptq temp = Q->front;
            Q->next = temp->next;
            Q->front = Q->next;
            if(Q->front==NULL)
                Q->rear = NULL;
            temp->next = NULL;
            return temp;
        }
    }
}

//function to find adjacent to a node
void findAdjacent(int node, int size, int matrix[size][size], int adjacent[size]){
    int counter = 0;
    for(int i=0; i<size; i++){
        if(matrix[node][i] == INT_MAX)
            continue;
        //else the node is adjacent
        adjacent[counter++] = i;
    }
    adjacent[counter] = -1;
}

//function to find node with specific index
char FindNode(int index, list L){
    list ptr = L->next;
    while(ptr != NULL){
        if(ptr->index == index)
            return ptr->router;
        ptr = ptr->next;
    }
    return NULL;
}
//functions for BFS
Path* BFS(int size, int graph[size][size], list L, char source, char destination){
    int visited[size];
    int cost[size];
    char path[size];
    // define all nodes as not visited
    for(int i=0; i<size; i++){
        visited[i] = 0;
        cost[i] = INT_MAX;
        path[i] = NULL;
    }
    //create a queue
    ptq Q = createQueue();
    int i, j;
    int adjacent[size];
    //define the source as visited and enqueue it to the queue
    i = findIndex(source,L);
    visited[i] = 1;
    cost[i] = 0;
    path[i] = source;
    EnQueue(Q,source);
    ptq temp;
    while(Q->front != NULL){
        temp = DeQueue(Q);
        i = findIndex(temp->token,L);
        findAdjacent(i,size,graph,adjacent);
        j=0;
        while(adjacent[j] != -1){

            if(visited[adjacent[j]] == 0){
                visited[adjacent[j]] = 1;
                cost[adjacent[j]] = cost[i] + graph[i][adjacent[j]];
                path[adjacent[j]] = temp->token;
                EnQueue(Q,FindNode(adjacent[j],L));
            }
            j++;
        }
    }
    //find the shortest path
    Path* K = (Path*)malloc(sizeof(Path));
    K->nodes = 0;
    char shortestPath[size];
    shortestPath[0] = destination;
    i = 0;
    j = findIndex(destination,L);
    int f=0;
    if(cost[j] == INT_MAX){
        printf("There is no path from %c to %c (BFS)\n",source,destination);
        K->total_cost = INT_MAX;
        return K;
    }
    while(path[j] != source){
        shortestPath[++i] = path[j];
        j = findIndex(path[j],L);
    }
    shortestPath[++i] = source;
    //print the shortest path from the Source to Destination
    K->shortest_path = (char*)malloc(sizeof(char)*(i+1));
    printf("From %c to %c:\n",source,destination);
    printf("The shortest path using BFS is: ");
    while(i > 0){
        printf("%c -> ",shortestPath[i]);
        K->shortest_path[f++] = shortestPath[i--];
        K->nodes ++;
    }
    printf("%c\n",shortestPath[i]);
    K->shortest_path[f] = shortestPath[i];
    K->nodes++;
    K->total_cost = cost[findIndex(destination,L)];
    printf("With a total cost of %d\n",cost[findIndex(destination,L)]);
    ptq t;
    while(Q != NULL){
        t = Q->next;
        free(Q);
        Q = t;
    }
    return K;
}

//functions for minheap
//function to create the heap
Heap CreateHeap(int maxSize){
    Heap H = (Heap)malloc(sizeof(struct minHeap));
    H->heapArray = (cell*)malloc(sizeof(cell)*(maxSize+1));

    if(H == NULL){
        printf("Out of memory..\n");
        return NULL;
    }
    H->max_size = maxSize+1;
    H->size = 0;
    H->heapArray[0].router = NULL;
    H->heapArray[0].cost = -1;
    return H;
}

//function to find the parent of a node in the heap
int parent(int pos){
    return (pos/2);
}

//function to find the left child for such node in heap
int leftChild(int pos){
    return (pos*2);
}

//function to find the right child for such node in heap
int rightChild(int pos){
    return ((pos*2) + 1);
}

//function that return true if the node in heap is leaf or false if not leaf
int isLeaf(int pos, Heap H){
    if(leftChild(pos) <= H->size || rightChild(pos) <= H->size)
        return 0;
    return 1;
}

//function to swap two nodes in the heap
Heap swap(int pos1, int pos2, Heap H){
    cell temp;
    temp.router = H->heapArray[pos1].router;
    temp.cost = H->heapArray[pos1].cost;

    H->heapArray[pos1].router = H->heapArray[pos2].router;
    H->heapArray[pos1].cost = H->heapArray[pos2].cost;

    H->heapArray[pos2].router = temp.router;
    H->heapArray[pos2].cost = temp.cost;

    return H;
}

//function to insert new node to the heap
Heap insertToHeap(char router, int cost, Heap H){
    if(H->size >= H->max_size)
        return H;
    int current = ++H->size;
    H->heapArray[current].router = router;
    H->heapArray[current].cost = cost;

    while(H->heapArray[current].cost < H->heapArray[parent(current)].cost){
        H = swap(current,parent(current),H);
        current = parent(current);
    }
    return H;
}
cell deleteFromHeap(Heap H){
    if(H->size == 0){
        printf("The Heap is empty!!\n");
        return;
    }
    cell temp;
    temp.router = H->heapArray[1].router;
    temp.cost = H->heapArray[1].cost;
    int current = 1;
    while(!isLeaf(current,H)){
        if(rightChild(current) <= H->size && leftChild(current) <= H->size){
            if(H->heapArray[leftChild(current)].cost < H->heapArray[rightChild(current)].cost){
                H = swap(current,leftChild(current),H);
                current = leftChild(current);
            }
            else{
                H = swap(current,rightChild(current),H);
                current = rightChild(current);
            }
        }
        else if(leftChild(current) <= H->size){
            H = swap(current,leftChild(current),H);
            current = leftChild(current);
        }
        else{
            H = swap(current,rightChild(current),H);
            current = rightChild(current);
        }
    }
    while(current < H->size){
        H = swap(current,current+1,H);
        current = current + 1;
    }

    H->size--;
    return temp;
}

//function to minHeapify the heap to be valid minHeap
Heap minHeapify(int pos, Heap H){
    if(!isLeaf(pos,H)){
        if(leftChild(pos) <= H->size && rightChild(pos) <= H->size){
            if(H->heapArray[pos].cost > H->heapArray[leftChild(pos)].cost || H->heapArray[pos].cost > H->heapArray[rightChild(pos)].cost){
                if(H->heapArray[leftChild(pos)].cost < H->heapArray[rightChild(pos)].cost){
                    H = swap(pos,leftChild(pos),H);
                    H = minHeapify(leftChild(pos),H);
                }
                else {
                    H = swap(pos,rightChild(pos),H);
                    H = minHeapify(rightChild(pos),H);
                }
            }
        }
        else if(leftChild(pos) <= H->size && H->heapArray[leftChild(pos)].cost < H->heapArray[pos].cost){
            H = swap(pos,leftChild(pos),H);
            H = minHeapify(leftChild(pos),H);
        }
        else if(rightChild(pos) <= H->size && H->heapArray[rightChild(pos)].cost < H->heapArray[pos].cost){
            H = swap(pos,rightChild(pos),H);
            H = minHeapify(rightChild(pos),H);
        }
    }
    return H;
}

Heap updateCost(char router, int newCost, Heap H){
    for(int i=1; i<=H->size; i++){
        if(router == H->heapArray[i].router){
            H->heapArray[i].cost = newCost;
            break;
        }
    }
    //minheapify the heap after the update
    for(int i = H->size/2; i > 0; i--)
        H = minHeapify(i,H);
    return H;
}

Path* dijkstra(int size, int graph[size][size], list L, char source, char destination){
    int known[size];
    int cost[size];
    char path[size];
    Heap H = CreateHeap(size);
    //initialize all nodes as unknown with cost infinity and path NULL
    //and add all vertices to the heap
    for(int i=0; i<size; i++){
        known[i] = 0;
        cost[i] = INT_MAX;
        path[size] = NULL;
        H = insertToHeap(FindNode(i,L),INT_MAX,H);
    }
    //update the source to be with cost 0 and minheapify the heap
    H = updateCost(source,0,H);
    cost[findIndex(source,L)] = 0;
    path[findIndex(source,L)] = source;
    list List = createLinkedList();
    cell temp;
    int adjacent[size];
    int i,j, newCost;
    while(H->size > 0){
        temp = deleteFromHeap(H);
        if(temp.cost == INT_MAX)
            break;
        findAdjacent(findIndex(temp.router,L),size,graph,adjacent);
        j = 0;
        while(adjacent[j] != -1){
            newCost = temp.cost + graph[findIndex(temp.router,L)][adjacent[j]];
            if(newCost < cost[adjacent[j]]){
                cost[adjacent[j]] = newCost;
                path[adjacent[j]] = temp.router;
                H = updateCost(FindNode(adjacent[j],L), newCost,H);
            }
        j++;
        }
        insertToList(temp.router,List);
        known[findIndex(temp.router,L)] = 1;
    }
    Path* K = (Path*)malloc(sizeof(Path));
    K->nodes = 0;
    //find the shortest path
    char shortestPath[size];
    shortestPath[0] = destination;
    i = 0;
    j = findIndex(destination,L);
    int f = 0;
    if(cost[j] == INT_MAX){
        printf("There is no path from %c to %c (Dijkstra)\n",source,destination);
        K->total_cost = INT_MAX;
        return K;
    }
    while(path[j] != source){
        shortestPath[++i] = path[j];
        j = findIndex(path[j],L);
    }
    shortestPath[++i] = source;
    //print the shortest path from the Source to Destination
    printf("From %c to %c:\n",source,destination);
    printf("The shortest path using Dijkstra is: ");
    K->shortest_path = (char*) malloc(sizeof(char) * (i));
    while(i > 0){
        printf("%c -> ",shortestPath[i]);
        K->shortest_path[f++] = shortestPath[i--];
        K->nodes++;
    }
    printf("%c\n",shortestPath[i]);
    K->shortest_path[f] = shortestPath[i];
    K->nodes++;
    K->total_cost = cost[findIndex(destination,L)];
    printf("With a total cost of %d\n",cost[findIndex(destination,L)]);

    free(H->heapArray);
    free(H);
    return K;
}

