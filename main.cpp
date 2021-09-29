#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
using namespace std;

struct Registro{
  char codigo [5];
  char nombre [20];
  char carrera [15];
  int ciclo;

  int pos;
  char location;
  int nextPos;
  char nextLocation;

};

class Sequential_File{
  private:
  int mainSize = 0;
  int auxSize = 0;

  public:
  void insertAll(vector<Registro> registros){
    //registros ordenados
    ofstream datos;
    datos.open("datos.bin", ios::binary | ios::in);
    if(datos.is_open()){
      for(auto registro:registros){

        registro.pos = datos.tellp();
        registro.location = 'd';
        registro.nextPos = int(datos.tellp()) + sizeof(Registro);
        registro.nextLocation = 'd';
        datos.write((char*)&registro, sizeof(Registro));
        mainSize++;

      }
    }else{
      cout<<"No se pudo abrir el archivo";
    }
    datos.close();
  }

  vector<Registro> getAll(){

  }

  void update(Registro registro){
    ofstream datos;
    if(registro.location == 'd'){
      datos.open("datos.bin", ios::binary | ios::in);
    }else{
      datos.open("aux.bin", ios::binary | ios::in);
    }
    
    if(datos.is_open()){
      int pos = registro.pos;
      datos.seekp(pos,ios::beg);
      datos.write((char*)&registro, sizeof(Registro));

    }else{
      cout<<"No se pudo abrir el archivo";
    }
    datos.close();
  }

  void add(Registro registro){
    ofstream datos;
    datos.open("aux.bin", ios::binary | ios::app);
    if(datos.is_open()){
      if(auxSize == 5){
        vector<Registro> registros = getAll();
        insertAll(registros);
      }
      else{
        auto prev = search(registro.nombre);
        prev.nextPos = datos.tellp();
        prev.nextLocation = 'a';

        update(prev);

        registro.location = 'a';
        registro.pos = datos.tellp();

        registro.nextPos = prev.nextPos;
        registro.nextLocation = prev.nextLocation;

        datos.write((char*)&registro, sizeof(Registro));
      }
    }else{
      cout<<"No se pudo abrir el archivo";
    }
    datos.close();
  }

  Registro search(string key){

  }

  vector<Registro> rangeSearch(string begin, string end){

  }

};


int main() {
  
}