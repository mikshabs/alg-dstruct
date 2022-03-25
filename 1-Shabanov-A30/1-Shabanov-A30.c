#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include "A30.h"

Node* CreateNode(int data) {
	Node* q;
	q = (Node*)malloc(sizeof(Node));
	if (q == NULL) {
		printf("Memory error");
		return NULL;
	}
	q->value = data;
	q->next = NULL;
	return q;
}
void PrintNode(Node* node) {
	while (node != NULL) {
		printf("%i ", node->value);
		node = node->next;
	}
	return 0;
}
void AddNode(Node** head, int  data) {
	Node* p = *head;
	Node* new_node = CreateNode(data);
	if (head == NULL || (*head)->value >= new_node->value) {
		new_node->next = *head;
		*head = new_node;
	}
	else {
		while (p->next != NULL && p->next->value < new_node->value) {
			p = p->next;
		}
		new_node->next = p->next;
		p->next = new_node;
	}
}
void DeleteNode(Node** head, int data) {
	Node* p = *head;
	Node* p1;
	Node* p2;
	if (p->value == data && p != NULL) {
		p1 = p;
		*head = p->next;
		free(p1);
		printf("Node is empty");
		p = NULL;
	}
	else {
		while (p->next != NULL) {
			if (p->next->value == data) {
				p2 = p->next;
				p->next = p->next->next;
				free(p2);
				break;
			}
			else
				p = p->next;
		}
	}
}
void ClearNode(Node* node) {
	while (node != NULL) {
		Node* p = node;
		node = node->next;
		free(p);
	}
}
int IsElement(Node* node, int data) {
	while (node != NULL) {
		if (node->value == data)
			return 1;
		node = node->next;
	}
	return 0;
}
void PushNode(Node** head, int data) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) {
		printf("Allocation memory error!");
		exit();
	}
	new_node->value = data;
	new_node->next = (*head);
	(*head) = new_node;
}
void ReverseNode(Node** head) {
	Node* p1, * p2;
	Node* p = NULL;
	p1 = *head;
	while (p1 != NULL) {
		p2 = p1->next;
		p1->next = p;
		p = p1;
		p1 = p2;
	}
	*head = p;
}
Node* GetNodeIntersection(Node** head1, Node** head2) {
	Node* result = NULL;
	Node* p1 = *head1;
	while (p1 != NULL) {
		if (IsElement(*head2, p1->value))
			PushNode(&result, p1->value);
		p1 = p1->next;
	}
	Node** p = &result;
	ReverseNode(p);
	return *p;
}
Node* GetNodeUnion(Node** head1, Node** head2) {
	Node* result = NULL;
	Node* p1 = *head1, * p2 = *head2;
	while (p1 != NULL && p2 != NULL) {
		if (p1->value < p2->value) {
			PushNode(&result, p1->value);
			p1 = p1->next;
		}
		else
			if (p1->value > p2->value) {
				PushNode(&result, p2->value);
				p2 = p2->next;
			}
			else
				if (p1->value < p2->value || p1->value == p2->value) {
					PushNode(&result, p2->value);
					p1 = p1->next;
					p2 = p2->next;
				}
	}
	while (p1 != NULL) {
		PushNode(&result, p1->value);
		p1 = p1->next;
	}
	while (p2 != NULL) {
		PushNode(&result, p2->value);
		p2 = p2->next;
	}
	Node** p = &result;
	ReverseNode(p);
	return *p;
}