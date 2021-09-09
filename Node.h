/**************************************************************************************************
 * Implementation of the TAD Node
**************************************************************************************************/

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
//vertice
//unsigned - apenas numeros positivos
class Node{

    // Attributes
    private:
        Edge* first_edge; //Lista de arestas do vertice
        Edge* last_edge;
        int id;
        int grupo;
        unsigned int in_degree; //grau de entrada
        unsigned int out_degree; //grau de saida
        float weight; //peso do vertice
        Node* next_node; //proximo vertice
        bool inserted;

    public:
        // Constructor
        Node(int id, int grupo);
        // Destructor
        ~Node();
        // Getters
        Edge* getFirstEdge();
        Edge* getLastEdge();
        int getId();
        int getGrupo();
        int getInDegree();
        int getOutDegree();
        float getWeight();
        Node* getNextNode();
        bool getInserted();
        // Setters
        void setNextNode(Node* node);
        void setWeight(float weight);
        void setInserted(bool inserted);
        // Other methods
        bool searchEdge(int target_id);
        bool insertEdge(int target_id, float weight);
        void removeAllEdges();
        int removeEdge(int id, bool directed, Node* target_node);
        void incrementOutDegree();
        void decrementOutDegree();
        void incrementInDegree();
        void decrementInDegree();
        Edge* hasEdgeBetween(int target_id);
        // Auxiliar methods

};

#endif // NODE_H_INCLUDED
