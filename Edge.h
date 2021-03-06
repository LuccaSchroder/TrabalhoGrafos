/**************************************************************************************************
 * Implementation of the TAD Edge
**************************************************************************************************/

#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

// Definition of the Edge class
//Arestas
class Edge{

    // Attributes
    private:
        int target_id;
        Edge* next_edge; //pr�xima aresta.
        float weight;
        int id_node;

    public:
        // Constructor
        Edge(int target_id, int id_node);
        // Destructor
        ~Edge();
        // Getters
        int getTargetId(); //id da aresta
        int getIdNode(); //id da aresta
        Edge* getNextEdge(); //ponteiro para pr�xima aresta
        float getWeight(); //peso da aresta
        // Setters
        void setNextEdge(Edge* edge);
        void setWeight(float weight);

};

#endif // EDGE_H_INCLUDED
