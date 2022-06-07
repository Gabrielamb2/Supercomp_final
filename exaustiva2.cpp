#include<iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <list>
#include<cstring>
#include <random>
#include <unordered_set>
using namespace std;

//g++ -Wall -O3 exaustiva2.cpp -o exaustiva2
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

typedef std::vector<vector<item>>matriz;
matriz inicializa_matriz(matriz mat, int n, int m) {
    
    for (int linhas = 0; linhas <= n ; linhas++) {
        std::vector<item> linha;
        for (int coluna = 0; coluna <= m ; coluna++) {
            linha.push_back({0, "zero"});    
        }
        mat.push_back(linha);
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
    
    for (int linhas = 1; linhas <= n ; linhas++) {
        for (int coluna = 1; coluna <= m ; coluna++) {
            int diagonal;
            int delecao;
            int insercao;
            if (seq1[coluna-1] == seq2[linhas-1]){
                diagonal = mat[linhas-1][coluna-1].valor + match;
            }else{
                diagonal = mat[linhas-1][coluna-1].valor +  mismatch;
            }
            delecao = mat[linhas-1][coluna].valor +  mismatch ;
            insercao = mat[linhas][coluna-1].valor +  gap;
            mat[linhas][coluna].traceback = calcula_max(diagonal,delecao,insercao);
            mat[linhas][coluna].valor = maximo(diagonal,delecao,insercao);
            mat[linhas][coluna].linha = linhas;
            mat[linhas][coluna].coluna = coluna;
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
    for (int a = 0; a <= n ; a++) {
        
        for (int b = 0; b <= m ; b++) {
           if(mat[a][b].valor>maximo.valor){
            maximo= mat[a][b];
        }
        }
    }

    
    return maximo.valor;
    
}
int score_seq(string seq1, string seq2, int n, int match, int mismatch){
    int score = 0;
    for(int i =0; i<n ; i++){
        if (seq1[i] ==seq2[i]){
            score += match;
        }else{
            score += mismatch;
        }
    }
    return score;
}

int main(){
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
            // cout << a_linha << endl;
            subsequencias_a.push_back(a_linha);
        }
    }

    vector<string>subsequencias_b;
    for(int e =0; e<b ; e++){
        string b_linha;
        for(int i =e; i<b ; i++){
            b_linha += seqB[i];
            // cout << b_linha << endl;
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
            if(sb.size() == sa.size()){
                score =  score_seq(sa, sb, sb.size(), wmat, wmis);
            }else{
                matriz tabela;
                tabela = inicializa_matriz(tabela,sb.size(),sa.size());
                tabela = calcula_score(tabela,sb.size(),sa.size(),sa,sb, wmat, wmis, wgap);
                score = voltando(tabela,sb.size(),sa.size(), sa, sb);
            }
            
            if (score > score_maximo.score ){
                score_maximo.score = score;
                score_maximo.seq1 = sa;
                score_maximo.seq2 = sb;
            }
        }
    }

    cout << score_maximo.score;
    // cout << "sa: " << score_maximo.seq1 << endl;
    // cout << "sb: " << score_maximo.seq2 << endl;


   
    return 0;
}

//g++ -Wall -O3 exaustiva2.cpp -o exaustiva2