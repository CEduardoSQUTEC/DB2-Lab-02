#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

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

struct comparator{
  inline bool operator() (const Registro& registro1, const Registro& registro2){
    if(string(registro1.nombre).compare(string(registro2.nombre))<0)
    return true;

    return false;
  }
};

class Sequential_File{
  private:
  int mainSize = 0;
  int auxSize = 0;

  public:
  void insertAll(vector<Registro> registros){
    mainSize= 0;
    auxSize = 0;
    //Borrado de datos
    ofstream files;
    files.open("datos.bin", std::ofstream::out | std::ofstream::trunc);
    files.close();
    files.open("aux.bin", std::ofstream::out | std::ofstream::trunc);
    files.close();

    sort(registros.begin(), registros.end(), comparator());

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
    ifstream datos;
    datos.open("datos.bin", ios::binary);
    vector<Registro> registros = {};
    Registro registro;
    if(datos.is_open()){
      datos.seekg(0,ios::beg);
      while (datos.read((char *) &registro, sizeof(Registro))) {
        registros.push_back(registro);
      }
    }
    else{cout<<"\nNo se pudo abrir el archivo";}

    datos.open("aux.bin", ios::binary);
    if(datos.is_open()){
      datos.seekg(0,ios::beg);
      while (datos.read((char *) &registro, sizeof(Registro))) {
        registros.push_back(registro);
      }
    }
    else{cout<<"\nNo se pudo abrir el archivo";}

    datos.close();
    return registros;
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
        auxSize++;
      }
    }else{
      cout<<"No se pudo abrir el archivo";
    }
    datos.close();
  }

  bool readNext(Registro & registro){
    ifstream datos;
    datos.open("datos.bin", ios::binary);

    ifstream datos_aux;
    datos_aux.open("aux.bin", ios::binary);

    if(registro.nextLocation == 'd'){
        datos.seekg(registro.nextPos, ios::beg);
		    if(datos.read((char*)&registro, sizeof(Registro))){
          datos.read((char*)&registro, sizeof(Registro));
          datos.close();
          datos_aux.close();
          return true;
        }
	    }
    if(registro.nextLocation == 'a'){
		  datos_aux.seekg(registro.nextPos, ios::beg);
		  if(datos_aux.read((char*)&registro, sizeof(Registro))){
        datos_aux.read((char*)&registro, sizeof(Registro));
        datos.close();
        datos_aux.close();
        return true;
      }
	  }
    datos.close();datos_aux.close();return false;
  }

  Registro search(string key){
    ifstream datos;
    datos.open("datos.bin", ios::binary);

    Registro registro;
    int a = 0;
    int b = mainSize-1;
    int m;

    do{
      m = ceil((a+b)/2);
      datos.seekg(m*sizeof(Registro), ios::beg);
      datos.read((char*)&registro, sizeof(Registro));
      if(string(registro.nombre).compare(key) < 0){
		    b = m-1;
      }
      else if (string(registro.nombre).compare(key) > 0) {
	      a = m;
      }
      else{
        return registro;
      }
    }while(a<b);

    m = ceil((a+b)/2);
    datos.seekg(m*sizeof(Registro), ios::beg);
    datos.read((char*)&registro, sizeof(Registro));
    Registro registroNext = registro;

    while(string(registro.nombre).compare(key) < 0 && readNext(registroNext)){
      if(string(registroNext.nombre).compare(key) == 0){
        return registroNext;
      }else if(string(registroNext.nombre).compare(key) > 0){
        return registro;
      }
      registro = registroNext;
    }

    datos.close();
    return registro;
  }

  vector<Registro> rangeSearch(string begin, string end){
    vector<Registro> registros = {};
    Registro registro = search(begin);

    if(string(registro.nombre).compare(begin)< 0 || string(registro.nombre).compare(end) > 0){
      readNext(registro);
    }
    while(string(registro.nombre).compare(begin)>= 0 && string(registro.nombre).compare(end)<=0){
      registros.push_back(registro);
      if(!readNext(registro)){
        break;
      };
    }
    return registros;
  }
};

int main() {
  
}
