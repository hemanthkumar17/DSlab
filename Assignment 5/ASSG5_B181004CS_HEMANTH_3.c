//Build minimum spanning tree
//Apply dijkstras algorithm find the path lengths with vertex 0
//for each remaining edges (u, v)
//pathlength[u] - pathlength[v] + weight(edge(u,v)) = cycle value

//Program to implement Dijkstra's algorithm to
//find the shortest path between a given node to any node in
//the graph

#include <stdio.h>
#include <stdlib.h>

//List definitions

typedef struct linkedList *node;

struct linkedList
{
    int key;
    int weight;
    node next;
};

node createNode(int key, int weight)
{
    node temp = (node)malloc(sizeof(struct linkedList));
    temp->key = key;
    temp->weight = weight;
    temp->next = NULL;
    return temp;
}

node insertNode(node list, node x)
{
    if (list == NULL)
        return x;
    node temp = list;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = x;
    return list;
}

node deleteNode(node head, int key)
{
    // Store head node
    node temp = head, prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->key == key)
    {
        head = temp->next; // Changed head
        free(temp);        // free old head
        return head;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->key != key)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return head;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
    return head;
}

void printList(node list)
{
    node temp = list;
    if (temp == NULL)
        printf("NULL");
    while (temp != NULL)
    {
        printf("%d, %d   ", temp->key, temp->weight);
        temp = temp->next;
    }
    printf("\n");
}

//Algorithm implementation - Kruskal's algorithm

void assignPath(node adjList[], int n, int k, int visited[], int pathlength[])
{
    visited[k] = 1;
    node temp = adjList[k];
    while (temp != NULL)
    {
        if (pathlength[temp->key] > pathlength[k] + temp->weight)
            pathlength[temp->key] = pathlength[k] + temp->weight;
        temp = temp->next;
    }
}

void assignPathall(node adjList[], int n, int k, int visited[], int pathlength[])
{
    for (int i = 0; i < n; i++)
    {
        int min = 999;
        for (int j = 0; j < n; j++)
            if (pathlength[j] < pathlength[min] && visited[j] == 0)
                min = j;
        if (pathlength[min] == 9999)
            break;
        assignPath(adjList, n, min, visited, pathlength);
    }
}

//Algorithm implementation - Dijkstra's algorithm

//To sort the list of edges for a particular vertex
node insertionSort(node list, node newList)
{
    if (list == NULL)
        return newList;
    node temp1 = list, temp2 = list;
    node min = temp1;
    int flag = 1;
    while (temp2 != NULL)
    {
        if (min->weight > temp2->weight)
        {
            min = temp1;
            flag = 0;
        }
        if (temp1 != temp2)
            temp1 = temp1->next;
        temp2 = temp2->next;
    }
    //min has the element previous to the min element if flag = 0
    //flag = 1 when min is at head and so min contains head element

    if (flag == 1)
    {
        list = list->next;
        min->next = NULL;
        newList = insertNode(newList, min);
    }
    else
    {
        temp1 = min->next;
        min->next = temp1->next;
        temp1->next = NULL;
        newList = insertNode(newList, temp1);
    }
    return insertionSort(list, newList);
}

//Find the smallest edge and return index

int getEdge(node adjList[], int n)
{
    int min = 0;
    for (int i = 1; i < n; i++)
    {
        if (adjList[i] == NULL)
        {
            if (min == i)
                min = i + 1;
            continue;
        }
        if (adjList[i]->weight < adjList[min]->weight)
            min = i;
    }
    return min;
}

//To check if 2 points are connected
void connsearch(node G[], int n, int u, int visited[])
{
    visited[u] = 1;
    node temp = G[u];
    while (temp != NULL)
    {
        if (visited[temp->key] == 0)
            connsearch(G, n, temp->key, visited);
        temp = temp->next;
    }
}

int connected(node G[], int n, int u, int v)
{
    int visited[1000] = {0};
    connsearch(G, n, u, visited);
    if (visited[v] == 1)
        return 1;
    return 0;
}

//Make a minimum spanning tree
//This function has been modified to serve our current algorithm

int checkNegCycle(node adjList[], int n, node residue[])
{
    node tree[n];
    for (size_t i = 0; i < n; i++)
    {
        tree[i] = NULL;
    }

    for (int i = 0; i < n - 1; i++)
    {
        //		printf("%d\n", i);
        int min = getEdge(adjList, n);
        //check if the edge is needed ie if both vertices are visited
        node temp = adjList[min];
        adjList[min] = adjList[min]->next;
        //		printf("Min1 = %d, Min2 = %d\n", min, temp->key);
        if (!connected(tree, n, temp->key, min) && !connected(tree, n, min, temp->key)) //Running DFS to find if the edge will form a cycle
        {
            // printf("YES = %d %d\n", temp->key, temp->weight);
            temp->next = tree[min];
            tree[min] = temp;
            residue[min] = deleteNode(residue[min], temp->key);
        }
        else
            i--;
    }
    //Minimum spanning tree formed in tree
    //Residue left in adjList

    //Using the built structures and key find the weight of all cycles
    //We print 1 and exit if the weight is negative

    for (int i = 0; i < n; i++)
    {
        node temp = tree[i];
        while (temp != NULL)
        {
            node temp2 = residue[temp->key];
            while (temp2 != NULL)
            {
                if (temp2->key == i)
                {
                    node temp3 = createNode(temp2->key, temp2->weight);
                    tree[temp->key] = insertNode(tree[temp->key], temp3);
                    residue[temp->key] = deleteNode(residue[temp->key], temp2->key);
                    break;
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }
    for (int i = 0; i < n; i++)
    {
        node temp = residue[i];
        while (temp != NULL)
        {
            //Applying dijkstra's algorithm
            int k = temp->key;
            int pathLength[1000];
            int visited[1000] = {0};
            for (int i = 0; i < n; i++)
                pathLength[i] = 9999;
            pathLength[999] = 9999;
            pathLength[k] = 0;
            assignPathall(tree, n, k, visited, pathLength);
            int w = pathLength[i] + temp->weight;

            node temp3 = createNode(temp->key, temp->weight);
            tree[i] = insertNode(tree[i], temp3);
            residue[i] = residue[i]->next;

            // printf(" %d + %d =  %d\n", pathLength[i], temp->weight, w);
            if (pathLength[temp->key] >= 9999)
            {
                temp = temp->next;
                continue;
            }

            if (w < 0)
            {
                printf("1\n");
                return 1;
            }
            temp = temp->next;
        }


    }
    printf("-1\n");
    return 0;
}

//Main function

void main()
{
    int n, e;
    scanf("%d %d", &n, &e);
    node adjList[1000] = {NULL};
    node residue[1000] = {NULL};
    for (int i = 0; i < e; i++)
    {
        int k1, k2, w;
        scanf("%d%d%d", &k1, &k2, &w);
        node temp = createNode(k2, w);
        adjList[k1] = insertNode(adjList[k1], temp);
        temp = createNode(k2, w);
        residue[k1] = insertNode(residue[k1], temp);
    }
    for (int i = 0; i < n; i++)
        adjList[i] = insertionSort(adjList[i], NULL);
    checkNegCycle(adjList, n, residue);
    //    for(int i = 0;i<n; i++)
    //        printList(adjList[i]);
}