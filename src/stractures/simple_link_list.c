#include <stdlib.h>
#include <stdio.h>
#include "simple_link_list.h"


void InsertNode(Type value,LinkList * list)
{
    ListNode * newNode = (ListNode*)malloc(sizeof(ListNode));
    if(newNode==NULL)
    {
        return;
    }
    newNode->value=value;
    newNode->next = list->head;
    list->head = newNode;
    list->count++;
}


void InitializeLinkList(LinkList * list)
{
    list->head = NULL;
    list->count = 0;
}

void DeleteNode(LinkList * list,int pos,Free_Node free_node)
{
    if(list->head==NULL || pos<0||pos>=list->count)
    {
        return;
    }
    ListNode * temp = list->head;
    if(pos==0)
    {
        list->head = temp->next;
        free(temp);
        list->count--;
        return;
    }
    ListNode * prev = NULL;
    for(int i=0;i<pos;i++)
    {
        prev = temp;
        temp = temp->next;
    }
    prev->next = temp->next;
    if(free_node!=NULL){
        free_node(temp->value);

    }
    free(temp);
    list->count--;
}