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

struct Registro {
    char codigo[5];
    char nombre[20];
    char carrera[15];
    int ciclo;

    int pos;
    char location;
    int nextPos;
    char nextLocation;

    Registro(string codigo, string nombre, string carrera, int ciclo) {
        strcpy(this->codigo, codigo.c_str());
        strcpy(this->nombre, nombre.c_str());
        strcpy(this->carrera, carrera.c_str());
        this->ciclo = ciclo;
    }

    Registro() {
    }

    void display() {
        cout << "\n" << string(codigo, sizeof(codigo));
        cout << string(nombre, sizeof(nombre));
        cout << string(carrera, sizeof(carrera));
        cout << ciclo;
    }
};

struct comparator {
    inline bool operator()(const Registro &registro1, const Registro &registro2) {
        if (string(registro1.nombre).compare(string(registro2.nombre)) < 0)
            return true;

        return false;
    }
};

class Sequential_File {
private:
    int mainSize = 0;
    int auxSize = 0;

public:
    void insertAll(vector<Registro> registros) {
        mainSize = 0;
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
        if (datos.is_open()) {
            for (auto registro: registros) {

                registro.pos = datos.tellp();
                registro.location = 'd';
                registro.nextPos = int(datos.tellp()) + sizeof(Registro);
                registro.nextLocation = 'd';
                datos.write((char *) &registro, sizeof(Registro));
                mainSize++;

            }
        } else {
            cout << "No se pudo abrir el archivo";
        }
        datos.close();
    }

    vector<Registro> getAll() {
        ifstream datos;
        datos.open("datos.bin", ios::binary);
        vector<Registro> registros = {};
        Registro registro;
        if (datos.is_open()) {
            datos.seekg(0, ios::beg);
            while (datos.read((char *) &registro, sizeof(Registro))) {
                registros.push_back(registro);
            }
        } else { cout << "\nNo se pudo abrir el archivo"; }

        datos.open("aux.bin", ios::binary);
        if (datos.is_open()) {
            datos.seekg(0, ios::beg);
            while (datos.read((char *) &registro, sizeof(Registro))) {
                registros.push_back(registro);
            }
        } else { cout << "\nNo se pudo abrir el archivo"; }

        datos.close();
        return registros;
    }

    void update(Registro registro) {
        ofstream datos;
        if (registro.location == 'd') {
            datos.open("datos.bin", ios::binary | ios::in);
        } else {
            datos.open("aux.bin", ios::binary | ios::in);
        }

        if (datos.is_open()) {
            int pos = registro.pos;
            datos.seekp(pos, ios::beg);
            datos.write((char *) &registro, sizeof(Registro));

        } else {
            cout << "No se pudo abrir el archivo";
        }
        datos.close();
    }

    void add(Registro registro) {
        ofstream datos;
        datos.open("aux.bin", ios::binary | ios::app);
        if (datos.is_open()) {
            if (auxSize == 5) {
                vector<Registro> registros = getAll();
                insertAll(registros);
            } else {
                auto prev = search(registro.nombre);

                registro.location = 'a';
                registro.pos = datos.tellp();

                registro.nextPos = prev.nextPos;
                registro.nextLocation = prev.nextLocation;

                prev.nextPos = datos.tellp();
                prev.nextLocation = 'a';

                update(prev);

                datos.write((char *) &registro, sizeof(Registro));
                auxSize++;
            }
        } else {
            cout << "No se pudo abrir el archivo";
        }
        datos.close();
    }

    bool readNext(Registro &registro) {
        ifstream datos;
        datos.open("datos.bin", ios::binary);

        ifstream datos_aux;
        datos_aux.open("aux.bin", ios::binary);

        if (registro.nextLocation == 'd') {
            datos.seekg(registro.nextPos, ios::beg);
            if (datos.read((char *) &registro, sizeof(Registro))) {
                datos.read((char *) &registro, sizeof(Registro));
                datos.close();
                datos_aux.close();
                return true;
            }
        }
        if (registro.nextLocation == 'a') {
            datos_aux.seekg(registro.nextPos, ios::beg);
            if (datos_aux.read((char *) &registro, sizeof(Registro))) {
                datos_aux.read((char *) &registro, sizeof(Registro));
                datos.close();
                datos_aux.close();
                return true;
            }
        }
        datos.close();
        datos_aux.close();
        return false;
    }

    Registro search(string key) {
        key.erase(remove(key.begin(), key.end(), ' '), key.end());
        ifstream datos;
        datos.open("datos.bin", ios::binary);

        Registro registro;
        int a = 0;
        int b = mainSize - 1;
        int m;

        do {
            m = ceil((a + b) / 2);
            datos.seekg(m * sizeof(Registro), ios::beg);
            datos.read((char *) &registro, sizeof(Registro));
            if (string(registro.nombre).compare(key) < 0) {
                b = m - 1;
            } else if (string(registro.nombre).compare(key) > 0) {
                a = m;
            } else {
                return registro;
            }
        } while (a < b);

        m = ceil((a + b) / 2);
        datos.seekg(m * sizeof(Registro), ios::beg);
        datos.read((char *) &registro, sizeof(Registro));
        Registro registroNext = registro;

        while (string(registro.nombre).compare(key) < 0 && readNext(registroNext)) {
            string nombre_n = string(registroNext.nombre, 15);
            nombre_n.erase(remove(nombre_n.begin(), nombre_n.end(), ' '), nombre_n.end());
            if (nombre_n.compare(key) == 0) {
                return registroNext;
            } else if (nombre_n.compare(key) > 0) {
                return registro;
            }
            registro = registroNext;
        }

        datos.close();
        return registro;
    }

    vector<Registro> rangeSearch(string begin, string end) {

        begin.erase(remove(begin.begin(), begin.end(), ' '), begin.end());
        end.erase(remove(end.begin(), end.end(), ' '), end.end());

        vector<Registro> registros = {};
        Registro registro = search(begin);

        string nombre_r = string(registro.nombre, 15);
        nombre_r.erase(remove(nombre_r.begin(), nombre_r.end(), ' '), nombre_r.end());

        if (nombre_r.compare(begin) < 0 || nombre_r.compare(end) > 0) {
            readNext(registro);
        }
        while (nombre_r.compare(begin) >= 0 && nombre_r.compare(end) <= 0) {
            registros.push_back(registro);
            if (!readNext(registro)) {
                break;
            };
        }
        return registros;
    }
};

int main() {
    cout << "=== Sequential File Tests ===";
    vector<Registro> registros = {};
    Registro registro("0008 ", "Juan Pablo          ", "CS             ", 6);
    Registro registro2("0007 ", "Pedro Pablo         ", "CS             ", 7);
    Registro registro3("0006 ", "Perez Pablo         ", "CS             ", 4);
    registros.push_back(registro);
    Sequential_File s;

    s.insertAll(registros);

    s.add(registro);
    s.add(registro3);
    s.add(registro2);

    cout << "Simple search" << endl;
    registro = s.search("Perez Pablo");
    registro.display();

    cout << endl << endl << endl << "Range search" << endl;
    registros = s.rangeSearch("Juan Pablo", "Perez Pablo");
    for (auto registro: registros) {
        registro.display();
    }

    cout << "=== ISAM Tests ===";

}
