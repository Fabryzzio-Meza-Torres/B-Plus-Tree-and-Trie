#include <iostream>
#include "Trie.h"
#include "TreeBPlus.h"
using namespace std;    

int main(){
    Trie* trie = new Trie();
    trie->insert("hola");
    trie->insert("holas");
    trie->insert("holita");
    trie->insert("holitas");
    trie->insert("holas");
    trie->print(trie->getRoot());
    cout << trie->buscar("hola") << endl;
    cout << trie->buscar("holas") << endl;
    cout << trie->buscar("holita") << endl;

    trie->eliminar("holas");
    trie->print(trie->getRoot());
    cout << trie->buscar("holas") << endl;


    BPlusTree tree(3);

    tree.insert(1);
    tree.insert(3);
    tree.insert(7);
    tree.insert(10);
    tree.insert(11);
    tree.insert(13);
    tree.insert(14);
    tree.insert(15);
    tree.insert(18);
    tree.insert(16);
    tree.insert(19);
    tree.insert(24);
    tree.insert(25);
    tree.insert(26);
    tree.insert(21);
    tree.insert(4);
    tree.insert(5);
    tree.insert(20);
    tree.insert(22);
    tree.insert(2);
    tree.insert(17);
    tree.insert(12);
    tree.insert(6);

    cout << "Recorrido: ";
    tree.traverse();
    cout << endl;

    int key = 0;
    BPlusTreeNode* result = tree.search(key);
    if (result)
        cout << "Encontrado " << key << " en el arbol b+" << endl;
    else
        cout << "No encontrado " << key<< " en el arbol b+" << endl;

    int key1 = 10;
    tree.remove(key1);
    cout << "Recorrido despues de la eliminacion de " << key1 << ": ";
    tree.traverse();

    cout << endl;
    //En el caso de que lo que desees eliminar no este en el árbol.
    tree.remove(50);
    cout << endl;
    return 0;
}
