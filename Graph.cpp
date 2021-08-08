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

void Graph::incrementNumberEdges(){
    
    this->number_edges++; 
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{   
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
        cout << "Inserindo --------------> " << this->getNode(id)->getId() << endl;
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    // Verifica se grafo está vazio, se estiver cria os vertices.
    this->insertNode(id);
    this->insertNode(target_id);

    Node* node = this->getNode(id);
    if( node->insertEdge(target_id, weight) ){
        node->incrementOutDegree();
        
        // Caso o grafo não seja direcionado.
        node = this->getNode(target_id);
        if( !this->getDirected() ){
            node->insertEdge(id, weight);
            node->incrementOutDegree();
        } else 
            node->incrementInDegree();
        
        //Atualiza o numero de arestas no grafo.
        this->incrementNumberEdges();
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
        cout << *it << " ";
    }
    cout << endl;
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

    if(node == nullptr){
        cout << "Erro: Id inserido nao corresponde a nenhum vertice do grafo." << endl;
        exit(3);
    }

    //Garante que o grafo é direcionado e que o vertice tenha pelo menos um vizinho.
    if(this->getDirected()) {
        
        if(node->getOutDegree() > 0 ){
            auxFechoDireto(lista, id, node);
            this->imprimeLista(lista);
        } else 
            cout << "Vertice nao e alcançado por nenhum outro." << endl;
        
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

                node->insertEdge( edge->getTargetId(), edge->getWeight() );
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
        cout << it->getIdNode() << " --- " << it->getTargetId() <<endl;
    }
    cout << "----------------------------------------" << endl;

    return graph;
}

bool Graph::cicloGrafo(int id, list<int>* lista, list<int>* pilha){
    Node* node = this->getNode(id);
    bool temCiclo;

    pilha->push_front(node->getId());

    if(node->getOutDegree() > 0){
        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            
            //Procura na pilha para ver se vertice ja foi visitado.
            if( pesquisaNaLista(pilha, edge->getTargetId()) )
                return true;
            
            //se vertice estiver na lista todos seus vizinhos ja foram visitados e ele nao faz parte de nenhum ciclo.
            if( !pesquisaNaLista(lista, edge->getTargetId()) ){
                temCiclo = cicloGrafo(edge->getTargetId(), lista, pilha);
                
                if(temCiclo) 
                    return true;
            }
        }
    }
    //vertices que nao fazem parte de nunhum ciclo.
    else{
        lista->push_back(node->getId()); //adiciona a lista para nao procurar nos vizinhos dele novamente.
        pilha->pop_front(); //remove da lista.
        return false;
    }
    return false; //foi adicionado porque aparece warning no terminal.
} 

//function that prints a topological sorting
list<int>* Graph::topologicalSorting(){
    //lista de retorno.
    list<int>* ordem = new list<int>;

    if(this->getDirected()){
        if(this->getOrder() > 0){
            //armazena vertices que nao pertencem a nunhum ciclo. (nao precisarao ser visitados novamente)
            list<int>* lista = new list<int>;
            //armazena sequencia de vertice para testar se forma um ciclo. (similar a uma pilha)
            list<int>* auxLista = new list<int>;

            if( !cicloGrafo(this->getFirstNode()->getId(), lista, auxLista) ){
                //Grafo nao tem nenhum ciclo (e uma DAG).
                //Inicializa vetor que armazena grau de entrada dos vertices com 0.
                int gEntrada[this->getOrder()];
                for(int i = 0; i < this->getOrder(); i++){
                    gEntrada[i] = 0;
                }
                //adiciona ao vetor grau de entrada de cada vertice.
                for( Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode() ){
                    for( Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge() ){
                        gEntrada[ edge->getTargetId() - 1]++;
                    }
                }

                list<int>* lista = new list<int>;

                for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
                    //condicao funciona apenas se os indices dos vertices do grafo forem ordenados de 1 a n;
                    if(gEntrada[ node->getId() - 1 ] == 0)
                        lista->push_back( node->getId() );
                }
                while ( lista->begin() != lista->end() ){
                    //adiciona primeiro elemento de lista na ordem e depois o remove.
                    list<int>::iterator it = lista->begin();
                    ordem->push_back(*it);

                    Node* node = this->getNode(*it);
                    lista->pop_front();

                    for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                        gEntrada[ edge->getTargetId() - 1 ]--;

                        if( gEntrada[ edge->getTargetId() - 1] == 0 && !pesquisaNaLista(ordem, edge->getTargetId())) 
                            lista->push_back( edge->getTargetId() );
                    }
                } 

            }else cout << "Grafo possui ciclo(s)" << endl;
            
        } else cout << "Grafo vazio" << endl;

    } else cout << "Grafo nao e direcionado." << endl;

    return ordem; 
}


float Graph::floydMarshall(int idSource, int idTarget){
    float inf = numeric_limits<float>::max();
    float matriz[this->getOrder()][this->getOrder()];
    Node *node;
    Edge *edge;
    //Verifica se os dois vertices existe no grafo.
    if( this->getNode(idSource) == nullptr || this->getNode(idTarget) == nullptr ){
        cout << "Erro: Nao existe vertice com Id inserido" << endl;
        exit(1);
    }
    //Se o grafo nao tem peso nas arestas, entao a custo de caminho minimo de um vertice para outro e 0;
    if( !this->getWeightedEdge() ){
        return 0;
    }
    //Funciona apenas se os vertices do grafo tiver ids ordenados de 1 a n.
    for(int i = 0; i < this->getOrder(); i++){
        node = this->getNode(i + 1);
        cout << "ENTROU NO FOR PARA PREENCHER MATRIZ A0" << endl;
        for(int j = 0; j < this->getOrder(); j++){
            edge = node->hasEdgeBetween(j + 1);
            
            if(i == j){
                matriz[i][j] = 0;
                cout << "INSERINDO 0 NA POSICAO " << i << " " << j << endl;
            }
                
            else { 
                //cout << "ARESTA ---> " << edge->getTargetId() << " PESO: " << edge->getWeight() << endl; 
                //Para vertices que sao vizinhos.
                if(edge == nullptr){
                    matriz[i][j] = 10000;
                    //cout << "INSERINDO 10000 NA POSICAO " << i << " " << j << endl;
                }
                //Para evitar que grafos que nao tem peso nas arestas tenham valor infinito.
                else if( edge->getWeight() == 0) 
                    matriz[i][j] = 0;
                //Para vertices que nao sao vizinhos.
                else{ 
                    matriz[i][j] = edge->getWeight();
                    cout << "INSERINDO " << edge->getWeight() << " NA POSICAO " << i << " " << j << endl;
                }
            }
        }
    }
    for(int k = 1; k < this->getOrder(); k++){
        for(int i = 0; i < this->getOrder(); i++){
            for(int j = 0; j < this->getOrder(); j++){
                cout << "ATUALIZANDO VALORES DA MATRIZ" << endl; 
                if(matriz[i][k] + matriz[k][j] < matriz[i][j]){
                    matriz[i][j] = matriz[i][k] + matriz[k][j];
                }
            }
        }
    }
    cout << "IMPRIMINDO MATRIZ" << endl;
    for(int i = 0; i < this->getOrder(); i++){
        for(int j = 0; j < this->getOrder(); j++)
            cout << matriz[i][j] << " \t";
        cout << endl;
    }

    return matriz[idSource - 1][idTarget - 1];
}

float Graph::dijkstra(int idSource, int idTarget){
    if( !this->searchNode(idSource) || !this->searchNode(idTarget) ){
        cout << "Nao existe vertice no grafo com o ID inserido" << endl;
        exit;
    }
    float inf = numeric_limits<float>::max();
    //Armazena vertices a serem visitados.
    vector<int>* vertice = new vector<int>;
    //Armazena vertices já visitados.
    list<int>* vVisitado = new list<int>;
    float caminho;

    //Vetor para armazenar peso das arestas.
    float pi[ this->getOrder() ];

    //Insere no inicial a lista de vertices ja visitados.
    vVisitado->push_back(idSource);

    //Ponteiro aponto para vertice inicial.
    Node* node = this->getNode(idSource);

    for(Node* aux = this->getFirstNode(); aux != nullptr; aux = aux->getNextNode()){
        //Preenche a lista de vertices a serem visitados.
        //if(aux->getId() != idSource){
            vertice->push_back( aux->getId() );
        //}
    }
    //Pegar todos os vertices do grafo e verificar se são vizinhos do vertice idSouce.
    Node* aux = this->getFirstNode();

    for(int i = 0; i < this->getOrder(); i++){
        //Para evitar que valor do vertice inicial seja alterado.
        if(aux->getId() != idSource){ 
            Edge* edge = node->hasEdgeBetween( aux->getId() );
            //cout << "VIZINHOS DE J" << edge->getTargetId() << endl;
            
            if( edge != nullptr)
                pi[i] = edge->getWeight();
            else 
                pi[i] = 100000;    
        } else {
            pi[i] = 0;
        }   
        aux = aux->getNextNode();
    }    

    //Construindo o caminho ate o id target ser visitado.
    while ( !pesquisaNaLista(vVisitado, idTarget) )
    {
        int j = -1, menor = 1000000;
        aux = this->getFirstNode();
        //Procurar menor elemento dentro do vetor.
        while (aux != nullptr) {
            
            //Atualizando valores de pi.
            if( !pesquisaNaLista(vVisitado, aux->getId()) ){ 
                if (pi[ retornaIndice(aux->getId(), vertice) ] < menor) {
                    menor = pi[ retornaIndice(aux->getId(), vertice) ];
                    //j recebe id do menor vertice.
                    j = aux->getId(); 
                }
            }
            aux = aux->getNextNode();
        }

        //Removendo o vertice da lista inicial de vertices.
        //vertice->erase( remove(vertice->begin(), vertice->end(), int(j)) );
        vVisitado->push_back(j);
        caminho = pi[ retornaIndice(j, vertice) ];

        //Percorrendo pelos vizinhos do vertice j para atualiza-los.
        for(Edge* edge = this->getNode(j)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            int i = edge->getTargetId();
            //Pegando vizinhos de j que já foram visitados.
            float aux = pi[ retornaIndice(j, vertice) ] + edge->getWeight();
            if(aux < pi[ retornaIndice(i, vertice) ]){
                //vertice->push_back( i );
                pi[ retornaIndice(i, vertice) ] = aux;
            }
        }
    }

    //delete vertice;
    //delete vVisitado;
    return caminho;
}

int Graph::retornaIndice (int j, vector<int>* vertice){
    vector<int>::iterator it;
    int cont = 0;
    for (it = vertice->begin(); it != vertice->end(); it++) {
        if(*it != j) 
            cont++;
        else return cont;
    }
    return 0;
}

/*
void breadthFirstSearch(ofstream& output_file){

}

Graph* agmKuskal(){

}*/

Graph* Graph::agmPrim(){

    Graph* agm = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    
    if (this->getNumberEdges() < 1){
        cout << "Grafo nao possui nenhuma arestas." << endl;
        agm = nullptr;
        return agm;
    }
    
    int* prox = new int[ this->getOrder() ]; 

    //Vetor auxiliar para armazenar o indice dos vertice.
    vector<int>* vertice = new vector<int>;

    //Guarda aresta de menor peso para comparar.
    int vSource, vTarget;
    float menorPeso = 100000;

    //Procura aresta com menor peso.
    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        vertice->push_back( node->getId() );

        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            if(edge->getWeight() < menorPeso){
                menorPeso = edge->getWeight();
                vSource = node->getId();
                vTarget = edge->getTargetId();
            }   
        }
    }

    //Insere 1 aresta no grafo de retorno agm.
    agm->insertEdge(vSource, vTarget, menorPeso);

    //Preenchendo vetor prox.    
    for(int i = 0; i < this->getOrder(); i++){
        Node* node = this->getNode( vertice->at(i) );
        //Pegando aresta (i, vSource) e (i, vTarget).
        Edge* edge1 = node->hasEdgeBetween( vSource );
        Edge* edge2 = node->hasEdgeBetween( vTarget ); 
        
        if(edge1 != nullptr && edge2 != nullptr){
            if( edge1->getWeight() < edge2->getWeight() )
                prox[i] = vSource;
            else
                prox[i] = vTarget;

        } else if ( edge1 != nullptr && edge2 == nullptr )
            prox[i] = vSource;
        
        else if ( edge1 == nullptr && edge2 != nullptr )
            prox[i] = vTarget;
        
        else
            prox[i] = 100000;
    }

    //Inserir 0 para as arestas vSource e vTarget.
    prox [ retornaIndice (vSource, vertice) ] = 0;
    prox [ retornaIndice (vTarget, vertice) ] = 0;
    
    int cont = 0;

    while(cont < this->getOrder() - 2){
        
        //Escolhendo a proxima aresta de menor peso.
        int menor = 100000;
        vSource = -1, vTarget = -1;
        for(int i = 0; i < this->getOrder(); i ++){
            if(prox[i] != 0){
                //Acessando a aresta.
                Node* node = this->getNode( vertice->at(i) );
                Edge* edge = node->hasEdgeBetween( prox[i] );
                //selecionando aresta de menor custo.
                if(edge != nullptr){
                    if(edge->getWeight() < menor){
                        menor = edge->getWeight();
                        vSource = vertice->at(i);
                        vTarget = prox[i];
                    }
                }
            }
        }
        //Insere aresta no grafo.
        agm->insertEdge(vSource, vTarget, menor);
        prox [ retornaIndice (vSource, vertice) ] = 0;

        //Atualiza o vetor prox.
        for(int i = 0; i < this->getOrder(); i ++){
            if(prox[i] != 0){

                Node* node = this->getNode( vertice->at(i) );
                
                Edge* edge1 = node->hasEdgeBetween( prox[i] );
                Edge* edge2 = node->hasEdgeBetween( vSource );
                
                if(edge1 != nullptr && edge2 != nullptr){
                    if( edge1->getWeight() > edge2->getWeight() )
                        prox[i] = vSource;

                } else if ( edge1 == nullptr && edge2 != nullptr )
                    prox[i] = vSource;
            }
        }
        cont++;
    }
    return agm;
}   
