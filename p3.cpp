#include <iostream>
#include <fstream>
#include <vector>

struct Matricula
{
    std::string codigo;
    int ciclo;
    float mensualidad;
    std::string observaciones;
};

class VariableRecord
{
private:
    std::string file;
    std::string metadatafile;

public:
    VariableRecord(const std::string &dataFile, const std::string &metadataFile)
            : file(dataFile), metadatafile(metadataFile) {}

    std::vector<Matricula> load()
    {
        std::vector<Matricula> records;
        std::ifstream dataFile(file, std::ios::binary);
        std::ifstream metadataFile(metadatafile, std::ios::binary);
        std::vector<std::streampos> positions;
        std::streampos pos;

        while (metadataFile.read((char *)&pos, sizeof(std::streampos)))
        {
            positions.push_back(pos);
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            dataFile.seekg(positions[i]);
            Matricula record = readRecord(dataFile);
            records.push_back(record);
        }

        return records;
    }

    Matricula readRecord(int pos)
    {
        std::ifstream dataFile(file, std::ios::binary);
        std::ifstream metadataFile(metadatafile, std::ios::binary);

        metadataFile.seekg(pos * sizeof(std::streampos));
        std::streampos dataPos;
        metadataFile.read((char *)&dataPos, sizeof(std::streampos));

        dataFile.seekg(dataPos);
        return readRecord(dataFile);
    }

    Matricula readRecord(std::ifstream &dataFile)
    {
        Matricula record;

        int lengthCodigo;
        dataFile.read((char *)&lengthCodigo, sizeof(int));
        record.codigo.resize(lengthCodigo);
        dataFile.read(&record.codigo[0], lengthCodigo);

        dataFile.read((char *)&record.ciclo, sizeof(int));

        dataFile.read((char *)&record.mensualidad, sizeof(float));

        int lengthObservaciones;
        dataFile.read((char *)&lengthObservaciones, sizeof(int));
        record.observaciones.resize(lengthObservaciones);
        dataFile.read(&record.observaciones[0], lengthObservaciones);

        return record;
    }
};

int main()
{
    VariableRecord records("data_p3_data.dat", "data_p3_metadata.dat");

    std::vector<Matricula> allRecords = records.load();

    for (const auto &rec : allRecords)
    {
        std::cout << "Codigo: " << rec.codigo << ", Ciclo: " << rec.ciclo
                  << ", Mensualidad: " << rec.mensualidad
                  << ", Observaciones: " << rec.observaciones << std::endl;
    }

    Matricula firstRecord = records.readRecord(4);
    std::cout << "Codigo: " << firstRecord.codigo << ", Ciclo: " << firstRecord.ciclo
              << ", Mensualidad: " << firstRecord.mensualidad
              << ", Observaciones: " << firstRecord.observaciones << std::endl;

    Matricula secondRecord = records.readRecord(8);
    std::cout << "Codigo: " << secondRecord.codigo << ", Ciclo: " << secondRecord.ciclo
              << ", Mensualidad: " << secondRecord.mensualidad
              << ", Observaciones: " << secondRecord.observaciones << std::endl;

    return 0;
}
