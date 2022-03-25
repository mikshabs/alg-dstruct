#pragma once
typedef struct Node {
	int value;
	struct Node* next;
} Node;
Node* CreateNode(int data);
void PrintNode(Node* node);
void AddNode(Node** head, int  data);
void DeleteNode(Node** head, int data);
void ClearNode(Node* node);
int IsElement(Node* node, int data);
void ReverseNode(Node** head);
void PushNode(Node** head, int data);
Node* GetNodeIntersection(Node** head1, Node** head2);
Node* GetNodeUnion(Node** head1, Node** head2);