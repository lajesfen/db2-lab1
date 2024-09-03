#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Alumno
{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;
};

class VariableRecord
{
private:
    string file;

    vector<string> split(const string &line, char delimiter)
    {
        vector<string> result;
        size_t start = 0;
        size_t end = line.find(delimiter);

        while (end != string::npos)
        {
            result.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find(delimiter, start);
        }
        result.push_back(line.substr(start));

        return result;
    }

public:
    VariableRecord(string file) : file(file) {}

    vector<Alumno> load()
    {
        vector<Alumno> alumnos;
        ifstream file(this->file);

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            vector<string> fields = split(line, '|');
            if (fields.size() != 4)
                continue;

            Alumno alumno;
            alumno.Nombre = fields[0];
            alumno.Apellidos = fields[1];
            alumno.Carrera = fields[2];
            alumno.mensualidad = stof(fields[3]);

            alumnos.push_back(alumno);
        }

        file.close();
        return alumnos;
    }

    void add(const Alumno &record)
    {
        ofstream file(this->file, ios::app);
        file << record.Nombre << "|" << record.Apellidos << "|" << record.Carrera << "|" << record.mensualidad << endl;
        file.close();
    }

    Alumno readRecord(int pos)
    {
        ifstream file(this->file);

        string line;
        getline(file, line);

        int current_pos = 0;
        while (getline(file, line))
        {
            if (current_pos == pos)
            {
                vector<string> fields = split(line, '|');
                Alumno alumno;
                alumno.Nombre = fields[0];
                alumno.Apellidos = fields[1];
                alumno.Carrera = fields[2];
                alumno.mensualidad = stof(fields[3]);

                file.close();
                return alumno;
            }
            ++current_pos;
        }

        file.close();
    }
};

int main()
{
    VariableRecord vr("data_p2.txt");

    // Agregar un nuevo estudiante
    Alumno newStudent = {"Sofia", "Salazar", "Ciencias", 1380};
    vr.add(newStudent);

    // Leer todos los registros, incluyendo el nuevo estudiante
    vector<Alumno> alumnos = vr.load();
    for (const Alumno &al : alumnos)
    {
        std::cout << al.Nombre << " " << al.Apellidos << " " << al.Carrera << " " << al.mensualidad << std::endl;
    }

    // Obtener el registro de la posición 10
    Alumno al = vr.readRecord(10);
    std::cout << al.Nombre << " " << al.Apellidos << " " << al.Carrera << " " << al.mensualidad << std::endl;

    return 0;
}
