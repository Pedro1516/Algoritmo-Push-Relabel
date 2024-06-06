#include <bits/stdc++.h>

using namespace std;

struct Vertice
{
    double altura;
    double excesso;

    Vertice(double altura, double excesso)
    {
        this->altura = altura;
        this->excesso = excesso;
    }
};

struct Aresta
{
    int vertices[2];
    double fluxo;
    double capacidade;

    Aresta(int vertices[2], double fluxo, double capacidade)
    {
        this->fluxo = fluxo;
        this->capacidade = capacidade;
        this->vertices[0] = vertices[0];
        this->vertices[1] = vertices[1];
    }
};

class Grafo
{
private:
    int qtd_vertices, qtd_arestas, fluxo_maximo, excesso_total = 0;
    vector<Vertice> vertice;
    vector<Aresta> aresta;

    void add_vertice(int altura, double excesso)
    {
        this->vertice.push_back(Vertice(altura, excesso));
    }

    void add_aresta(int vertices[2], double capacidade)
    {
        this->aresta.push_back(Aresta(vertices, 0, capacidade));
    }

    void push(Aresta *aresta, Vertice *vertice)
    {
        int aux = aresta->fluxo;
        if (vertice->excesso > aresta->capacidade - aresta->fluxo)
        {
            aresta->fluxo = aresta->capacidade;
            vertice->excesso -= aresta->fluxo - aux;

            this->vertice[aresta->vertices[1]].excesso += aresta->fluxo - aux; // atualiza o excesso do vertice seguinte
        }
        else
        {
            aresta->fluxo += vertice->excesso;
            this->vertice[aresta->vertices[1]].excesso += vertice->excesso;

            vertice->excesso = 0;
        }
        // add vertice inversa
        if (aresta->fluxo == aresta->capacidade)
        {
            int vert[2];
            vert[0] = aresta->vertices[1];
            vert[1] = aresta->vertices[0];
            add_aresta(vert, aresta->fluxo);
        }
        // atualiza o excesso total quando o fluxo chega a T
        if (aresta->vertices[1] == this->vertice.size() - 1 || aresta->vertices[1] == 0)
        {

            excesso_total -= aresta->fluxo - aux;
        }
    }

    void relabel(int pos_vertice)
    {
        int menor_altura = 32000;
        for (auto e : aresta)
        {
            if (e.vertices[0] == pos_vertice)
            {
                if (menor_altura > vertice[e.vertices[1]].altura && e.fluxo < e.capacidade)
                    menor_altura = vertice[e.vertices[1]].altura;
            }
        }

        vertice[pos_vertice].altura = menor_altura + 1;
    }

    void pre_fluxo()
    {
        int aux_vertice[2];
        for (int i = 0; i < aresta.size(); i++)
        {
            if (aresta[i].vertices[0] == 0)
            { // filtra todas as arestas partindo do vertice S
                aresta[i].fluxo = aresta[i].capacidade;

                // add aresta invertida
                aux_vertice[0] = aresta[i].vertices[1];
                aux_vertice[1] = aresta[i].vertices[0];
                add_aresta(aux_vertice, aresta[i].capacidade);

                this->vertice[aresta[i].vertices[1]].excesso = aresta[i].capacidade; // atualiza excesso dos vertices adjacentes a S
                excesso_total += aresta[i].capacidade;
            }
        }
    }

public:
    void criar_grafo(int vertice, int aresta)
    {
        this->qtd_vertices = vertice;
        this->qtd_arestas = aresta;

        for (size_t i = 0; i < vertice; i++)
        {
            if (i == 0)
                add_vertice(vertice, 0);
            else
                add_vertice(0, 0);
        }
    }

    void preenche_grafo()
    {
        int aux = this->qtd_arestas, vet[2], capacidade;
        while (aux--)
        {
            cin >> vet[0] >> vet[1] >> capacidade;
            add_aresta(vet, capacidade);
        }
    }

    void mostrar_grafo()
    {
        for (size_t i = 0; i < vertice.size(); i++)
        {
            cout << "Vertice: " << i << " excesso: " << vertice[i].excesso << " altura: " << vertice[i].altura;
            for (size_t j = 0; j < aresta.size(); j++)
            {
                if (aresta[j].vertices[0] == i)
                    cout << " [(" << i << "," << aresta[j].vertices[1] << ") fluxo: " << aresta[j].fluxo << " capacidade: " << aresta[j].capacidade << "] ";
            }
            cout << endl;
        }

        cout << "vertices: " << qtd_vertices << endl
             << "arestas:  " << qtd_arestas << endl;
    }

    int calcular_fluxo()
    {
        pre_fluxo();

        bool aux = false;
        while (excesso_total > 0)
        {
            for (size_t i = 1; i < vertice.size() - 1; i++)
            {
                if (vertice[i].excesso > 0)
                {
                    for (size_t j = 0; j < aresta.size(); j++)
                    {
                        if (aresta[j].vertices[0] == i)
                        {
                            if (vertice[aresta[j].vertices[0]].altura > vertice[aresta[j].vertices[1]].altura && aresta[j].fluxo < aresta[j].capacidade)
                            {
                                push(&aresta[j], &vertice[i]);
                                aux = true;
                                if (vertice[i].excesso == 0)
                                    break;
                            }
                        }
                    }

                    if (!aux)
                    {
                        relabel(i);
                    }
                    aux = false;
                }
            }
        }
        fluxo_maximo = vertice[vertice.size() - 1].excesso;
        return vertice[vertice.size() - 1].excesso;
    }
};
