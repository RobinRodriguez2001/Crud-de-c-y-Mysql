#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;

void menu(MYSQL* conector);
void crear(MYSQL* conector);
void mostrar(MYSQL* conector);
void modificar(MYSQL* conector);
void eliminar(MYSQL* conector);

int main() {
    MYSQL* conector;
    conector = mysql_init(0);
    conector = mysql_real_connect(conector, "localhost", "root", "danigero", "estudents", 3306, NULL, 0);

    if (conector) {
        cout << "\t----------MENU----------\n\n";
        menu(conector);
    }
    else {
        cout << "Error al conectar la base de datos" << endl;
        cout << "\nIntentelo de nuevo mas tarde..." << endl;
    }

    mysql_close(conector);
    return 0;
}

void menu(MYSQL* conector) {
    int opc;

    while (true) {
        cout << "\n\t1. Crear Registro";
        cout << "\n\t2. Mostrar Registros";
        cout << "\n\t3. Modificar Registro";
        cout << "\n\t4. Eliminar Registro";
        cout << "\n\t5. Salir";
        cout << "\n\n\tSeleccione una opcion: ";
        cin >> opc;

        switch (opc) {
        case 1:
            system("cls");
            crear(conector);
            system("pause");
            system("cls");
            break;
        case 2:
            system("cls");
            mostrar(conector);
            system("pause");
            system("cls");
            break;
        case 3:
            system("cls");
            modificar(conector);
            system("pause");
            system("cls");
            break;
        case 4:
            system("cls");
            eliminar(conector);
            system("pause");
            system("cls");
            break;
        case 5:
            exit(0);
        default:
            system("cls");
            cout << "Opción no válida, intente de nuevo." << endl;
            system("pause");
            system("cls");
        }
    }
}

void crear(MYSQL* conector) {
    int codigo;
    string nombre, apellido, direccion, telefono, fecha_nacimiento, tipo_sangre;

    cout << "Digite el codigo de Estudiante: ";
    cin >> codigo;
    cout << "Digite el nombre: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Digite el apellido: ";
    getline(cin, apellido);
    cout << "Digite la direccion: ";
    getline(cin, direccion);
    cout << "Digite el telefono: ";
    getline(cin, telefono);
    cout << "Digite la fecha de nacimiento (YYYY-MM-DD): ";
    getline(cin, fecha_nacimiento);
    cout << "Digite el tipo de sangre: ";
    cin >> tipo_sangre;

    // Verificar si el tipo de sangre ya existe
    string check_tipo_sangre = "SELECT id_tipo_sangre FROM tipos_sangre WHERE tipo_sangre = '" + tipo_sangre + "'";
    const char* q_check_tipo_sangre = check_tipo_sangre.c_str();
    int qstate_check = mysql_query(conector, q_check_tipo_sangre);

    int id_tipo_sangre = 0;
    MYSQL_RES* res = mysql_store_result(conector);
    if (mysql_num_rows(res) > 0) {
        MYSQL_ROW row = mysql_fetch_row(res);
        id_tipo_sangre = stoi(row[0]);
    }
    else {
        // Insertar nuevo tipo de sangre
        string insert_tipo_sangre = "INSERT INTO tipos_sangre (tipo_sangre) VALUES ('" + tipo_sangre + "')";
        const char* q_insert_tipo_sangre = insert_tipo_sangre.c_str();
        mysql_query(conector, q_insert_tipo_sangre);
        id_tipo_sangre = mysql_insert_id(conector);
    }
    mysql_free_result(res);

    // Insertar estudiante con id_tipo_sangre
    string insert = "INSERT INTO estudiantes (codigo, nombres, apellidos, direccion, telefono, fecha_nacimiento, id_tipo_sangre) VALUES (" +
        to_string(codigo) + ", '" + nombre + "', '" + apellido + "', '" + direccion + "', '" + telefono + "', '" + fecha_nacimiento + "', " + to_string(id_tipo_sangre) + ")";
    const char* q = insert.c_str();
    int qstate = mysql_query(conector, q);

    if (!qstate) {
        cout << "Ingreso exitoso..." << endl;
    }
    else {
        cout << "Error al ingresar: " << mysql_error(conector) << endl;
    }
}

void mostrar(MYSQL* conector) {
    MYSQL_ROW row;
    MYSQL_RES* res;

    int qstate = mysql_query(conector, "SELECT e.codigo, e.nombres, e.apellidos, e.direccion, e.telefono, e.fecha_nacimiento, t.tipo_sangre FROM estudiantes e JOIN tipos_sangre t ON e.id_tipo_sangre = t.id_tipo_sangre");
    if (!qstate) {
        res = mysql_store_result(conector);
        cout << "Listado de Estudiantes:\n\n";
        while ((row = mysql_fetch_row(res))) {
            cout << " Codigo: " << row[0] << "\n";
            cout << " Nombre: " << row[1] << "\n";
            cout << " Apellido: " << row[2] << "\n";
            cout << " Dirección: " << row[3] << "\n";
            cout << " Telefono: " << row[4] << "\n";
            cout << " Fecha Nacimiento: " << row[5] << "\n";
            cout << " Tipo de sangre: " << row[6] << "\n\n";
        }
    }
    else {
        cout << "Error al mostrar los registros: " << mysql_error(conector) << endl;
    }
}

void modificar(MYSQL* conector) {
    int id_estudiante;
    string campo, nuevo_valor;

    cout << "Digite el ID del estudiante a modificar: ";
    cin >> id_estudiante;
    cout << "Digite el nombre del campo a modificar "<<endl;
    cout << "(nombres, apellidos, direccion, telefono, fecha_nacimiento, id_tipo_sangre) : ";
    cin >> campo;
    cout << "Digite el nuevo valor: ";
    cin.ignore();
    getline(cin, nuevo_valor);

    string update = "UPDATE estudiantes SET " + campo + " = '" + nuevo_valor + "' WHERE id_estudiante = " + to_string(id_estudiante);
    const char* q = update.c_str();
    int qstate = mysql_query(conector, q);

    if (!qstate) {
        cout << "Modificacion exitosa..." << endl;
    }
    else {
        cout << "Error al modificar: " << mysql_error(conector) << endl;
    }
}

void eliminar(MYSQL* conector) {
    int codigo;

    cout << "Digite el ID del estudiante a eliminar: ";
    cin >> codigo;

    string del = "DELETE FROM estudiantes WHERE codigo = " + to_string(codigo);
    const char* q = del.c_str();
    int qstate = mysql_query(conector, q);

    if (!qstate) {
        cout << "Eliminacion exitosa..." << endl;
    }
    else {
        cout << "Error al eliminar: " << mysql_error(conector) << endl;
    }
}