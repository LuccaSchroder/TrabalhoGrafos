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

//Funcao auxiliar fecho transitivo direto.
void verticeTransitivoDireto(Graph* graph, ofstream& output_file) {
    int vertice;

    cout << "Digite o ID do vertice: " << endl;
    cin >> vertice; 

    graph->fechoDireto(vertice);
    
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

    cout << "Ordem: " << vInduzido->getOrder() << endl;
    cout << "Arestas: " << endl;
    for (Node* node = vInduzido->getFirstNode(); node != nullptr; node = node->getNextNode()){
        for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            cout << node->getId() << " " << edge->getTargetId() << endl; 
        }
    }

    delete [] listIdNodes;
}

//Caminhamento em profundidade.
void caminhoProfund(Graph* graph, ofstream& output_file){
    Graph* arvore;
    int vertice;

    cout << "Digite o ID do vertice" << endl;
    cin >> vertice;

    arvore = graph->caminhoProfund(vertice);

    output_file << "strict graph {" << endl;

    for(Node* node = arvore->getFirstNode(); node != nullptr; node = node->getNextNode()){
       for (Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
       {
            cout << node->getId() << "--" << edge->getTargetId() << ";" << endl;
            output_file <<  node->getId() << "--" << edge->getTargetId() << endl;
       }
    }
    output_file << "}" << endl;

}

void ordenacaoTopologica(Graph* graph, ofstream& output_file){
    graph->topologicalSorting();
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
    cout << "[9] Caminhamento em Profundidade" << endl;
    cout << "[10] Algoritmo Guloso" << endl;
    cout << "[11] Algoritmo Guloso Randomizado " << endl;
    cout << "[12] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}


void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        //Subgrafo induzido por um conjunto de vértices X;
        case 1:{
            subgrafoInduzido(graph, output_file);
            break;
        }
            //Caminho mínimo entre dois vértices usando Dijkstra;
        case 2:{

            break;
        }

            //Caminho mínimo entre dois vértices usando Floyd;
        case 3:{

            break;
        }

            //AGM - Kruscal;
        case 4:{



            break;
        }

            //AGM Prim;
        case 5:{

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
        case 9:{
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

