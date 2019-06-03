#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <cstring>

using namespace std;

//Creamos una clase, class analisis Lexico
class cAnalisisLexico{

    ifstream inFile;
    ofstream outFile;
    string nameFileIn;
    string nameFileOut;
    ifstream inPR;

    list<string> listaPR;

    void verificaArgumentos(int nargs, char**args){

        if(nargs == 1) {cout <<"No hay archivos para analizar" <<endl; exit(EXIT_FAILURE);}

        if(nargs == 2) {

            cout <<"Numero de argumentos: " <<nargs <<endl;
            cout <<"Archivo a Analizar: " <<args[1] <<endl;
            cout <<"Archivo de Salida por default: miAnalisis.txt" <<endl;
            nameFileOut = "miAnalisis.txt";
        }

        else if(nargs == 3){

            cout <<"Numero de argumentos: " <<nargs <<endl;
            cout <<"Archivo a Analizar: " <<args[1] <<endl;
            cout <<"Archivo de Salida: " <<args[2] <<endl;
            nameFileOut = args[2];
        }

        else{

            cout <<"Sólo se tomarán los dos primero Archivos" <<endl;
            cout <<"Archivo a Analizar: " <<args[1] <<endl;
            cout <<"Archivo de Salida: " <<args[2] <<endl;
            nameFileOut = args[2];
        }

        nameFileIn = args[1];
        abrirFicheros(nameFileIn, nameFileOut);

    }

    void abrirFicheros(string fileIn, string fileOut){

        inFile.open(fileIn.c_str());
        outFile.open(fileOut.c_str());

        try{
            if(inFile.fail())
                throw 1;

            if(outFile.fail())
                throw 2;

            }catch(int i){
                if(i == 1) cout <<"Error al abrir archivo inicial" <<endl;

                else if (i==2) cout <<"Error al abrir el archivo de salida" <<endl;

                else cout <<"Error inesperado" <<endl;
            }

        if(inFile.is_open() && outFile.is_open()) cout <<"Exito al abrir los archivos" <<endl;

        else cout <<"Error al abrir los Archivos" <<endl;

    }


    //Creamos el constructor sin argumentos
public:
    //

    cAnalisisLexico(){}
    //Constructor con argumentos


    cAnalisisLexico(int nargs, char**args){

        verificaArgumentos(nargs, args);
        string pr;

        try{
            inPR.open("palabrasReservadas.txt");
            if(inPR.fail()) throw 0;

            while(!inPR.eof()){

                inPR >>pr;
                listaPR.push_back(pr);
            }


        }catch(int i){
            cerr <<"Error al abrir palabras revervadas" <<endl;
        }
    }

    bool esPalabraReservada(string palabra){
        bool resp;

        for(list <string> :: iterator it = listaPR.begin(); it != listaPR.end(); it++){

            if(strcmp((*it).c_str(), palabra.c_str()) == 0 ){
                resp = true;
                break;

            }
        }
        return resp;
    }


    void analizar(){

        string lex;
        char c;
        while(!inFile.eof()){
            //inFile>>lex;
            //outFile<<lex;
            c = inFile.get();
            outFile<<c;
        }

    }

    void analizarLetra(){

        string id;

        while(inFile.good()){

            char lex = 0;
            inFile.get(lex);
            if(lex == '(') outFile<< "TokPI";
            else if(lex == ';') outFile<< "TokPyC";
            else if(lex == '<') outFile<< "TokMenor";
            else if(lex == '>') outFile<< "TokMayor";
            else if(lex == '/'){
                lex=inFile.get();
            }

            else if(isalpha(lex) || lex=='_'){
                id = lex;
                lex=inFile.get();
                while(isalnum(lex) || lex == '_'){
                    id += lex;
                    lex=inFile.get();
                }
                outFile<<"TokID,"<<id<<")";
                inFile.unget();
            }

            else outFile<<lex;
        }
    }

    void recuperaTokens(){

        string id;

        while(inFile.good()){

            char lex = 0;
            inFile.get(lex);
            if(lex == '(') outFile<< "TokPI";
            else if(lex == ';') outFile<< "TokPyC";
            else if(lex == '<') outFile<< "TokMenor";
            else if(lex == '>') outFile<< "TokMayor";

            else if(isalpha(lex) || lex=='_'){
                id = lex;
                lex=inFile.get();
                while(isalnum(lex) || lex == '_'){
                    id += lex;
                    lex=inFile.get();
                }
                cout<<"TokID";
                inFile.unget();
            }

            else outFile<<lex;
        }
    }

};


int main(int nargs, char**args){

    cAnalisisLexico miAnalisis(nargs, args);

    //miAnalisis.analizar();
    miAnalisis.analizarLetra();
    miAnalisis.recuperaTokens();


    /*if(miAnalisis.esPalabraReservada("else")) cout <<"Es reservada"<<endl;

    else cout <<"No es reservada"<<endl; */

    return 0;

}
