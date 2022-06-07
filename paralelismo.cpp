#include<iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <list>
#include<cstring>
#include <random>
#include<omp.h>
#include <unordered_set>
using namespace std;

//g++ -Wall -O3 paralelismo.cpp -o paralelismo
struct item {
    int valor;
    string traceback; 
    int coluna;
    int linha;
};

struct max_score {
    int score;
    string seq1; 
    string seq2;
};

typedef vector<item>matriz;
matriz inicializa_matriz(matriz mat, int n, int m) {
    //uma linha eh igual a n elementos 
    //quantidade de elementos = n*m
    for (int elementos = 1; elementos <= (n+1)*(m+1) ; elementos++) {
        mat.push_back({0, "zero"});        
    }
    return mat;
}


string calcula_max(int diag, int delet, int inser){
    if((diag >= delet) and (diag >= inser)){
        return "diagonal";
    }else if((delet >= diag) and (delet >= inser)){
        return "delete";
    }else{
        return "insert";
    }
}

int maximo(int d,int del, int i){
    if(d> del and d> i){
        return d;
    }else if(del> d and del> i){
        return del;
    }else{
        return i;
    }
}

matriz calcula_score(matriz mat, int n, int m, string seq1, string seq2, int match, int mismatch, int gap){
    //linha seq 1
    //coluna seq 2
    for(int i = 1; i<=n; i++){
        for(int j = 1; j<=m; j++){
        int diagonal;
        int delecao;
        int insercao;
        
        

        int indice_diagonal = (i-1)*(m+1)+(j-1);
        int indice_esquerda = i*(m+1)+(j-1);
        int indice_cima = (i-1)*(m+1)+j;
        
        if (seq1[j-1] == seq2[i-1]){ 
            diagonal = mat[indice_diagonal].valor + 2;
        }else{
            diagonal = mat[indice_diagonal].valor -1;
        }

        delecao = mat[indice_esquerda].valor - 1 ;

        insercao = mat[indice_cima].valor - 1;
        int indice = i*(m+1)+j;
        mat[indice].traceback = calcula_max(diagonal,delecao,insercao);
        mat[indice].valor = maximo(diagonal,delecao,insercao);
        mat[indice].linha = j;
        mat[indice].coluna = i;
            

        }        
        
    }
    

    return mat;
}



int voltando(matriz mat, int n, int m, string seq1, string seq2){
    item maximo;
    maximo.valor = 0;
    
    string voltar;
    string seq1_alinhada;
    string seq2_alinhada;
    for (int a = 0; a <= n*m ; a++) {
           if(mat[a].valor>maximo.valor){
            maximo= mat[a];
            }
    }

    
    return maximo.valor;
    
}



int main(){
    // double tdata = omp_get_wtime();
    int a; //tamanho sequencia 1 linha
    int b; //tamanho sequencia 2 coluna
    cin >> a >> b;
    string seqA;
    string seqB;
    cin >> seqA;
    cin >> seqB;

    int wmat;
    int wmis;
    int wgap;
    cin >> wmat;
    cin >> wmis;
    cin >> wgap;

    // cout << "sequencia a: " << seqA << " tamanho: " << a << endl;
    // cout << "sequencia b: " << seqB << " tamanho: " << b << endl;
    // cout << "" << endl;

    // cout << "match: " << wmat << endl;
    // cout << "mismatch: " << wmis << endl;
    // cout << "gap: " << wgap << endl;
    // cout << "" << endl;


    //----------------------------------Gerar todas as subsequencias a´ e b´ não-nulas de a e b, respectivamente.-------------------------------------------------
    vector<string>subsequencias_a;
    for(int e =0; e<a ; e++){
        string a_linha;
        for(int i =e; i<a ; i++){
            a_linha += seqA[i];
            subsequencias_a.push_back(a_linha);
        }
    }

    vector<string>subsequencias_b;
    for(int e =0; e<b ; e++){
        string b_linha;
        for(int i =e; i<b ; i++){
            b_linha += seqB[i];
            subsequencias_b.push_back(b_linha);
        }
    }

    //testar
    //  for(int i =0; i< subsequencias_a.size(); i++){
    //     cout << subsequencias_a[i] << endl;
    // }

    //------------------------Calcular os alinhamentos de cada par de subsequencias (a´, b´) com os pesos wmat, wmis e wgap-------------------------------------------

    max_score score_maximo;
    score_maximo.score = 0;
    int score;
    for (auto const &sb: subsequencias_b){
        for (auto const &sa: subsequencias_a){
            matriz tabela;
            tabela = inicializa_matriz(tabela,sb.size(),sa.size());
            tabela = calcula_score(tabela,sb.size(),sa.size(),sa,sb, wmat, wmis, wgap);
            score = voltando(tabela,sb.size(),sa.size(), sa, sb);
            if (score > score_maximo.score ){
                score_maximo.score = score;
                score_maximo.seq1 = sa;
                score_maximo.seq2 = sb;
            }
        }
    }

    cout << score_maximo.score << endl;
    // cout << "sa: " << score_maximo.seq1 << endl;
    // cout << "sb: " << score_maximo.seq2 << endl;

    // tdata = omp_get_wtime() - tdata;
    // cout << "tempo em segundos " << tdata << endl;

   
    return 0;
}

//g++ -Wall -O3 paralelismo.cpp -o paralelismo