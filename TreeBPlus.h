using namespace std;

const int MAX_KEYS = 3;

// A B+Tree node
class BPlusTreeNode {
public:
    int* keys; // An array of keys
    int t; // Minimum degree (defines the range for number of keys)
    BPlusTreeNode** C; // An array of child pointers
    int n; // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    BPlusTreeNode(int _t, bool _leaf); // Constructor

    void removeFromNonLeaf(int t);
    void insertNonFull(int k);
    void removeFromLeaf(int t);
    void borrowFromPrev(int t);
    void splitChild(int i, BPlusTreeNode* y);
    void traverse();
    void merge(int t);
    int getSuccesor(int t);
    int findKey(int k);
    void borrowFromNext(int t);
    void fill(int t);
    int getPredeccesor(int t);
    BPlusTreeNode* search(int k);
    void remove(int k);
};

// A B+Tree
class BPlusTree {
    BPlusTreeNode* root;
    int t;

public:
    BPlusTree(int _t) { root = nullptr; t = _t; }
    void traverse() { if (root != nullptr) root->traverse(); }
    BPlusTreeNode* search(int k) { return (root == nullptr) ? nullptr : root->search(k); }

    void insert(int k);
    void remove(int k);
};

BPlusTreeNode::BPlusTreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;
    keys = new int[2*t-1];
    C = new BPlusTreeNode*[2*t];
    n = 0;
}

void BPlusTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (leaf == false)
            C[i]->traverse();
        cout << " " << keys[i];
    }
    if (leaf == false)
        C[i]->traverse();
}

BPlusTreeNode* BPlusTreeNode::search(int k) {
    int i = 0;
    while (i < n && k > keys[i])
        i++;
    if (keys[i] == k)
        return this;
    if (leaf)
        return nullptr;
    return C[i]->search(k);
}

void BPlusTree::insert(int k) {
    if (root == nullptr) {
        root = new BPlusTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BPlusTreeNode *s = new BPlusTreeNode(t, false);
            s->C[0] = root;
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);
            root = s;
        } else
            root->insertNonFull(k);
    }
}

void BPlusTreeNode::insertNonFull(int k) {
    int i = n-1;
    if (leaf == true) {
        while (i >= 0 && keys[i] > k) {
            keys[i+1] = keys[i];
            i--;
        }
        keys[i+1] = k;
        n = n+1;
    } else {
        while (i >= 0 && keys[i] > k)
            i--;
        if (C[i+1]->n == 2*t-1) {
            splitChild(i+1, C[i+1]);
            if (keys[i+1] < k)
                i++;
        }
        C[i+1]->insertNonFull(k);
    }
}

void BPlusTreeNode::splitChild(int i, BPlusTreeNode* y) {
    BPlusTreeNode* z = new BPlusTreeNode(y->t, y->leaf);
    z->n = t - 1;
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];
    if (y->leaf == false) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
    y->n = t - 1;
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
    C[i+1] = z;
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];
    keys[i] = y->keys[t-1];
    n = n + 1;
}

void BPlusTree::remove(int k) {
    if (!root) {
        cout << "El árbol está vacío\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BPlusTreeNode *tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->C[0];
        delete tmp;
    }
}

void BPlusTreeNode::remove(int k) {
    int t = findKey(k);

    if (t < n && keys[t] == k) {
        if (leaf)
            removeFromLeaf(t);
        else
            removeFromNonLeaf(t);
    } else {
        if (leaf) {
            cout << k << " no esta presente en el árbol.\n";
            return;
        }

        bool flag = (t == n) ? true : false;

        if (C[t]->n < t) {
            fill(t);
        }
        if (flag && t > n){
            C[t - 1]->remove(k);}
        else{
            C[t]->remove(k);}
    }
}

void BPlusTreeNode::removeFromLeaf(int t) {
    for (int i = t + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
}

void BPlusTreeNode::removeFromNonLeaf(int t) {
    int k = keys[t];

    if (C[t]->n >= t) {
        int pred = getPredeccesor(t);
        keys[t] = pred;
        C[t]->remove(pred);
    } else if (C[t + 1]->n >= t) {
        int succ = getSuccesor(t);
        keys[t] = succ;
        C[t + 1]->remove(succ);
    } else {
        merge(t);
        C[t]->remove(k);
    }
}

int BPlusTreeNode::getPredeccesor(int t) {
    BPlusTreeNode* cur = C[t];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

int BPlusTreeNode::getSuccesor(int t) {
    BPlusTreeNode* cur = C[t + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

void BPlusTreeNode::fill(int t) {
    if (t != 0 && C[t - 1]->n >= t)
        borrowFromPrev(t);
    else if (t != n && C[t + 1]->n >= t)
        borrowFromNext(t);
    else {
        if (t != n)
            merge(t);
        else
            merge(t - 1);
    }
}

void BPlusTreeNode::borrowFromPrev(int t) {
    BPlusTreeNode* child = C[t];
    BPlusTreeNode* brother = C[t - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->keys[0] = keys[t - 1];

    if (!child->leaf)
        child->C[0] = brother->C[brother->n];

    keys[t - 1] = brother->keys[brother->n - 1];

    child->n += 1;
    brother->n -= 1;
}

void BPlusTreeNode::borrowFromNext(int t) {
    BPlusTreeNode* child = C[t];
    BPlusTreeNode* brother = C[t + 1];

    child->keys[child->n] = keys[t];

    if (!child->leaf)
        child->C[child->n + 1] = brother->C[0];

    keys[t] = brother->keys[0];

    for (int i = 1; i < brother->n; ++i)
        brother->keys[i - 1] = brother->keys[i];

    if (!brother->leaf) {
        for (int i = 1; i <= brother->n; ++i)
            brother->C[i - 1] = brother->C[i];
    }

    child->n += 1;
    brother->n -= 1;
}

void BPlusTreeNode::merge(int t) {
    BPlusTreeNode* child = C[t];
    BPlusTreeNode* brother = C[t + 1];

    child->keys[t - 1] = keys[t];

    for (int i = 0; i < brother->n; ++i)
        child->keys[i + t] = brother->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= brother->n; ++i)
            child->C[i + t] = brother->C[i];
    }

    for (int i = t + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    for (int i = t + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += brother->n + 1;
    n--;

    delete brother;
}

int BPlusTreeNode::findKey(int k) {
    int t = 0;
    while (t < n && keys[t] < k)
        ++t;
    return t;
}

