//Program to implement Dijkstra's algorithm to 
//find the shortest path between a given node to any node in
//the graph

#include <stdio.h>
#include <stdlib.h>

//List definitions

typedef struct linkedList* node;

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
	if(list == NULL)
		return x;
	node temp = list;
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = x;
	return list;
}

void printList(node list)
{
	node temp = list;
	while(temp != NULL)
	{
		printf("%d, %d   ", temp->key, temp->weight);
		temp = temp->next;
	}
	printf("\n");
}

//Algorithm implementation

void assignPath(node adjList[], int n, int k, int visited[], int pathlength[])
{
    visited[k] = 1;
    node temp = adjList[k];
    while(temp != NULL)
    {
        if(pathlength[temp->key] > pathlength[k] + temp->weight)
            pathlength[temp->key] = pathlength[k] + temp->weight;
        temp = temp->next;
    }
}

void assignPathall(node adjList[], int n, int k, int visited[], int pathlength[])
{
    for(int i = 0;i < n;i ++)
    {
        int min = 999;
        for(int j = 0;j < n;j ++)
            if(pathlength[j] < pathlength[min] && visited[j] == 0)
                min = j;
        if(pathlength[min] == 9999)
            break;
        visited[min] = 1;
        assignPath(adjList, n, min, visited, pathlength);
    }
}

void printPath(node adjList[], int n, int k)
{
    int pathLength[1000];
    int visited[1000] = {0};
    for(int i = 0;i < n;i++)
        pathLength[i] = 9999;
    pathLength[999] = 9999;
    pathLength[k] = 0;
    assignPathall(adjList, n, k, visited, pathLength);
    for(int i = 0;i < n;i ++)
    {
        printf("%d ", i);
        if(pathLength[i] != 9999)
            printf("%d\n", pathLength[i]);
        else
        {
            printf("INF\n");
        }
        
    }
}

//Main function

void main()
{
	int n, e;
	scanf("%d", &n);
	node adjList[1000] = {NULL};
    scanf("%d", &e);
    for(int i = 0;i < e;i ++)
    {
        int k1, k2, w;
        scanf("%d%d%d", &k1, &k2, &w);
        node temp = createNode(k2, w);
        adjList[k1] = insertNode(adjList[k1], temp);        
    }
	int k;
    scanf("%d", &k);
    printPath(adjList, n, k);
//    for(int i = 0;i<n; i++)
//        printList(adjList[i]);
}