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

// FixedRecord con metodo MOVE_THE_LAST
class FixedRecord {
private:
    std::string fileName;
    const int offset = sizeof(int);

public:
    std::vector<Alumno> alumnos;

    FixedRecord(std::string fileName) {
        this->fileName = fileName;
        std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);

        if (!file) {
            std::ofstream newFile(fileName, std::ios::out | std::ios::binary);
            int initialSize = 0;
            newFile.write((char*) &initialSize, offset);
            newFile.close();
        }
    }

    int getSize() {
        std::fstream file(fileName, std::ios::in | std::ios::binary);
        int size;
        file.seekg(0, std::ios::beg);
        file.read((char*) &size, offset);
        file.close();
        return size;
    }

    void setSize(int size) {
        std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(0, std::ios::beg);
        file.write((char*) &size, offset);
        file.close();
    }

    std::vector<Alumno> load() {
        std::fstream file(fileName, std::ios::in | std::ios::binary);

        int size = getSize();
        file.seekg(offset, std::ios::beg);
        for (int i = 0; i < size; i++) {
            Alumno alumno;
            file.read((char*) &alumno, sizeof(alumno));
            alumnos.push_back(alumno);
        }
        file.close();

        return alumnos;
    }

    void add(Alumno record) {
        std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);

        int size = getSize();
        file.seekp(offset + size * sizeof(Alumno), std::ios::beg);
        file.write((char*) &record, sizeof(record));
        setSize(size + 1);

        file.close();
    }

    Alumno readRecord(int pos) {
        std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);
        Alumno alumno;

        file.seekg(offset + pos * sizeof(Alumno), std::ios::beg);
        file.read((char*) &alumno, sizeof(alumno));
        file.close();
        return alumno;
    }

    // del = delete
    bool del(int pos) {
        std::fstream file(fileName, std::ios::in | std::ios::out | std::ios::binary);

        int size = getSize();
        if (pos < 0 || pos >= size) {
            return false;
        }

        Alumno lastRecord = readRecord(size - 1);
        file.seekp(offset + pos * sizeof(Alumno), std::ios::beg);
        file.write((char*) &lastRecord, sizeof(lastRecord));

        setSize(size - 1);

        file.close();
        return true;
    }
};

int main() {
    // ** IMPORTANTE ** No debe existir el archivo data1-1.dat antes de ejecutar el codigo.
    FixedRecord records("data1-1.dat");

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

    std::cout << "Size: " << records.getSize() << " records.\n";

    return 0;
}
