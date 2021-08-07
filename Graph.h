/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <vector>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void insertNode(int id);
        void insertEdge(int id, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);

        //methods phase1
        void fechoDireto(int id);
        Graph* caminhoProfund(int id);
        list<int>* topologicalSorting();
        //void breadthFirstSearch(ofstream& output_file);
        Graph* getVertexInduced(int* listIdNodes, int ordem);
        Graph* agmKuskal();
        Graph* agmPrim();
        float floydMarshall(int idSource, int idTarget);
        float dijkstra(int idSource, int idTarget);

        //methods phase1
        float greed();
        float greedRandom();
        float greedRactiveRandom();
    private:
        //Auxiliar methods
        void auxFechoDireto(list<int>* lista, int id, Node* node);
        void imprimeLista (list<int>* lista);
        bool pesquisaNaLista(list<int>* lista, int id);
        void auxCaminhoProfund(int id, Graph* graph, list<int>* auxList, list<Edge>* arestas);
        bool buscaVetor(int* listIdNodes, int id, int ordem);
        int retornaIndice(int j, vector<int>* vertice);   
        bool cicloGrafo(int id, list<int>* lista, list<int>* pilha);
};

#endif // GRAPH_H_INCLUDED
