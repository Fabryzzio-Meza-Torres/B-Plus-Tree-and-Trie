#include <iostream>
#include "Trie.h"
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
    return 0;
}