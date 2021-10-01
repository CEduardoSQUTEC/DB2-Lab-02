//
// Created by ceduardosq on 9/30/21.
//

#ifndef TRASHPROJECT_RECORD_H
#define TRASHPROJECT_RECORD_H


struct Record {
    char code[5]{};
    char nombre[20]{};
    char carrera[15]{};
    int ciclo{};

    Record() = default;
};

#endif //TRASHPROJECT_RECORD_H
