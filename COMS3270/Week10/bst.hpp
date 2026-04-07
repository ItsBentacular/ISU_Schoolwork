#ifndef BST_H
#define BST_H

template <class T>
class bst
{
private:
    class node
    {
    public:
        T data;
        node *left, *right;
        node(T d) : data(d), left(0), right(0) {}
        virtual ~node()
        {
            if (left)
            {
                delete left;
            }
            if (right)
            {
                delete right;
            }
        }
    };

    node *root;

public:
    bst() : root(0) {}
    virtual ~bst() {
        if(root) {
            delete root;
        }
    }
    void insert(T d)
    {
        node *tmp, *cur, *parent;

        bool left;

        tmp = new node(d);

        if (!root)
        {
            root = tmp;
            return;
        }
        for (cur = root; cur;)
        {
            parent = cur;
            if (d > root->data)
            {
                cur = cur->right;
                left = false;
            }
            else
            {
                cur = cur->left;
                left = true;
            }
        }
        if(left)
        {
            parent->left = tmp;
        } else 
        {
            parent->right = tmp;
        }
    }

    bool contains(T d)
    {
        node *cur;

        if(!root) {
            return false;
        }
        for(cur = root; cur && d != cur->data;) {
            if(d < cur->data) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return cur ? true : false;
    }
};

#endif