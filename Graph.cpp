#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <algorithm>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}


Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

Node *Graph::getNode(int id)
{
    if(this->searchNode(id)){
        for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
            if(aux->getId() == id)
                return aux;
    }
    return nullptr;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    //Precisa inserir peso tem que passar no parametro.
    
    // Verifica se ja existe algum vertice no grafo.
    if(!this->searchNode(id)){
        Node* node = new Node(id);
        node->setNextNode(nullptr);

        if(this->first_node != nullptr){
            this->last_node->setNextNode(node);
            this->last_node = node;

        } else {        
            this->first_node = node;
            this->last_node = node;
        }
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    // Verifica se grafo está vazio, se estiver cria os vertices.
    this->insertNode(id);
    this->insertNode(target_id);

    // Procurando vertice para adicionar aresta.
    for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode()){
        if(aux->getId() == id){
            aux->insertEdge(target_id, weight);
            aux->incrementOutDegree();
        }
        if(aux->getId() == target_id){
            // Caso o grafo não seja direcionado.
            if(!this->getDirected()){
                aux->insertEdge(id, weight);
                aux->incrementOutDegree();
            } else 
                aux->incrementInDegree();
        }
    }
}

void Graph::removeNode(int id){

}

bool Graph::searchNode(int id)
{
    // Verificar se existe algum vertice no grafo.
    if(this->first_node != nullptr){
        for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
            if(aux->getId() == id)
                return true;
    }
    // Nao encontra/existe vertice no grafo.
    return false;
}

//Imprime uma lista do tipo list.
void Graph::imprimeLista (list<int>* lista){
    list<int>::iterator it;

    for (it = lista->begin(); it != lista->end(); it++) {
        cout << *it << endl;
    }
}

bool Graph::pesquisaNaLista(list<int>* lista, int id){
    list<int>::iterator find;

    find = std::find(lista->begin(), lista->end(), id);
    
    if (find != lista->end())
        return true;
    else
        return false;
}

void Graph::auxFechoDireto(list<int>* lista, int id, Node* node) { 
    
    for (Edge* vizinho = node->getFirstEdge(); vizinho != nullptr; vizinho = vizinho->getNextEdge()) {
            //Verificar se elemento existe na lista,  caso exista não inserir.

            //Adiciona vertice na lista.
            if( !this->pesquisaNaLista( lista, vizinho->getTargetId() ))
                lista->push_back( vizinho->getTargetId() );

            //Seleciona vertice vizinho.
            Node* aux = getNode( vizinho->getTargetId() );

            //Funacao recursiva.
            auxFechoDireto(lista, aux->getId(), aux);       
    }

}

void Graph::fechoDireto(int id){
    Node* node = this->getNode(id);
    list<int> *lista;
    lista = new list<int>;

    //Garante que o grafo é direcionado e que o vertice tenha pelo menos um vizinho.
    if(this->getDirected()) {
        
        if(node->getOutDegree() > 0 ){
            auxFechoDireto(lista, id, node);
            this->imprimeLista(lista);
        } else 
            cout << "Vertice nao alcança nenhum outro" << endl;
        
    } else
        cout << "Grafo nao direcionado" << endl;
    
}


//Function that prints a set of edges belongs breadth tree

bool Graph::buscaVetor(int* listIdNode, int id, int ordem){
    for(int i = 0; i < ordem; i++){
        if(listIdNode[i] == id)
            return true;
    }
    return false;
}

Graph* Graph::getVertexInduced(int* listIdNodes, int ordem){
    Graph* vInduzido = new Graph(ordem, this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

    Node *node, *original;

    if(ordem <= 0) return nullptr;

    //Para cada vertice da lista.
    for(int i = 0; i < ordem; i++){
        vInduzido->insertNode( listIdNodes[i] );
        //Vertice para o novo grafo.
        node = vInduzido->getNode( listIdNodes[i] );
        
        //Vertice do grafo original.
        original = this->getNode( listIdNodes[i] );

        for(Edge* edge = original->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            if( buscaVetor(listIdNodes, edge->getTargetId(), ordem) ){

                node->insertEdge( edge->getTargetId(), node->getWeight());
                listIdNodes[i] = -1;
            }
        }
    }
    return vInduzido;
}

void Graph::auxCaminhoProfund(int id, Graph* graph, list<int>* auxList, list<Edge>* arestas){
    Node* node = this->getNode(id);
    
    for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
    {
        //Adiciona vertice ao grafo que nao foram inseridos ainda e adiciona na lista.
        //lista armazena vertices ja visitados.
        if( !pesquisaNaLista(auxList, node->getId()) ){
            auxList->push_back( node->getId() );
            graph->insertNode( node->getId() );
        }    
        
        //Adiciona arestas ao grafo.
        if( !pesquisaNaLista(auxList, edge->getTargetId()) ){
            auxList->push_back( edge->getTargetId() );
            graph->insertEdge(node->getId(), edge->getTargetId(),edge->getWeight());            
            
            //Chamando funcao recursiva.
            auxCaminhoProfund(edge->getTargetId(), graph, auxList, arestas);
        
        //Arestas de retorno.
        } else { 
            //Para evitar que as arestas duplicadas sejam consideradas arestas de retorno.
            if( !graph->getNode( edge->getTargetId() )->searchEdge( node->getId() ) )
                arestas->push_back(*edge);
        }   
    }
}

Graph* Graph::caminhoProfund(int id){
    Graph* graph = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    list<int>* auxList = new list<int>;
    list<Edge>* arestasRet = new list<Edge>;

    auxCaminhoProfund(id, graph, auxList, arestasRet);

    list<Edge>::iterator it;

    cout << "---------- Arestas de Retorno ----------" << endl;
    for (it = arestasRet->begin(); it != arestasRet->end(); it++)
    {
        cout << it->getIdNode() << " " << it->getTargetId() <<endl;
    }

    return graph;
}

/*
void Graph::breadthFirstSearch(ofstream &output_file){

}



float Graph::floydMarshall(int idSource, int idTarget){

}



float Graph::dijkstra(int idSource, int idTarget){

}

//function that prints a topological sorting
void topologicalSorting(){

}

void breadthFirstSearch(ofstream& output_file){

}

Graph* agmKuskal(){

}
Graph* agmPrim(){

}
*/