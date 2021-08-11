#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;
//teste
Graph* leitura(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if(!graph->getWeightedEdge() && !graph->getWeightedNode()){
        int c = 1;
        while(input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);

        }        

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() ){

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

        }

    }else if(graph->getWeightedNode() && !graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }else if(graph->getWeightedNode() && graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }

    return graph;
}

Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    input_file >> order >> numEdges;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while(input_file >> idNodeSource >> idNodeTarget) {

        graph->insertEdge(idNodeSource, idNodeTarget, 0);

    }

    return graph;
}

//Função para escrever no arquivo.
void escreveArquivo(Graph* graph, ofstream& output_file){
    
    if ( !graph->getDirected() ) 
        output_file << "strict graph {" << endl;
    else 
        output_file << "digraph graphname {" << endl;

    for(Node* node = graph->getFirstNode(); node != nullptr; node = node->getNextNode()){
       for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
       {
           if(graph->getDirected()){
                cout << node->getId() << "->" << edge->getTargetId() << ";" << endl;
                output_file <<  node->getId() << "->" << edge->getTargetId() << endl;
            } else {
                cout << node->getId() << "--" << edge->getTargetId() << ";" << endl;
                output_file <<  node->getId() << "--" << edge->getTargetId() << endl;
            }
       }
    }
    output_file << "}" << endl;
}

//Função para imprimir grafo.
void imprimeGrafo(Graph* graph){

    cout << "Ordem: " << graph->getOrder() << endl;
    cout << "Arestas " << endl;
    for (Node* node = graph->getFirstNode(); node != nullptr; node = node->getNextNode()){
        for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            cout << node->getId() << " " << edge->getTargetId() << endl; 
        }
    }
}

//Funcao auxiliar fecho transitivo direto.
void verticeTransitivoDireto(Graph* graph, ofstream& output_file) {
    int vertice;
    list<int>* fecho;
    list<int>::iterator it;

    cout << "Digite o ID do vertice: " << endl;
    cin >> vertice; 

    fecho = graph->fechoDireto(vertice);

    int opcao;
    cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
    cin >> opcao;
    
    if(opcao){
        output_file << "Fecho Transitivo(" << vertice << ") = { ";
        for(it = fecho->begin(); it != fecho->end(); it++){
            output_file << *it << " ";
        }
        output_file << "}";
    }
    
}

//Funcao auxiliar fecho transitivo indireto.
void verticeTransitivoIndireto(Graph* graph, ofstream& output_file) {
    int vertice;
    list<int>* fecho;
    list<int>::iterator it;

    cout << "Digite o ID do vertice: " << endl;
    cin >> vertice; 

    fecho = graph->fechoIndireto(vertice);

    int opcao;
    cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
    cin >> opcao;
    
    if(opcao){
        output_file << "Fecho Intransitivo(" << vertice << ") = { ";
        for(it = fecho->begin(); it != fecho->end(); it++){
            output_file << *it << " ";
        }
        output_file << "}";
    }
    
}

//Funcao auxiliar grafo vertice induzido.
void subgrafoInduzido(Graph* graph, ofstream& output_file){
    int quantVertices;
    Graph* vInduzido;
    
    cout << "Quantos vertice?" << endl;
    cin >> quantVertices;
    
    int* listIdNodes = new int [quantVertices];

    cout << "Digite o ID dos vertices" << endl;
    for(int i = 0; i < quantVertices; i++){
        cin >> listIdNodes[i];
    }

    vInduzido = graph->getVertexInduced(listIdNodes, quantVertices);
    imprimeGrafo(vInduzido);
    
    delete [] listIdNodes;
}

//Caminhamento em profundidade.
void caminhoProfund(Graph* graph, ofstream& output_file){
    Graph* arvore;
    int vertice;

    cout << "Digite o ID do vertice" << endl;
    cin >> vertice;

    arvore = graph->caminhoProfund(vertice);

    escreveArquivo(arvore, output_file);
}

void ordenacaoTopologica(Graph* graph, ofstream& output_file){
    list<int>* ordem;
    list<int>::iterator it;
    
    ordem = graph->topologicalSorting();
    
    for(it = ordem->begin(); it != ordem->end(); it++){
        cout << *it << " ";
    }
    cout << endl;

    int opcao;
    cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
    cin >> opcao;
    
    if(opcao){
        output_file << "Ordenação Topológica" << " = { ";
        for(it = ordem->begin(); it != ordem->end(); it++){
            output_file << *it << " ";
        }
        output_file << "}";
    }
    cout << endl;
}

void caminhoMinFloyd(Graph* graph, ofstream& output_file){
    int a, b;

    cout << "Digite o ID dos vertices: " << endl;
    cin >> a >> b;

    float cam = graph->floydMarshall(a, b);
    //cout << "O caminho mínimo entre os dois vertices é " << cam << endl << endl;

    if(cam == 10000){
        cout << "Não existe caminho entre os dois vertices" << endl;
    } else {
        cout << "O valor caminho mínimo entre os dois vertices é " << cam << endl << endl;
        int opcao;
        cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
        cin >> opcao;

        if(opcao){
            output_file << "Caminho minimo(" << a << ", " << b << ") = " << cam;
        }
    }
}

void caminhoMinDijkstra(Graph* graph, ofstream& output_file){
    int a, b;
    list<int>* vVisitado = new list<int>;
    int opcao;
    list<int>::iterator it;

    cout << "Digite o ID dos vertices: " << endl;
    cin >> a >> b;

    float cam = graph->dijkstra(a, b, vVisitado);
    if(cam == 100000){
        cout << "Não existe caminho entre os dois vertices" << endl;
    } else {
        cout << "O valor caminho mínimo entre os dois vertices é " << cam << endl << endl;
        int opcao;
        cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
        cin >> opcao;

        if(opcao){
            output_file << "Caminho minimo(" << a << ", " << b << ") = { ";
            for(it = vVisitado->begin(); it != vVisitado->end(); it++){
                output_file << *it << " ";
            }
            output_file << "}";
        }
    }        
     
}

//Função auxiliar algoritmo de Prim.
void auxAgmPrim(Graph* graph, ofstream& output_file){
    Graph* prim;
    int quantVertices;
    
    cout << "Quantos vertice?" << endl;
    cin >> quantVertices;
    
    int* listIdNodes = new int [quantVertices];

    cout << "Digite o ID dos vertices" << endl;
    for(int i = 0; i < quantVertices; i++){
        cin >> listIdNodes[i];
    }

    prim = graph->getVertexInduced(listIdNodes, quantVertices);
    prim = prim->agmPrim();
    //prim = graph->agmPrim();
    if(prim != nullptr)
        imprimeGrafo(prim);
    int opcao;
    cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
    cin >> opcao;

    if(opcao) escreveArquivo(prim, output_file);
}

void auxAgmKruscal(Graph* graph, ofstream& output_file){
    Graph* kruscal;

    int quantVertices;
    
    cout << "Quantos vertice?" << endl;
    cin >> quantVertices;
    
    int* listIdNodes = new int [quantVertices];

    cout << "Digite o ID dos vertices" << endl;
    for(int i = 0; i < quantVertices; i++){
        cin >> listIdNodes[i];
    }

    kruscal = graph->getVertexInduced(listIdNodes, quantVertices);
    kruscal = kruscal->agmPrim();
    //prim = graph->agmPrim();
    if(kruscal != nullptr)
        imprimeGrafo(kruscal);
    int opcao;
    cout << "Salvar no arquivo?(sim(1)/nao(0))" << endl;
    cin >> opcao;

    if(opcao) escreveArquivo(kruscal, output_file);
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Subgrafo induzido por conjunto de vértices" << endl;
    cout << "[2] Caminho Mínimo entre dois vértices - Dijkstra" << endl;
    cout << "[3] Caminho Mínimo entre dois vértices - Floyd" << endl;
    cout << "[4] Árvore Geradora Mínima de Prim" << endl;
    cout << "[5] Árvore Geradora Mínima de Kruskal" << endl;
    cout << "[6] Imprimir caminhamento em largura" << endl;
    cout << "[7] Imprimir ordenacao topológica" << endl;
    cout << "[8] Imprimir Fecho Transitivo Direto de um vertice" << endl;
    cout << "[9] Imprimir Fecho Transitivo Indireto de um vertice" << endl;
    cout << "[10] Caminhamento em Profundidade" << endl;
    cout << "[11] Algoritmo Guloso" << endl;
    cout << "[12] Algoritmo Guloso Randomizado " << endl;
    cout << "[13] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}


void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {
        case 0:{
            exit;
            break;
        }
            //Subgrafo induzido por um conjunto de vértices X;
        case 1:{
            subgrafoInduzido(graph, output_file);
            break;
        }
            //Caminho mínimo entre dois vértices usando Dijkstra;
        case 2:{
            caminhoMinDijkstra(graph, output_file);
            break;
        }
            //Caminho mínimo entre dois vértices usando Floyd;
        case 3:{
            caminhoMinFloyd(graph, output_file);
            break;
        }
            //AGM Prim;
        case 4:{
            auxAgmPrim(graph, output_file);
            break;
        }
            //AGM - Kruscal;
        case 5:{
            auxAgmKruscal(graph, output_file);
            break;
        }

            //Busca em largura;
        case 6:{

            break;
        }
            //Ordenação Topologica;
        case 7:{
            ordenacaoTopologica(graph, output_file);
            break;
        }
            //Fecho transitivo direto do vertice;
        case 8:{
            verticeTransitivoDireto(graph, output_file);
            break;
        }
            //Fecho transitivo Indireto do vertice;
        case 9:{
            verticeTransitivoIndireto(graph, output_file);
            break;
        }
            //Caminhamento em profundidade;
        case 10:{
            caminhoProfund(graph, output_file);
            break;
        }
        default:
        {
            cout << " Error!!! invalid option!!" << endl;
        }

    }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        system("clear");
        selecao = menu();

        if(output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;

    }

    return 0;
}



int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);



    Graph* graph;

    if(input_file.is_open()){
        graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];


    mainMenu(output_file, graph);



    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}

