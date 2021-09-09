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
#include <random>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int nGrupo;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

    public:
        random_device rd;
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters e Setters
        int getOrder();
        int getNGrupo();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        void setOrder(int order);
        void setNGrupo(int order);
        //Other methods
        void insertNode(int id, int grupo);
        void insertEdge(int id, int grupo, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        void incrementNumberEdges();

        //methods phase1
        list<int>* fechoDireto(int id);
        list<int>* fechoIndireto(int id);
        Graph* caminhoProfund(int id);
        list<int>* topologicalSorting();
        //void breadthFirstSearch(ofstream& output_file);
        Graph* getVertexInduced(int* listIdNodes, int ordem);
        Graph* agmKruskal();
        Graph* agmPrim();
        float floydMarshall(int idSource, int idTarget);
        float dijkstra(int idSource, int idTarget, list<int>* vVisitado);

        //methods phase1
        void greed(ofstream& output_file);
        float greedRandom(float alfa, int interacoes, ofstream& output_file);
        float greedRactiveRandom(float* alfa, int m, int interacoes, int bloco, ofstream& output_file);
    private:
        //Auxiliar methods
        void auxFechoDireto(list<int>* lista, int id, Node* node);
        void auxFechoIndireto(list<int>* lista, list<int>* pilha, list<int>* visitado, Node* node, int id);
        void imprimeLista (list<int>* lista);
        bool pesquisaNaLista(list<int>* lista, int id);
        void auxCaminhoProfund(int id, Graph* graph, list<int>* auxList, list<Edge>* arestas);
        bool buscaVetor(int* listIdNodes, int id, int ordem);
        int retornaIndice(int j, vector<int>* vertice);   
        bool cicloGrafo(int id, list<int>* lista, list<int>* pilha);
        void reloadInserted();
        void ordenaVetor(int* vSource, int* vTarget, float* peso, int i);
        int posicaoNode(int id);
        bool verificaAresta(int* vSource, int* vTarget, int id, int id_target, int i);
        void imprimeAGMGeneralizada();
        void escreveArquivo(Graph* graph, ofstream& output_file);
        void preencheVetorPAGMG(int* vSource, int* vTarget, float* peso);
        void constroiLCR(vector<Edge>* aresta, int* vSource, int* vTarget, float* peso);
        void inicializaProb(float* prob, int m);
        void inicializaMedia(float* media, float* alfa, int m);
        void atualizaProbabilidades(float* prob, float* media, int m, float solbest);
        float escolheAlfa(float prob[], float alfa[], int m, int* posicao);
        void atualizarMedia(float* media, int* quant, float* vet, int m);
        float agmGuloso(Graph* PAGMG);
        Graph* agmGulosoRand(float* s, float alfa);
};

#endif // GRAPH_H_INCLUDED
