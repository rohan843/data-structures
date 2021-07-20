#include <bits\stdc++.h>

using namespace std;

#define T 3
//M == 2*T

class TreeNode
{
public:
    int keys[2 * T - 1];
    TreeNode *chld[2 * T];
    int n;     //number of keys
    bool leaf; //true if node is a leaf node (n != 0 for a leaf node as well)
    TreeNode()
    {
        leaf = true;
        n = 0;
        for (int i = 0; i < 2 * T; i++)
        {
            chld[i] = NULL;
        }
    }
};

//Creates an empty root node for the B - Tree
TreeNode *initTree()
{
    TreeNode *root = new TreeNode();
    return root;
}

//Takes a non full parent node and splits its ith child node (fully filled) appropriately about the median key
void splitChild(TreeNode *root, int i)
{
    //create a new treenode to accomodate the half of ith child,
    //The newly made node is a leaf if the ith child is also a leaf
    TreeNode *nw = new TreeNode();
    nw->leaf = root->chld[i]->leaf;

    //Making space for the child's median key in the parent node
    for (int j = root->n - 1; j >= i; j--)
    {
        root->keys[j + 1] = root->keys[j];
        root->chld[j + 2] = root->chld[j + 1];
    }

    //Placing the median value in the parent from the ith child
    root->keys[i] = root->chld[i]->keys[T - 1];
    root->n++;

    //Adding the new child to the parent
    root->chld[i + 1] = nw;

    //copying data from the ith child in the new child
    int j = T;
    while (j < root->chld[i]->n)
    {
        nw->keys[j - T] = root->chld[i]->keys[j];
        nw->chld[j - T] = root->chld[i]->chld[j];
        j++;
    }
    nw->chld[j - T] = root->chld[i]->chld[j];

    //updating the size of the modified ith child and (i+1)th child
    root->chld[i]->n = T - 1;
    root->chld[i + 1]->n = T - 1;
}

void insertKeyNonFullParent(TreeNode *root, int data)
{
    //if the current node is a leaf node, we insert the data
    //as it is assured that this node will be non full.
    if (root->leaf)
    {
        int i = 0;
        while (i < root->n && root->keys[i] < data)
        {
            i++;
        }
        int j = root->n - 1;
        while (j >= i)
        {
            root->keys[j + 1] = root->keys[j];
            j--;
        }
        root->keys[i] = data;
        root->n++;
    }
    else
    {
        int i = 0;
        while (i < root->n && root->keys[i] < data)
        {
            i++;
        }
        if (root->chld[i]->n == 2 * T - 1)
        {
            splitChild(root, i);
            if (root->keys[i] < data)
            {
                i++;
            }
        }
        insertKeyNonFullParent(root->chld[i], data);
    }
}

//Inserts the data in the B - tree and returns the
//updated root. Works in a single pass down the tree.
//Any full node encountered in the way is also split in half.
TreeNode *insertKey(int data, TreeNode *root)
{
    if (root->n == 2 * T - 1)
    {
        //if the current root is a fully filled node, we create
        //a new node as the root, and assign the current root as its 0th
        //child. Then we split the previous root node.
        TreeNode *nw = new TreeNode();
        nw->leaf = false;
        nw->chld[0] = root;
        root = nw;
        splitChild(root, 0);
    }
    //insert the key in the tree rooted at 'root' (non full)
    insertKeyNonFullParent(root, data);
    return root;
}

//deletes a given key from the B - tree and returns the modified root
//if the key doesn't exist, simply returns the current root
//Works in a manner to ensure that any recursive call on a node x of the B - tree
//ensures beforehand that the node x has atleast T keys, because to be a B-tree,
//atleast T - 1 keys are needed in a node, and we may be deleting a key from x.
TreeNode *deleteKey(int data, TreeNode *root)
{
    //linearly searching for the key in the keys of the root
    int ki = 0;
    while (ki < root->n)
    {
        if (root->keys[ki] == data)
        {
            break;
        }
    }

    //if the key is not found in the current node
    //these lines will operate
    if (ki == root->n)
    {
        //if the current node was a leaf, then the key is not present in the tree
        if (root->leaf)
            return root;
    }

    //if the key was found and the current node was a leaf, simply delete the key,
    //maintaining the sorted order of the keys array
    if (root->leaf)
    {
        for (int i = ki; i < root->n - 1; i++)
        {
            root->keys[i] = root->keys[i + 1];
        }
    }

    //if the key is found, but the current node is an internal node
    else
    {
        //if the child y that precedes the key in the current node has atleast T keys,
        //we replace the current key (at index ki) with its immediate predecessor
        //in the child y (i.e., its keys array's last element). Then we call the delete
        //function on node y for the predecessor key.
        if (root->chld[ki]->n >= T)
        {
            root->keys[ki] = root->chld[ki]->keys[root->chld[ki]->n - 1];
            deleteKey(root->chld[ki]->keys[root->chld[ki]->n - 1], root->chld[ki]);
        }

        //If the preceding node has lesser keys, but the succeding node has
        //atleast T - 1 keys, we follow the same procedure and bring the smallest key of the
        //succeding node. Then recursively delete it from the succeding node.
        else if (root->chld[ki + 1]->n >= T)
        {
            root->keys[ki] = root->chld[ki + 1]->keys[0];
            deleteKey(root->chld[ki + 1]->keys[0], root->chld[ki + 1]);
        }

        //if both the preceding and succeding nodes have less than T, i.e., T - 1
        //keys, we append the current key (i.e., "data") in the preceding node
        //and the succeding node in the preceding node as well.
        //Now the preceding node has 2 * T - 1 keys.
        //We appropriately change the current root's keys and chld arrays and 
        //free the memory for the succeding node.
        //Then we call the delete function recursively for the same key in the preceding node (now the only child remaining among the preceding and succeding node) 
        else
    }
}

void print(TreeNode *root, string incrementalOffset = "  ", string initialOffset = "")
{
    if (root == NULL)
    {
        return;
    }
    cout << initialOffset;
    for (int i = 0; i < root->n; i++)
    {
        cout << root->keys[i] << ' ';
    }
    cout << endl;
    if (!root->leaf)
    {
        for (int i = 0; i <= root->n; i++)
        {
            if (i != 0)
                cout << initialOffset + ' ' << root->keys[i - 1] << ':' << endl;
            print(root->chld[i], incrementalOffset, initialOffset + incrementalOffset);
        }
    }
}

int main()
{
    TreeNode *root = initTree();
    int n;
    cin >> n;
    n++;
    for (int i = 1; i < n; i++)
    {
        root = insertKey(/*rand() % */ i, root);
    }
    //cout << root->leaf << endl;
    print(root);
    return 0;
}
