#include <bits/stdc++.h>
#include "./libs/grafo.cpp"

using namespace std;

int main()
{
    Grafo t;
    int a, v;
    cin >> v >> a;
    t.criar_grafo(v, a);
    t.preenche_grafo();

    cout << "Fluxo Maximo: " <<  t.calcular_fluxo() << endl;

    t.mostrar_grafo();
}