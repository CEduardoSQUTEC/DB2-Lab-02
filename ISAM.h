//
// Created by ceduardosq on 9/30/21.
//

#ifndef TRASHPROJECT_ISAM_H
#define TRASHPROJECT_ISAM_H

#include <string>
#include <fstream>
#include "ISAMNode.h"

template<typename T>
class ISAM {
    std::string _filename{};
    std::string _index[3]{};
    ISAMNode<T> *_root = nullptr;
public:
    ISAM() = default;

    ISAM(const std::string &filename) {
        _filename = filename;
        std::fstream file(filename, std::ios::ate);
        if (!file.is_open()) throw std::invalid_argument("Invalid file.");
        long size = file.tellg();
        if (size == 27 * sizeof(T)) throw std::invalid_argument("File must have 27 records.");
        file.close();

        std::string prefix = filename.substr(0, filename.find(".dat"));
        for (int i = 0; i < 3; ++i) {
            _index[i] = prefix + ".index" + std::to_string(i) + ".dat";
            std::ofstream index(_index[i], std::ios::trunc);
            if (!index.is_open()) throw std::bad_exception("Cannot create index files.");
        }
    }



};


#endif //TRASHPROJECT_ISAM_H
