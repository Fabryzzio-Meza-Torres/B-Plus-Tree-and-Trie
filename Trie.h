#include <iostream>
#include <vector>
using namespace std;
struct Node
{
    string value;
    vector<Node *> children;
    Node(string value) : value(value), children(26, nullptr) {}
};
struct Trie
{
private:
    Node *root;
    int charL(char c){
        return c-'a';
    }

public:
    Trie() : root(new Node("")) {}
    void insert(string word)
    {
        Node *current = root;
        for (char c : word)
        {
            if (current->children[charL(c)] == nullptr)
            {
                current->children[charL(c)] = new Node(current->value + c);
            }
            current = current->children[charL(c)];
        }
    }
    bool buscar(string word){
        Node* current = root;
        for(char c: word){
            if(!current->children[charL(c)]){
                return false;
            }
            current = current->children[charL(c)];
        }
        return current != nullptr && current->value == word;
    }
    void eliminar(string word){
        Node* current = root;
        for(char c: word){
            if(!current->children[charL(c)]){
                return;
            }
            current = current->children[charL(c)];
        }
        if(current->value == word){
            current->value = "";
        }

    }
    Node *getRoot()
    {
        return root;
    }
    void print(Node *root)
    {
        if (root == nullptr)
        {
            return;
        }
        cout << root->value << endl;
        for (int i = 0; i < 26; i++)
        {
            print(root->children[i]);
        }
    }
};
