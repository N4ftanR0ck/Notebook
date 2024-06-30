#define SPLAY_H
#pragma once
#include <malloc.h>
#include <stdio.h>
#include "item.h"

int dateKey(item* itm);
item* rightRotate(item* x);
item* leftRotate(item* x);

item* reTreeDate(item* newroot, item* root);
item* reTreePr(item* newroot, item* root);
item* delete_key_date(item* root, item* itm);
item* insert_pr(item* root, int pr, item* itm, FILE* stream);
item* insert_date(item* root, int date, item* itm, FILE* stream);
item* cpyItem(item* itm1, item* itm2);
item* splay_pr(item* root, int key);
item* splay_date(item* root, int key);
item* leftRotate(item* x);
item* rightRotate(item* x);
int dateKey(item* itm);

int dateKey(item* itm)
{
    if (itm == NULL)
        return 0;
    return itm->year * 365 + itm->mounth * 30 + itm->day;
}

item* rightRotate(item* x)
{
    item* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

item* leftRotate(item* x)
{
    item* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}


 item* splay_date( item* root, int key)
{
    if (root == NULL || dateKey(root) == key)
        return root;

    if (dateKey(root) > key)
    {
        if (root->left == NULL) return root;

        // Zig-Zig 
        if (dateKey(root->left) > key)
        {
            root->left->left = splay_date(root->left->left, key);

            root = rightRotate(root);
        }
        else if (dateKey(root->left) < key) // Zig-Zag
        {
            root->left->right = splay_date(root->left->right, key);

            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL) ? root : rightRotate(root);
    }
    else
    {
        if (root->right == NULL) return root;

        // Zig-Zag 
        if (dateKey(root->right) > key)
        {
            root->right->left = splay_date(root->right->left, key);

            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (dateKey(root->right) < key)// Zag-Zag 
        {
            root->right->right = splay_date(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

 item* splay_pr(item* root, int key)
 {
     if (root == NULL || root->priority == key)
         return root;

     if (root->priority < key)
     {
         if (root->left == NULL) return root;

         // Zig-Zig 
         if (root->left->priority < key)
         {
             root->left->left = splay_pr(root->left->left, key);

             root = rightRotate(root);
         }
         else if (root->left->priority < key) // Zig-Zag
         {
             root->left->right = splay_pr(root->left->right, key);

             if (root->left->right != NULL)
                 root->left = leftRotate(root->left);
         }

         return (root->left == NULL) ? root : rightRotate(root);
     }
     else
     {
         if (root->right == NULL) return root;

         // Zig-Zag 
         if (root->right->priority < key)
         {
             root->right->left = splay_pr(root->right->left, key);

             if (root->right->left != NULL)
                 root->right = rightRotate(root->right);
         }
         else if (root->right->priority > key)// Zag-Zag 
         {
             root->right->right = splay_pr(root->right->right, key);
             root = leftRotate(root);
         }

         return (root->right == NULL) ? root : leftRotate(root);
     }
 }

 item* cpyItem(item* itm1, item* itm2)
 {
     itm1->day = itm2->day;
     itm1->year = itm2->year;
     itm1->min = itm2->min;
     itm1->mounth = itm2->mounth;
     itm1->hour = itm2->hour;
     itm1->priority = itm2->priority;
     itm1->locate = itm2->locate;
     itm1->event = itm2->event;
     return itm1;
 }

 item* insert_date(item* root, int date, item* itm, FILE* stream)
{
     item* newitm = (item*)malloc(sizeof(item));
     newitm = cpyItem(newitm, itm);
     if (root == NULL)
     {
         newitm->left = NULL;
         newitm->right = NULL;
         return newitm;
     }

    root = splay_date(root, date);
    if (dateKey(root) == date && !strcmp(root->event, newitm->event))
    {
        printf("this event already recorded!\n");
        return root;
    }

    if (dateKey(root) > date)
    {
        newitm->right = root;
        newitm->left = root->left;
        root->left = NULL;
    }

    
    else
    {
        newitm->left = root;
        newitm->right = root->right;
        root->right = NULL;
    }
    return newitm; 
}

 

 item* insert_pr(item* root, int pr, item* itm, FILE* stream)
 {
     item* newitm = (item*)malloc(sizeof(item));
     int date = dateKey(itm);
     newitm = cpyItem(newitm, itm);
     //free(itm); itm = NULL;
     if (root == NULL)
     {
         newitm->left = NULL;
         newitm->right = NULL;
         return newitm;
     }

     root = splay_pr(root, pr);
     if (dateKey(root) == date && !strcmp(root->event, newitm->event))
     {
         printf("this event already recorded!\n");
         return root;
     }

     if (root->priority < pr)
     {
         newitm->right = root;
         newitm->left = root->left;
         root->left = NULL;
     }


     else
     {
         newitm->left = root;
         newitm->right = root->right;
         root->right = NULL;
     }
     return newitm;
 }

 item* delete_key_date( item* root, item* itm)
 {
     item* temp;
     if (!root)
         return NULL;

     int key = dateKey(itm);

     root = splay_date(root, key);
     if (key != dateKey(root))
         return root;
     if (!root->left) {
         temp = root;
         root = root->right;
     }
     else {
         temp = root;
         root = splay_date(root->left, key);
         root->right = temp->right;
     }

     free(temp);
     return root;
 }

 item* reTreePr(item* newroot, item* root)
 {
     if (root != NULL)
     {
         newroot = insert_pr(newroot, root->priority, root, stdin);
         item* temp1 = root->left;
         item* temp2 = root->right;
         free(root); root = NULL;
         newroot = reTreePr(newroot, temp1);
         newroot = reTreePr(newroot, temp2);
     }
     return newroot;
 }

 item* reTreeDate(item* newroot, item* root)
 {
     if (root != NULL)
     {
         newroot = insert_date(newroot, dateKey(root), root, stdin); 
         item* temp1 = root->left;
         item* temp2 = root->right;
         free(root); root = NULL;
         newroot = reTreeDate(newroot, temp1);
         newroot = reTreeDate(newroot, temp2);
     }
     return newroot;
 }