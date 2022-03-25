#include <gtest\gtest.h>
extern "C"
{
#include "A30.h"
}
TEST(CreatNode, Data_CorrectNode) {
	Node* node = CreateNode(1);
	EXPECT_TRUE(node != NULL);
	EXPECT_FALSE(node->value == 5);
	ASSERT_TRUE(node->value == 1);
	EXPECT_TRUE(node->next == NULL);
}
TEST(AddNode, Node_Added) {
	Node* node = CreateNode(1);
	Node** point = &(node);
	AddNode(point, 5);
	EXPECT_TRUE((*point)->value == 1);
	(*point) = (*point)->next;
	EXPECT_TRUE((*point)->value == 5);
	EXPECT_TRUE((*point)->next == NULL);
}
TEST(DeleteNode, FirstNode_Deleted) {
	Node* node = CreateNode(1);
	Node** point = &(node);
	DeleteNode(point, 1);
	EXPECT_TRUE(*point == NULL);
}
TEST(DeleteNode, SecondNode_Deleted) {
	Node* node = CreateNode(1);
	Node** point = &(node);
	AddNode(point, 5); DeleteNode(point, 5);
	EXPECT_TRUE((*point)->value == 1);
	EXPECT_TRUE((*point)->next == NULL);
}
TEST(DeleteNode, NoNode_Deleted) {
	Node* node = CreateNode(1);
	Node** point = &(node);
	AddNode(point, 5);
	DeleteNode(point, 6);
	EXPECT_TRUE((*point)->value == 1);
	EXPECT_TRUE((*point)->next->value == 5);
	EXPECT_TRUE((*point)->next->next == NULL);
}
TEST(PushNode, Node_Pushed) {
	Node* node = CreateNode(1);
	Node** point = &(node);
	PushNode(point, 6);
	EXPECT_TRUE((*point)->value == 6);
	EXPECT_TRUE((*point)->next->value == 1);
	EXPECT_TRUE((*point)->next->next == NULL);
}
TEST(IsElement, ElementIn) {
	Node* node = CreateNode(1);
	int k = IsElement(node, 1);
	EXPECT_TRUE(k == 1);
}
TEST(IsElement, ElementOut) {
	Node* node = CreateNode(1);
	int d = IsElement(node, 6);
	EXPECT_TRUE(d == 0);
}
TEST(GetNodeIntersection, NodeInter) {
	Node* node1 = CreateNode(1);
	Node** point1 = &(node1);
	Node* node2 = CreateNode(1);
	Node** point2 = &(node2);
	AddNode(point1, 2);
	AddNode(point1, 3);
	AddNode(point2, 5);
	AddNode(point2, 3);
	Node* node_inter = GetNodeIntersection(point1, point2);
	EXPECT_TRUE(node_inter->value == 1);
	EXPECT_TRUE(node_inter->next->value == 3);
	EXPECT_TRUE(node_inter->next->next == NULL);
}
TEST(GetNodeUnion, NodeUnion) {
	Node* node1 = CreateNode(1);
	Node** point1 = &(node1);
	Node* node2 = CreateNode(1);
	Node** point2 = &(node2);
	AddNode(point1, 2);
	AddNode(point1, 3);
	AddNode(point2, 5);
	AddNode(point2, 3);
	Node* node_union = GetNodeUnion(point1, point2);
	EXPECT_TRUE(node_union->value == 1);
	node_union = node_union->next;
	EXPECT_TRUE(node_union->value == 2);
	node_union = node_union->next;
	EXPECT_TRUE(node_union->value == 3);
	node_union = node_union->next;
	EXPECT_TRUE(node_union->value == 5);
	EXPECT_TRUE(node_union->next == NULL);
}
TEST(ReverseNode, NodeReverse) {
	Node* node = CreateNode(1);
	Node** point = &(node);
	AddNode(point, 2);
	AddNode(point, 3);
	ReverseNode(point);
	EXPECT_TRUE((*point)->value == 3);
	(*point) = (*point)->next;
	EXPECT_TRUE((*point)->value == 2);
	(*point) = (*point)->next;
	EXPECT_TRUE((*point)->value == 1);
	EXPECT_TRUE((*point)->next == NULL);
}