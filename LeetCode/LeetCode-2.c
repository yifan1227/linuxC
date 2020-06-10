#include <stdlib.h>
#include <stdio.h>

struct ListNode
{
	int val;
	struct ListNode* next;
};

#define node_t struct ListNode*
node_t res = (node_t)malloc(sizeof(struct ListNode));

node_t create(int val, node_t head)
{
	node_t new_node = (node_t)malloc(sizeof(struct ListNode));
	new_node->next = NULL;
	new_node->val = val;
	while (head->next)
	{
		head = head->next;
	}
	head->next = new_node;
	return new_node;
}

void traverse(node_t head)
{
	node_t cur = head->next;
	while (cur->next)
	{
		printf("%d ->", cur->val);
		cur = cur->next;
	}
	printf("%d\n", cur->val);
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
	
	node_t cur1 = l1->next;
	node_t cur2 = l2->next;
	node_t cur3 = res;
	int flag = 0;
	int sum;
	int x, y;
	while (cur1 || cur2 || flag)
	{
		node_t nres = (node_t)malloc(sizeof(struct ListNode));
		nres->next = NULL;
		x = cur1 ? cur1->val : 0;
		y = cur2 ? cur2->val : 0;
		sum = x + y;
		if (flag)
		{
			sum += 1;
			flag = 0;
		}
		if (sum > 9)
		{
			sum -= 10;
			flag = 1;
		}
		nres->val = sum;
		if (cur1)
			cur1 = cur1->next;
		if (cur2)
			cur2 = cur2->next;
		cur3->next = nres;
		cur3 = cur3->next;
	}
	printf("List1: ");
	traverse(l1);
	printf("List2: ");
	traverse(l2);
	printf("Resul: ");
	traverse(res);
	return 0;
}


int main()
{
	node_t l1= (node_t)malloc(sizeof(struct ListNode));
	node_t l2= (node_t)malloc(sizeof(struct ListNode));
	l1->next = NULL;
	l2->next = NULL;
	l1->val = 0;
	l2->val = 0;
	res->val = 0;
	int arr1[] = { 5,2,3,7 };
	int arr2[] = { 9,7,6,3,2 };
	for (int i = 0; i < sizeof(arr1)/4; i++)
	{
		create(arr1[i], l1);
	}
	for (int i = 0; i < sizeof(arr2) / 4; i++)
	{
		create(arr2[i], l2);
	}
	addTwoNumbers(l1, l2);
	return 1;
}
