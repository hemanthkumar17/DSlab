//Program to implement Prim's algorithm to 
//find the sum of edge weights of theminimum spanning tree

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

node createNode(int key)
{
	node temp = (node)malloc(sizeof(struct linkedList));
	temp->key = key;
	temp->weight = 1;
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

//To sort the list of edges for a particular vertex
node insertionSort(node list, node newList)
{
	if(list == NULL)
		return newList;
	node temp1 =list, temp2 = list;
	node min = temp1;
	int flag = 1;
	while(temp2 != NULL)
	{
		if(min->weight > temp2->weight)
		{   
			min = temp1;
			flag = 0;
		}
		if(temp1 != temp2)
			temp1 = temp1->next;
		temp2 = temp2->next;
	}
	//min has the element previous to the min element if flag = 0
	//flag = 1 when min is at head and so min contains head element

	if(flag == 1)
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

int getEdge(node adjList[], int n, int visited[1000])
{
	int min = 0;
	for(int i = 1;i < n;i ++)
	{

		if(adjList[i] == NULL)
		{
			if(min == i)
				min = i+1;
			continue;
		}

        if(visited[i] == 0)
            continue;

		if(adjList[i]->weight < adjList[min]->weight)
			min = i;
	}
	return min;
}

//To check if 2 points are connected
void connsearch(node G[], int n, int u, int visited[]){
	visited[u] = 1;
	node temp = G[u];
	while(temp != NULL)
	{
		if(visited[temp->key] == 0)
			connsearch(G, n, temp->key, visited);
		temp = temp-> next;
	}
}

int connected(node G[], int n, int u, int v){
	int visited[1000] = {0};
	connsearch(G, n, u, visited);
	if(visited[v] == 1)
		return 1;
	return 0;
}

//Make a minimum spanning tree

int minspan(node adjList[], int n)
{
	node tree[n];
	node tempList[n];
	int visited[1000] = {0};
	for (size_t i = 0; i < n; i++)
		tempList[i] = adjList[i];
	for(int i = 0;i < n-1;i ++)
	{
//		printf("%d\n", i);
		int min = getEdge(tempList, n, visited);
		//check if the edge is needed ie if both vertices are visited
		node temp = tempList[min];
		node temp1 = tempList[temp->key];
        node temp2 = temp1;
        while(temp1->weight != temp->weight && temp1 != 0)
        {
            if(temp2 != temp1)
                temp2 = temp2->next;
            temp1 = temp1->next;
        }
        if(temp2 == temp1)
            tempList[temp->key] = tempList[temp->key]->next;
        else
        {
            temp2->next = temp1->next;
        }
        
		tempList[min] = tempList[min]->next;
//		printf("Min1 = %d, Min2 = %d\n", min, temp->key);
		if(!connected(tree, n, temp->key, temp1->key))			//Running DFS to find if the edge will form a cycle 
		{
			visited[min] = 1;
			visited[temp->key] = 1;
//			printf("YES = %d %d\n", temp->key, temp->weight);
			temp->next = tree[min];
			tree[min] = temp;
			temp1->next = tree[temp->key];
			tree[temp->key] = temp1;            
		}
		else
			i--;

//		for(int j = 0;j<n; j++)
//			printList(tempList[j]);
	}
	int sum = 0;
	for(int i = 0;i < n;i ++)
	{
		node temp = tree[i];
		while(temp != NULL)
		{
			sum+= temp->weight;
			temp = temp->next;
		}
	}
	printf("%d\n", sum/2);
	return sum/2;
}

//Main function 


void main()
{
	int n;
	scanf("%d", &n);
	node adjList[n];
	for(int i = 0;i < n;i ++)
	{
		adjList[i] = NULL;
		node temp = NULL;
		int k;
		char ch = ' ';
		while(ch != '\n')
		{
			scanf("%d", &k);
			scanf("%c", &ch);
			if(k >= n)
			{
				printf("Invalid key value: %d, ignoring it\n", k);
				continue;
			}
			else{
			if(temp == NULL)
			{
				adjList[i] = createNode(k);
				temp = adjList[i];
			}
			else
			{
				temp->next = createNode(k);
				temp = temp->next;
			}
			}
		}

	}

	for(int i = 0;i < n;i ++)
	{
		node temp = adjList[i];
		int k;
		while(temp != NULL)
		{
			scanf("%d", &k);
			temp->weight = k;
			temp = temp->next;
		}
		adjList[i] = insertionSort(adjList[i], NULL);        
	}
	minspan(adjList, n);
//    for(int i = 0;i<n; i++)
//        printList(adjList[i]);

}