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
#include <time.h>
#include <random>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

/**
 * Para o uso do algoritmo guloso da AGM generalizada as funções insertEdge e insertNode
 * recebem agora como parametro, tambem, o grupo que cada vertice pertence.
**/

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

int Graph::getNGrupo(){
    return this->nGrupo;
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

Node *Graph::getNode(int id) {
    if(this->searchNode(id)){
        for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
            if(aux->getId() == id)
                return aux;
    }
    return nullptr;
}

void Graph::setOrder(int order){
    this->order = order;
}

void Graph::setNGrupo(int nGrupo){
    this->nGrupo = nGrupo;
}

void Graph::incrementNumberEdges(){
    
    this->number_edges++; 
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/

void Graph::insertNode(int id, int grupo)
{   
    // Verifica se ja existe algum vertice no grafo.
    if(!this->searchNode(id)){
        Node* node = new Node(id, grupo);
        node->setNextNode(nullptr);

        if(this->first_node != nullptr){
            this->last_node->setNextNode(node);
            this->last_node = node;

        } else {        
            this->first_node = node;
            this->last_node = node;
        }
        //cout << "Inserindo --------------> " << this->getNode(id)->getId() << endl;
    }
}

void Graph::insertEdge(int id, int grupo, int target_id, float weight)
{
    // Verifica se grafo está vazio, se estiver cria os vertices.
    this->insertNode(id, grupo);
    this->insertNode(target_id, grupo);

    Node* node = this->getNode(id);
    if( node->insertEdge(target_id, weight) ){
        node->incrementOutDegree();

        //Atualiza o numero de arestas no grafo.
        this->incrementNumberEdges();
        //this->incrementNumberEdges();
        
        // Caso o grafo não seja direcionado.
        node = this->getNode(target_id);
        if( !this->getDirected() ){
            node->insertEdge(id, weight);
            node->incrementOutDegree();
        } else 
            node->incrementInDegree();
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
        //Visitar apenas vertices que ainda nao foram visitados.
        if( !this->pesquisaNaLista( lista, vizinho->getTargetId() ) && vizinho->getTargetId() != id){
            lista->push_back( vizinho->getTargetId() );

            //Seleciona vertice vizinho.
            Node* aux = getNode( vizinho->getTargetId() );

            //Funcao recursiva.
            auxFechoDireto(lista, id, aux);
        }
    }
}

list<int>* Graph::fechoDireto(int id){
    Node* node = this->getNode(id);
    list<int> *lista;
    lista = new list<int>;

    if(node == nullptr){
        cout << "Erro: Id inserido nao corresponde a nenhum vertice do grafo." << endl;
        exit(3);
    }

    //Garante que o vertice tenha pelo menos um vizinho.
    if(node->getOutDegree() > 0 ){
        auxFechoDireto(lista, id, node);
    } else 
        cout << "Vertice nao alcança nenhum outro." << endl; 
    
    return lista;
}

void Graph::auxFechoIndireto(list<int>* lista, list<int>* pilha, list<int>* visitado, Node* node, int id) { 

    if(node->getId() != id){
        //Se existir ciclo no grafo evita inserir um mesmo vertice varias vezes.
        if( !pesquisaNaLista(pilha, node->getId()) ){
            pilha->push_front( node->getId() );

            for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()) {
                
                if( !pesquisaNaLista(lista, edge->getTargetId()) ){
                    if(edge->getTargetId() == id){
                        lista->push_back( node->getId() );
                        
                    } else {   
                        Node* aux = this->getNode( edge->getTargetId() );                     
                        auxFechoIndireto(lista, pilha, visitado, aux, id);

                        if( pesquisaNaLista(lista, edge->getTargetId()) ){
                            if( !pesquisaNaLista(lista, node->getId()) )
                                lista->push_back( node->getId() );
                        }
                    }
                }
            }
            pilha->pop_front();
            if( !pesquisaNaLista(visitado, node->getId()) )
                visitado->push_back( node->getId() );
        }
    } else {
        auxFechoIndireto(lista, pilha, visitado, node->getNextNode(), id);
    } 
}

list<int>* Graph::fechoIndireto(int id){
    Node* node = this->getNode(id);
    list<int>* lista = new list<int>;
    list<int>* pilha = new list<int>;
    list<int>* visitado = new list<int>;

    if(node == nullptr){
        cout << "Erro: Id inserido nao corresponde a nenhum vertice do grafo." << endl;
        exit(3);
    }
        
    if(node->getInDegree() > 0 ){
        auxFechoIndireto(lista, pilha, visitado,this->getFirstNode(), id);

        for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        //Verifica se ficou algum vertice sem ser visitado.
        if( !pesquisaNaLista(visitado, node->getId()) )
            auxFechoIndireto(lista, pilha, visitado, node, id);
        }
        
    } else 
        cout << "Vertice nao e alcançado por nenhum outro." << endl;
 
    return lista;
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
        vInduzido->insertNode( listIdNodes[i], this->getNode(listIdNodes[i])->getGrupo() );
        //Vertice para o novo grafo.
        node = vInduzido->getNode( listIdNodes[i] );
        
        //Vertice do grafo original.
        original = this->getNode( listIdNodes[i] );

        for(Edge* edge = original->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            if( buscaVetor(listIdNodes, edge->getTargetId(), ordem) ){

                vInduzido->insertEdge(node->getId(), node->getGrupo(), edge->getTargetId(), edge->getWeight() );
            }
        }
        vInduzido->insertNode( listIdNodes[i], this->getNode(listIdNodes[i])->getGrupo() );
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
            graph->insertNode( node->getId(), node->getGrupo() );
        }    
        
        //Adiciona arestas ao grafo.
        if( !pesquisaNaLista(auxList, edge->getTargetId()) ){
            auxList->push_back( edge->getTargetId() );
            graph->insertEdge(node->getId(), node->getGrupo(), edge->getTargetId(),edge->getWeight());            
            
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

int Graph::posicaoNode(int id){
    int cont = 0;
    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        if(node->getId() == id) return cont;
        else cont++;
    }
    return 0;
}

list<int>* Graph::topologicalSorting(){
    //lista de retorno.
    list<int>* ordem = new list<int>;

    if(this->getDirected()){
        if(this->getOrder() > 0){
            //armazena vertices que nao pertencem a nunhum ciclo. (nao precisarao ser visitados novamente)
            list<int>* lista = new list<int>;
            //armazena sequencia de vertice para testar se forma um ciclo. (similar a uma pilha)
            list<int>* auxLista = new list<int>;

            //cout << "IMPRIMINDO LISTA DE VERTICES" << endl;
            //imprimeLista(lista);

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
                        gEntrada[ posicaoNode(edge->getTargetId()) ]++;
                    }
                }

                list<int>* lista = new list<int>;

                for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
                    //condicao funciona apenas se os indices dos vertices do grafo forem ordenados de 1 a n;
                    if(gEntrada[ posicaoNode(node->getId()) ] == 0)
                        lista->push_back( node->getId() );
                }
                while ( lista->begin() != lista->end() ){
                    //adiciona primeiro elemento de lista na ordem e depois o remove.
                    list<int>::iterator it = lista->begin();
                    ordem->push_back(*it);

                    Node* node = this->getNode(*it);
                    lista->pop_front();

                    for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                        gEntrada[ posicaoNode(edge->getTargetId()) ]--;

                        if( gEntrada[ posicaoNode(edge->getTargetId()) ] == 0 && !pesquisaNaLista(ordem, edge->getTargetId())) 
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
    vector<int>* vertice = new vector<int>;

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
    //Preencher um vector com todos os vertices para consultar posição.
    for(Node* aux = this->getFirstNode(); aux != nullptr; aux = aux->getNextNode()){
        vertice->push_back( aux->getId() );
    }

    //Funciona apenas se os vertices do grafo tiver ids ordenados de 1 a n.
    for(int i = 0; i < this->getOrder(); i++){
        node = this->getNode( vertice->at(i) );
        //cout << "ENTROU NO FOR PARA PREENCHER MATRIZ A0" << endl;
        for(int j = 0; j < this->getOrder(); j++){
            edge = node->hasEdgeBetween( vertice->at(j) );
            
            if(i == j){
                matriz[i][j] = 0;
                //cout << "INSERINDO 0 NA POSICAO " << i << " " << j << endl;
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
                    //cout << "INSERINDO " << edge->getWeight() << " NA POSICAO " << i << " " << j << endl;
                }
            }
        }
    }
    for(int k = 1; k < this->getOrder(); k++){
        for(int i = 0; i < this->getOrder(); i++){
            for(int j = 0; j < this->getOrder(); j++){
                //cout << "ATUALIZANDO VALORES DA MATRIZ" << endl; 
                if(matriz[i][k] + matriz[k][j] < matriz[i][j]){
                    matriz[i][j] = matriz[i][k] + matriz[k][j];
                }
            }
        }
    }
    //cout << "IMPRIMINDO MATRIZ" << endl;
    for(int i = 0; i < this->getOrder(); i++){
        for(int j = 0; j < this->getOrder(); j++)
            cout << matriz[i][j] << " \t";
        cout << endl;
    }

    return matriz[ retornaIndice(idSource, vertice) ][ retornaIndice(idTarget, vertice) ];
}

float Graph::dijkstra(int idSource, int idTarget, list<int>* vVisitado){

    //Verifica se os ids passados por parâmetro corresponde a vertices do grafo.
    if( !this->searchNode(idSource) || !this->searchNode(idTarget) ){
        cout << "Nao existe vertice no grafo com o ID inserido" << endl;
        exit;
    }

    float inf = numeric_limits<float>::max();
    
    //Armazena vertices a serem visitados.
    vector<int>* vertice = new vector<int>;
    
    //Armazena vertices já visitados.
    //list<int>* vVisitado = new list<int>;
    float caminho;

    //Vetor para armazenar peso das arestas.
    float pi[ this->getOrder() ];

    //Insere no inicial a lista de vertices ja visitados.
    vVisitado->push_back(idSource);

    //Ponteiro para vertice inicial.
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
    agm->insertEdge(vSource, this->getNode(vSource)->getGrupo(), vTarget, menorPeso);

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
    prox [ retornaIndice (vSource, vertice) ] = -1;
    prox [ retornaIndice (vTarget, vertice) ] = -1;
    
    int cont = 0;

    while(cont < this->getOrder() - 2 && vSource != -1 && vTarget != -1){
        
        //Escolhendo a proxima aresta de menor peso.
        int menor = 100000;
        vSource = -1, vTarget = -1;
        for(int i = 0; i < this->getOrder(); i ++){
            if(prox[i] != -1){
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
        if(vSource != -1 && vTarget != -1)
            agm->insertEdge(vSource, this->getNode(vSource)->getGrupo(), vTarget, menor);
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

void Graph::reloadInserted(){
    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        node->setInserted(false);
    }
}

void Graph::ordenaVetor(int* vSource, int* vTarget, float* peso, int tam){
    int auxInicial, auxFinal;
    float auxPeso;

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if(i != j){
                if (peso[i] < peso[j]) {

                    auxPeso = peso[i];
                    peso[i] = peso[j];
                    peso[j] = auxPeso;
                    
                    auxInicial = vSource[i];
                    vSource[i] = vSource[j];
                    vSource[j] = auxInicial;

                    auxFinal = vTarget[i];
                    vTarget[i] = vTarget[j];
                    vTarget[j] = auxFinal;
                }
            } 
        }
    }
}

bool Graph::verificaAresta(int* vSource, int* vTarget, int n, int e, int tam){
    int auxN = -1;
    for(int i = 0 ; i < tam; i++){
        if(vSource[i] == e) {
            if(vTarget[i] == n)
                return true;
        }
    }    
    return false;
}

Graph* Graph::agmKruskal(){
    //Instanciando grafo de retorno.
    Graph* kruskal = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());

    int* vSource = new int [ this->getNumberEdges()];
    int* vTarget = new int [ this->getNumberEdges()];
    float* peso = new float [ this->getNumberEdges()];
    
    //Preencher vetores com as arestas e pesos.
    int i = 0;
    
    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        //Grafo de retorno armazena inicialmente apenas vertices sem arestas.
        kruskal->insertNode( node->getId(), node->getGrupo() );

        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){         
            if(!kruskal->getDirected()){
                if( !verificaAresta(vSource, vTarget, node->getId(), edge->getTargetId(), i) ){
                    vSource[i] = node->getId();
                    vTarget[i] = edge->getTargetId();
                    peso[i] = edge->getWeight();
                    i++;
                } 
                
            } else {
                vSource[i] = node->getId();
                vTarget[i] = edge->getTargetId();
                peso[i] = edge->getWeight();
                i++;
            }
        }
    } 

    //Ordenar arestas por ordem crescente de pesos.
    ordenaVetor(vSource, vTarget, peso, i);
    
    int cont = 0;
    i = 0;
    int p = 0;

    //Insere as asrestas no grafo de retorno.
    while( cont < kruskal->getOrder()-1 && i < this->getNumberEdges() ){
        Node* u = getNode(vSource[i]);
        Node* v = getNode(vTarget[i]);
        p = peso[i];

        if( !u->getInserted() ||  !v->getInserted() ){
            u->setInserted(true);
            v->setInserted(true);

            kruskal->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
            u->insertEdge(v->getId(), p);
            cont++;           
        }else {
            list<int>* fechoDi = kruskal->fechoDireto(u->getId());
            if( !pesquisaNaLista(fechoDi, v->getId()) )
                if(kruskal->getDirected()){
                    list<int>* fechoIn = kruskal->fechoIndireto(u->getId());
                    if( !pesquisaNaLista(fechoIn, v->getId()) ){
                        kruskal->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                        u->insertEdge(v->getId(), p);
                        cont++;
                    }
                } else {
                    kruskal->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                    u->insertEdge(v->getId(), p);
                    cont++;
                }
        }   
        i++;
    }

    reloadInserted();
    return kruskal;
}

void Graph::preencheVetorPAGMG(int* vSource, int* vTarget, float* peso){

    int i = 0;

    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){         
            if(!this->getDirected()){
                if( !verificaAresta(vSource, vTarget, node->getId(), edge->getTargetId(), i) ){
                    vSource[i] = node->getId();
                    vTarget[i] = edge->getTargetId();
                    peso[i] = edge->getWeight();
                    i++;
                } 
                
            } else {
                vSource[i] = node->getId();
                vTarget[i] = edge->getTargetId();
                peso[i] = edge->getWeight();
                i++;
            }
        }
    }
}

float Graph::agmGuloso(Graph* PAGMG){
    //s armazana a soma dos pesos da arestas do grafo PAGMG.
    float s = 0;
    int* vSource = new int [this->getNumberEdges()];
    int* vTarget = new int [this->getNumberEdges()];
    float* peso = new float [this->getNumberEdges()];

    preencheVetorPAGMG(vSource, vTarget, peso);
    
    //Ordenar arestas por ordem crescente de pesos.
    ordenaVetor(vSource, vTarget, peso, this->getNumberEdges());

    //Vetor que armazena vertice selecionado de cada grupo.
    int* grupo = new int [ this->getNGrupo() ];
    
    //Variavel armazena quantidade de grupos.
    int num = this->getNGrupo();

    for(int i = 0; i < num; i++){
        grupo[i] = 0;
    }

    int cont = 0;
    int i = 0;
    int p = 0;

    while( num > 1 || i < this->getNumberEdges()){
        Node* u = getNode(vSource[i]);
        Node* v = getNode(vTarget[i]);
        p = peso[i];

        if(( grupo[u->getGrupo() - 1] == 0 || grupo[u->getGrupo() - 1] == u->getId() ) && ( grupo[v->getGrupo() - 1] == 0 || grupo[v->getGrupo() - 1] == v->getId() )){
            if( !u->getInserted() ||  !v->getInserted() ){
                u->setInserted(true);
                v->setInserted(true);

                PAGMG->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                s = s + p;

                if(grupo[u->getGrupo() - 1] == 0) grupo[u->getGrupo() - 1] = u->getId();
                if(grupo[v->getGrupo() - 1] == 0) grupo[v->getGrupo() - 1] = v->getId();
            } else {
               
                list<int>* fechoDi = PAGMG->fechoDireto(u->getId());
                
                if( !pesquisaNaLista(fechoDi, v->getId()) )
                    if(PAGMG->getDirected()){
                        list<int>* fechoIn = PAGMG->fechoIndireto(u->getId());
                        if( !pesquisaNaLista(fechoIn, v->getId()) ){
                            PAGMG->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                            s = s + p;

                            if(grupo[u->getGrupo() - 1] == 0) grupo[u->getGrupo() - 1] = u->getId();
                            if(grupo[v->getGrupo() - 1] == 0) grupo[v->getGrupo() - 1] = v->getId();
                        }
                    } else {
                        PAGMG->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                        s = s + p;

                        if(grupo[u->getGrupo() - 1] == 0) grupo[u->getGrupo() - 1] = u->getId();
                        if(grupo[v->getGrupo() - 1] == 0) grupo[v->getGrupo() - 1] = v->getId();
                    }
            }
            num = num -1;   
        }
        i++;
    }

    reloadInserted();   
    return s;
}

void Graph::constroiLCR(vector<Edge>* aresta, int* vSource, int* vTarget, float* peso){
    for(int i = 0; i < this->getNumberEdges(); i++){
        Edge* aux = new Edge(vTarget[i], vSource[i]);
        aux->setWeight(peso[i]);
        aresta->push_back(*aux);
    }
}

Graph* Graph::agmGulosoRand(float* s, float alfa){
    Graph* PAGMG = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    //s armazana a soma dos pesos da arestas do grafo PAGMG.
    *s = 0;
    int* vSource = new int [this->getNumberEdges()];
    int* vTarget = new int [this->getNumberEdges()];
    float* peso = new float [this->getNumberEdges()];
    vector<Edge>* aresta = new vector<Edge>;

    preencheVetorPAGMG(vSource, vTarget, peso);
    //Ordenar arestas por ordem crescente de pesos.
    ordenaVetor(vSource, vTarget, peso, this->getNumberEdges());
    //Vetor que armazena vertice selecionado de cada grupo.
    int* grupo = new int [ this->getNGrupo() ];
    //Variavel armazena quantidade de grupos.
    int num = this->getNGrupo();

    for(int i = 0; i < num; i++){
        grupo[i] = 0;
    }

    int cont = 0;
    int i = 0;
    float p = 0;
    int k;
    constroiLCR(aresta, vSource, vTarget, peso);
    vector<Edge>::pointer edge;
    while( num > 1 || i < this->getNumberEdges()){
        int aux = (alfa*(aresta->size()-1));
        
        if (aux!= 0) k = rand() % (aux);
        else k = 0;
        
        edge = &aresta->at(k);

        Node* u = this->getNode(edge->getIdNode());
        Node* v = this->getNode(edge->getTargetId());
        p = edge->getWeight();

        if(( grupo[u->getGrupo() - 1] == 0 || grupo[u->getGrupo() - 1] == u->getId() ) && ( grupo[v->getGrupo() - 1] == 0 || grupo[v->getGrupo() - 1] == v->getId() )){
            if( !u->getInserted() ||  !v->getInserted() ){
                u->setInserted(true);
                v->setInserted(true);

                PAGMG->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                *s = *s + p;

                if(grupo[u->getGrupo() - 1] == 0) grupo[u->getGrupo() - 1] = u->getId();
                if(grupo[v->getGrupo() - 1] == 0) grupo[v->getGrupo() - 1] = v->getId();
            } else {
               
                list<int>* fechoDi = PAGMG->fechoDireto(u->getId());
                
                if( !pesquisaNaLista(fechoDi, v->getId()) )
                    if(PAGMG->getDirected()){
                        list<int>* fechoIn = PAGMG->fechoIndireto(u->getId());
                        if( !pesquisaNaLista(fechoIn, v->getId()) ){
                            PAGMG->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                            *s = *s + p;

                            if(grupo[u->getGrupo() - 1] == 0) grupo[u->getGrupo() - 1] = u->getId();
                            if(grupo[v->getGrupo() - 1] == 0) grupo[v->getGrupo() - 1] = v->getId();
                        }
                    } else {
                        PAGMG->insertEdge(u->getId(), u->getGrupo(), v->getId(), p);
                        *s = *s + p;

                        if(grupo[u->getGrupo() - 1] == 0) grupo[u->getGrupo() - 1] = u->getId();
                        if(grupo[v->getGrupo() - 1] == 0) grupo[v->getGrupo() - 1] = v->getId();
                    }
            }
            num = num -1;   
        }
        aresta->erase(aresta->begin() + k);
        i++;
    }

    reloadInserted();   
    delete [] vSource;
    delete [] vTarget;
    delete [] peso;
    delete aresta;
    return PAGMG;
}

void Graph::greed(ofstream& output_file){
    float tempo;
    time_t tInicial, tFinal;
    
    Graph* guloso = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    tInicial = time(NULL);
    float s = agmGuloso(guloso);
    tFinal = time(NULL);
    tempo = difftime(tFinal, tInicial);
    cout << "Solucao: " << s << endl;
    cout << "Tempo de Execucao do Algoritmo Guloso: " << tempo << endl;
    escreveArquivo(guloso, output_file);
}

float Graph::greedRandom(float alfa, int interacoes, ofstream& output_file){
    float tempo;
    time_t tInicial, tFinal;
    
    Graph* guloso;
    int i = 0, k;
    float solbest = 10000000, s = 0;

    tInicial = time(NULL);
    while(i < interacoes){
        cout << "Interacao: " << i << endl;
        guloso = agmGulosoRand(&s, alfa); 
        cout << "Valor de s: " << s << endl;
        i++;
        if(s < solbest){
            solbest = s;
        }
    }
    tFinal = time(NULL);
    tempo = difftime(tFinal, tInicial);
    cout << "Melhor Solucao: " << solbest << endl;
    cout << "Tempo de Execucao do Algoritmo Guloso: " << tempo << endl;
    escreveArquivo(guloso, output_file);
    return solbest;
}

void Graph::inicializaMedia(float* media, float alfa[], int m){
    float s = 0;
    for(int i = 0; i < m; i++){
        Graph* guloso = new Graph(this->getOrder(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
        guloso = agmGulosoRand(&s, alfa[i]);
        media[i] = s/alfa[i];
    } 
}

void Graph::inicializaProb(float* prob, int m){
    for(int i = 0; i < m; i++){
        prob[i] = (float) 1/m;
    }
} 

float Graph::escolheAlfa(float* prob, float* alfa, int m, int* p){
    mt19937 generator{rd()};
    vector<float> aux(prob, prob+m);
    discrete_distribution<int> dist{aux.begin(), aux.end()};
    *p = dist(generator);
    return alfa[*p];
}

void Graph::atualizarMedia(float* media, int* quant, float* s, int m){
    for(int i = 0; i < m; i++){
        if(quant[i] != 0){
            media[i] = (float)(s[i])/(quant[i]);
        }
    }
}

void Graph::atualizaProbabilidades(float* prob, float* media, int m, float solbest){
    float* q = new float [m];
    float aux;
    for(int i = 0; i < m; i++){
        q[i] = pow((solbest/media[i]), 2);
        aux = aux + q[i];
    }
    for(int i = 0; i < m; i++){
        prob[i] = (q[i]/aux);
    }
}

float Graph::greedRactiveRandom(float* alfa, int m, int interacoes, int bloco, ofstream& output_file){
    float tempo;
    time_t tInicial, tFinal;
    
    Graph* guloso;
    int i = 0, k;
    int posicao = 0; 
    float solbest = 100000, s = 0, a = 0;
    float* vet = new float [m];
    int* quantAlfa = new int [m];

    for(int i = 0; i < m; i++){
        vet[i] = 0;
        quantAlfa[i] = 0;
    }
    
    float* media = new float [m];
    float* prob = new float [m];
    inicializaProb(prob, m);
    inicializaMedia(media, alfa, m);

    tInicial = time(NULL);
    while(i < interacoes){
        cout << "Interacao: " << i << endl;
        if(i % bloco==0) {
            atualizaProbabilidades(prob, media, m, solbest);
            cout << "Probabilidade dos alfas: ";
            for(int i = 0; i < m; i++){
                cout << prob[i] << " ";
            }
            cout << endl;
        }
        a = escolheAlfa(prob, alfa, m, &posicao);
        cout << "Valor de alfa: " << a << endl;
        guloso = agmGulosoRand(&s, a); 
        i++;
        cout << "Solucao: " << s << endl;
        vet[posicao] += s;
        quantAlfa[posicao]++;
        atualizarMedia(media, quantAlfa, vet, m);
        if(s < solbest){
            solbest = s;
        }
    }
    tFinal = time(NULL);
    tempo = difftime(tFinal, tInicial);
    cout << "Melhor Solucao: " << solbest << endl;
    cout << "Tempo de Execucao do Algoritmo Guloso: " << tempo << endl;
    escreveArquivo(guloso, output_file);
    return solbest;
}

void Graph::imprimeAGMGeneralizada(){
    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        cout << "--------------------AGM GENERALIZADA--------------------" << endl;
        cout << node->getId() << " " << node->getGrupo() << endl;
        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            cout << node->getId() << " " << edge->getTargetId() << edge->getWeight() << endl;
        }
    }
}

void Graph::escreveArquivo(Graph* graph, ofstream& output_file){
    int opcao;
    cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
    cin >> opcao;

    if(opcao){
        if ( !graph->getDirected() ) 
            output_file << "strict graph {" << endl;
        else 
            output_file << "digraph graphname {" << endl;

        for(Node* node = graph->getFirstNode(); node != nullptr; node = node->getNextNode()){
            for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                if(graph->getDirected())
                        output_file <<  node->getId() << " -> " << edge->getTargetId();
                    else
                        output_file <<  node->getId() << " -- " << edge->getTargetId();
                output_file << " [label = " << edge->getWeight() << "]" << endl;
            }
        }
        output_file << "}" << endl;
    }
}