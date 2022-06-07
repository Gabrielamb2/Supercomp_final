#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include<bits/stdc++.h>
#include <omp.h>
using namespace std;
using std::max;

struct item_g {
    long item_score;
    string seq_a;
    string seq_b;
};

struct combinacao {
    int value;
    string seq_a;
    string seq_b;
};

struct item {
    int valor;
    string traceback; 
    int coluna;
    int linha;
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

int func_maior_score(combinacao el,int wmat,int wmis,int wgap){
    matriz tabela;
    int score;
    tabela = inicializa_matriz(tabela,el.seq_b.size(),el.seq_a.size());
    tabela = calcula_score(tabela,el.seq_b.size(),el.seq_a.size(),el.seq_a,el.seq_b, wmat, wmis, wgap);
    score = voltando(tabela,el.seq_b.size(),el.seq_a.size(),el.seq_a, el.seq_b);

    return score;
    
}

int main() {
	float time;
    time = omp_get_wtime();

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
    vector<string>subseqs_a;
    for(int e =0; e<a ; e++){
        string a_linha;
        for(int i =e; i<a ; i++){
            a_linha += seqA[i];
            subseqs_a.push_back(a_linha);
        }
    }

    vector<string>subseqs_b;
    for(int e =0; e<b ; e++){
        string b_linha;
        for(int i =e; i<b ; i++){
            b_linha += seqB[i];
            subseqs_b.push_back(b_linha);
        }
    }

//------------------------Calcular os alinhamentos de cada par de subsequencias (a´, b´) com os pesos wmat, wmis e wgap-------------------------------------------
    item melhor;    
    vector<combinacao> combincaoes;      
    vector<item_g> results, melhores;
    results.resize(800000);
    for (int e =0; e<(int)combincaoes.size(); e++){ 
        results.push_back({0,seqA,seqB});
    }

    int i=0;
    int melhor_score= -1;
    int melhor_valor_geral = -1;
    int score;
    for (auto& sub_a : subseqs_a){
        for (auto& sub_b : subseqs_b){
            combincaoes.push_back({i,sub_a, sub_b});
            i+=1;
            if (i>=800000){ 
                #pragma omp parallel for reduction(max:melhor_score)
                for (auto& el : combincaoes){ 
                    score = func_maior_score(el, wmat, wmis, wgap);
                    if (score>melhor_score)melhor_score=score;
                } 

                for (int i=0; i<(int)results.size(); i++){
                    if (melhor_score > melhor_valor_geral){
                        melhor_valor_geral = melhor_score;
                    }
                }
            i = 0;   
            combincaoes.clear();   //zera o vetor           
            }
        }
    }

// Calculando o score para os valores restantes
    #pragma omp parallel for reduction(max:melhor_score)
    for (auto& el : combincaoes){ 
            score = func_maior_score(el, wmat, wmis, wgap);
            if (score>melhor_score)melhor_score=score;
            
    }
    
        
    for (int i=0; i<(int)results.size(); i++){
        if (melhor_score > melhor_valor_geral){
            melhor_valor_geral = melhor_score;
        }
    }



    cout << melhor_valor_geral;
    time = omp_get_wtime() - time;
    // cout << "Tempo em segundos : " << time << endl;


     return 0;

}


// Para compilar: 

// g++ -Wall -O3 -fopenmp -g paralelismo_omp.cpp -o paralelismo_omp
// ./paralelismo_omp <entradas3/in0.txt 


