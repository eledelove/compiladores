#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class cToken{

    protected:
    string nomToken;
    string lexema;
    public:

        //friend class cAnalisisLexico;
        void setNomToken(string nomTok){
            this -> nomToken=nomTok;
        }
        void setLexema(string lex="123"){
            this -> lexema = lex;
        }
        string getName(){return this -> nomToken;}
        string getLexema(){return this-> lexema;}

};

template <class T> class ctokenNum : public cToken{

    T valorNum;
    void setValor(T vN){
        this ->valorNum=vN;
    }

    T getValor(){
        return this -> valorNum;
    }

    T convierteToNum(){
        stringstream aux = lexema;
        aux>>valorNum;
        return valorNum;
    }
};

//Creamos una clase, class analisis Lexico
class cAnalisisLexico{

    ifstream inFile;
    ofstream outFile;
    ifstream inPR;

    string nameFileIn;
    string nameFileOut;

    list<string> listaPR;
    vector<cToken> bufferToken;
    typedef void (cAnalisisLexico::*punteroFuncionToken)();
    map<char,punteroFuncionToken> tipoToken;

    //Creamos el constructor sin argumentos
public:
    //

    cAnalisisLexico(){}
    //Constructor con argumentos


    cAnalisisLexico(int nargs, char**args){

        verificaArgumentos(nargs, args);

    }


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

            cout <<"Solo se tomaran los dos primero Archivos" <<endl;
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

        inPR.open("palabrasReservadas.txt");
        string pr;

        try{
            if(inFile.fail())
                throw 1;

            if(outFile.fail())
                throw 2;

            if(inPR.fail())
                throw 3;

            }catch(int i){
                if(i == 1) cout <<"Error al abrir archivo inicial" <<endl;

                else if (i==2) cout <<"Error al abrir el archivo de salida" <<endl;

                else if (i==3) cout <<"Error al abrir PalabrasReservadas" <<endl;

                else cout <<"Error inesperado" <<endl;
            }

        if(inFile.is_open() && outFile.is_open() && inPR.is_open()){
            cout <<"Exito al abrir los archivos" <<endl;
            while(!inPR.eof()){

            inPR >>pr;
            listaPR.push_back(pr);
            }
        }

        else cout <<"Error al abrir los Archivos" <<endl;


        for(list<string> :: iterator i=listaPR.begin(); i != listaPR.end(); i++){
            cout<<*i<<endl;
        }
    }


    void analizar(){

        char c=0;
        while(!inFile.eof()){

            c = inFile.get();
            outFile<<c;
        }

    }

    void analizarLetra(){


        while(inFile.good()){
            char c=0;
            inFile.get(c);
            outFile<<c;
        }
    }

    bool esReservada(char * id){

        bool reservada = false;

        for(list <string>:: iterator i=listaPR.begin(); i != listaPR.end(); i++){
            if(strcmp(id, (*i).c_str()) == 0){
                reservada = true;
                break;
            }
        }
        return reservada;
    }

    void TokenPI(){
        outFile<<"TokenPI";
    }

    void recuperaTokens(){

        string id;

        tipoToken['(']=&cAnalisisLexico::TokenPI;
        /*tipoToken[';']=&cAnalisisLexico::TokenPyC;
        tipoToken['/']=&cAnalisisLexico::TokenDIv;
        tipoToken['<']=&cAnalisisLexico::TokenMenor;
        tipoToken['>']=&cAnalisisLexico::TokenMayor;
        tipoToken['[']=&cAnalisisLexico::TokenCorcheteIzq;*/

        while(inFile.good()){

            char lex =0;
            inFile.get(lex);
            if(lex =='(')
                (this->*tipoToken[lex])();

            /*if(lex == '(') outFile<< "TokPI";
            else if(lex == ')') outFile<< "TokPD";
            else if(lex == ';') outFile<< "TokPyC";
            else if(lex == '<') outFile<< "TokMenor";
            else if(lex == '>') outFile<< "TokMayor";*/
            else if(lex == '+' || lex== '-' || isdigit(lex) ){

                string num;
                num=lex;

                lex=inFile.get();
                if(lex == '+') outFile<<"TokIncremento++";
                else if (lex == '-') outFile<<"TokDecremento--";
                else if (lex == '=') outFile<<"TokIncremento+=";
                else if (isalpha(lex) && toupper(lex) != 'E') outFile<<"TokOPArit"<<lex;

                else{

                while(isdigit(lex)){
                    num += lex;
                    lex=inFile.get();

                }
                if(lex == '.'){
                    num += lex;
                    lex=inFile.get();
                    while(isdigit(lex)){
                        num+=lex;
                        lex = inFile.get();
                }
                if(toupper(lex) != 'E')
                outFile<<"TokFloat"<<num;

                else {//if(toupper(lex) == 'E'){

                    parteFinal:
                    num +=lex;
                    lex = inFile.get();

                    if(lex == '+' || lex == '-' || isdigit(lex)){
                        num += lex;
                        lex = inFile.get();

                        while(isdigit(lex)){
                        num+=lex;
                        lex = inFile.get();
                        }

                        outFile<<"TokNumExp"<<num;

                    }
                    else outFile<<"Error en el formato";
                    }


                }

                else if(toupper(lex) == 'E') goto parteFinal;

                else
                outFile<<"TOKint"<<num;
                inFile.unget();

                }

            }
            else if(lex == '/'){
                lex = inFile.get();
                if(lex == '/'){
                    lex = inFile.get();
                    while (lex != '\n') lex = inFile.get();
                    inFile.unget();
                    outFile<<"Linea_Comentario";
                }

                else if(lex == '*'){

                    do{
                    while(inFile.get() != '*'); //se sale del while si llega un asterisco
                    inFile.unget();
                    while(inFile.get() == '*');
                    inFile.unget();
                    if(inFile.get() == '/'){
                    outFile<<"Comenario_Largo";
                    break;
                    }
                    inFile.unget();
                    }
                    while(inFile.get() != '/');
                }

                else outFile<<"TOkDiv";
            }


            else if(isalpha(lex) || lex=='_'){
                id = lex;
                lex=inFile.get();
                while(isalnum(lex) || lex == '_'){
                    id += lex;
                    lex=inFile.get();
                }

                if(!esReservada((char*)id.c_str()))
                    outFile<<"TokID("<<id<<")";
                else
                    outFile<<"TokReservada("<<id<<")";

                inFile.unget();
            }

            else outFile<<lex;
        }
    }

};




int main(int nargs, char**args){

    cAnalisisLexico miAnalisis(nargs, args);

    //miAnalisis.analizar();
    //miAnalisis.analizarLetra();
    miAnalisis.recuperaTokens();




    return 0;

}
