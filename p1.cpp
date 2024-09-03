#include <iostream>
#include <vector>
#include <fstream>

struct Alumno {
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    int ciclo;
    float mensualidad;
};

class FixedRecord {
private:
    std::vector<Alumno> alumnos;
    std::fstream file;
    std::vector<int> freePos;
    int delMode;

public:
    FixedRecord(std::string fileName, std::string mode) {
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

        if(mode == "MOVE_THE_LAST") {
            delMode = 1;
        } else if(mode == "FREE_LIST") {
            delMode = 2;
        }
    }

    ~FixedRecord() {
        if (file.is_open()) {
            file.close();
        }
    }

    std::vector<Alumno> load() {
        file.seekg(0, std::ios::beg);

        while(true) {
            Alumno alumno;
            file.read((char*) &alumno, sizeof(alumno));

            if(file.eof()) break;
            alumnos.push_back(alumno);
        }

        return alumnos;
    }

    void add(Alumno record) {
        if(delMode == 2 && !freePos.empty()) {
            int pos = freePos.back();
            freePos.pop_back();

            file.seekp(pos * sizeof(Alumno), std::ios::beg);
        } else {
            file.seekp(0, std::ios::end);
        }
        file.write((char*) &record, sizeof(record));
        file.flush();
    }

    Alumno readRecord(int pos) {
        Alumno alumno;

        file.seekg(pos * sizeof(Alumno));
        file.read((char*) &alumno, sizeof(alumno));
        return alumno;
    }

    // del = delete
    bool del(int pos) {
        file.seekg(0, std::ios::end);
        int lastPos = file.tellg() / sizeof(Alumno) - 1;

        if (pos < 0 || pos > lastPos) {
            return false;
        }

        if(delMode == 1) {
            Alumno lastRecord = readRecord(lastPos);

            file.seekp(pos * sizeof(Alumno), std::ios::beg);
            file.write((char*) &lastRecord, sizeof(lastRecord));

            // Overwrite last record with empty record
            Alumno emptyRecord = {};
            file.seekp(lastPos * sizeof(Alumno), std::ios::beg);
            file.write((char*) &emptyRecord, sizeof(emptyRecord));

        } else if(delMode == 2) {
            freePos.push_back(pos);

            // Overwrite record in pos with empty record
            Alumno emptyRecord = {};
            file.seekp(pos * sizeof(Alumno), std::ios::beg);
            file.write((char*) &emptyRecord, sizeof(emptyRecord));
        }

        file.flush();
        return true;
    }
};

int main() {
    FixedRecord records("data.dat", "FREE_LIST");

    Alumno al1 = {"1", "John", "Doe", "CS", 4, 1500.0f};
    Alumno al2 = {"2", "Amanda", "Diggins", "BioIng", 1, 1000.0f};
    Alumno al3 = {"3", "Edward", "Hopkins", "DS", 3, 1650.0f};
    Alumno al4 = {"4", "Renato", "Santos", "CS", 6, 1340.0f};

    records.add(al1);
    records.add(al2);
    records.add(al3);
    records.add(al4);

    records.del(1); // Elimina a "Amanda" (Pos 1)

    Alumno al5 = {"5", "Luciano", "Aguirre", "CS", 5, 1300.0f};
    records.add(al5); // Aniade a "Luciano" a Pos 1 (Posicion libre)

    auto alumnos = records.load();
    for (const auto& alumno : alumnos) {
        std::cout << "Codigo: " << alumno.codigo << "\n"
                  << "Nombre: " << alumno.nombre << "\n"
                  << "Apellidos: " << alumno.apellidos << "\n"
                  << "Carrera: " << alumno.carrera << "\n"
                  << "Ciclo: " << alumno.ciclo << "\n"
                  << "Mensualidad: " << alumno.mensualidad << "\n\n";
    }

    return 0;
}
