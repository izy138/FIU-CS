#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node *right;
    struct node *left;
};

struct node* createNode(int data) {
    struct node *temp;
    temp = (struct node*)malloc(sizeof(struct node));
    if(temp == NULL) {
        printf("Error creating node\n");
        exit(1);
    }
    temp->data = data;
    temp->right = NULL;
    temp->left = NULL;
    return temp;
}

void treeInsert(struct node** Tree, int value) {
    struct node* newNode = createNode(value); // creates a node with the new value being inserted to tree
    struct node* pNode = NULL; // pointer to the parent node is null
    struct node* cNode = *Tree; // the current node is set to the tree's root pointer

    while (cNode != NULL) { // if the current node (the tree root) is not null.
        pNode = cNode; // set the parent node to the current node.
        if (newNode->data < cNode->data) // if the newNode.data is less than the currentNode.data
            cNode = cNode->left; // then the current node is set to current.left
        else
            cNode = cNode->right; // else the current node is set to the current.right
    }
    if (pNode == NULL) // if the parent node is null
        *Tree= newNode; // the tree is empty so the new node is now the tree root
    else if (newNode->data < pNode->data) // else if the newNode.data is less than the root.data
        pNode->left = newNode; //sets the new node to the parent.left
    else
        pNode->right = newNode; // else the new node is greater and sets it to the parent.right
}

void inorder(struct node *root) {
    if(root != NULL) { // if the tree is not empty
        inorder(root->left);  // prints the left side of the tree
        printf("%d ", root->data);
        inorder(root->right); // prints the right side of the tree.
    }
}
//free memory allocated for nodes
void freeMemory(struct node* rootNode) {
    if (rootNode != NULL) {
        freeMemory(rootNode->left);
        freeMemory(rootNode->right);
        free(rootNode);
    }
}

int main() {
    struct node *root = NULL;
    int n, value;

    printf("How many values do you want to insert? ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter value #%d: ", i+1);
        scanf("%d", &value);
        treeInsert(&root, value);
    }

    printf("Inorder traversal:\n");
    inorder(root);
    printf("\n");

    freeMemory(root);

    return 0;

}