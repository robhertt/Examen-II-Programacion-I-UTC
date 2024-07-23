#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <windows.h>
#include <conio.h>

using namespace std;

//constantes
const int MAX_CANDIDATOS = 20;
const int MAX_VOTANTES = 1000;

//estructira para almacer datos del candidato
struct Candidato {
    string nombre;
    string partido;
    string papeleta;
    int votos;
};

// estructura para almacenar datos del votante
struct Votante {
    int cedula;
    string nombre;
};

//Variables Globales
Candidato candidatos[MAX_CANDIDATOS];
Votante votantes[MAX_VOTANTES];
int votantesRegistrados[MAX_VOTANTES];
int numCandidatos = 0;
int numVotantes = 0;

// Funciones
void Menu();
void ingresarCandidatos();
void realizarVotacion();
void mostrarResultados();
void validarEntradaNumerica(int &entrada);
void limpiarEntrada();
void validarVotante(int& id);
void agregarVotante(const int& id);
void gotoxy(int x, int y);
void setColor(int color);

//Uso del Gotoxy y poder ubicar salidas en puntos especificos del programa
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//funcion de color de texto
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Menu priicipal
void Menu() {
    int opcion;
    do {
        setColor(2);
        gotoxy(17, 3); cout << "Sistema de Votaciones Electronico";
        setColor(15);
        gotoxy(17, 8); cout << "1. Ingresar Candidatos";
        gotoxy(17, 9); cout << "2. Votaciones";
        gotoxy(17, 10); cout << "3. Resultados";
        gotoxy(17, 11); cout << "4. Salir";
        setColor(2);
        gotoxy(17, 15); cout << "Seleccione una opcion: ";
        validarEntradaNumerica(opcion);
        system("cls");

        switch (opcion) {
            case 1:
                ingresarCandidatos();
                break;
            case 2:
                realizarVotacion();
                break;
            case 3:
                mostrarResultados();
                break;
            case 4:
                gotoxy(17, 17); cout << "Saliendo del sistema de votaciones.";
                break;
            default:
                setColor(12);
                gotoxy(17, 17); cout << "Opcion no valida. Intente de nuevo.";
        }
    } while (opcion != 4);
}

void ingresarCandidatos() {
    int num;
    setColor(2); 
    gotoxy(17, 3); cout << "Ingrese el numero de candidatos: ";
    validarEntradaNumerica(num);
    system("cls");

    for (int i = 0; i < num; ++i) {
        if (numCandidatos >= MAX_CANDIDATOS) {
            cout << "Se ha alcanzado el número máximo de candidatos.";
            break;
        }
        Candidato cand;
        setColor(2);  
        gotoxy(17, 3); cout << "INGRESO DE DATOS DE CANDIDATOS";
        setColor(15);
        gotoxy(17, 5); cout << "Ingresando datos del candidato # " << numCandidatos + 1 ;
        setColor(2);
        cin.ignore();
        gotoxy(17, 8); cout << "Nombre del candidato: "; setColor(15);      
        getline(cin, cand.nombre);
        setColor(2);
        gotoxy(17, 9); cout << "Partido politico: "; setColor(15);
        getline(cin, cand.partido);
        setColor(2);
        gotoxy(17, 10); cout << "Papeleta: "; setColor(15);
        getline(cin, cand.papeleta);
        cand.votos = 0;  // Inicializar votos a 0
        candidatos[numCandidatos++] = cand;
        system("cls");
    }
    setColor(15); 
    gotoxy(17, 15); cout << "Candidatos ingresados correctamente.\n";
    system("cls");
}

void realizarVotacion() {
    int voto;
    
    if (numCandidatos == 0) {
        setColor(12);
        gotoxy(17, 17); cout << "No hay candidatos registrados. Ingrese candidatos antes de votar.";
        return;
    }
    setColor(2);
    gotoxy(17, 3); cout << "INGRESE SU VOTO";
    setColor(15);
    Votante vot;
    gotoxy(17, 8); cout << "Ingrese su Numero de Cedula: ";
    cin >> vot.cedula;
    validarVotante(vot.cedula);
    setColor(15);
    cin.ignore(); // Limpiar el buffer antes de usar getline
    gotoxy(17, 9); cout << "Ingrese su nombre: ";
    getline(cin, vot.nombre);// aca uso el getline para almacenar toda la linea digitada, en caso que se digite un espacio con el apellido
    gotoxy(17, 11); cout << "Realizar Votacion";
    setColor(2);
    for (int i = 0; i < numCandidatos; ++i) {
        gotoxy(17, 13 + i); cout << i + 1 << ". " << candidatos[i].nombre << ", candidato por el partido: " << candidatos[i].partido;
    }
    setColor(15);
    gotoxy(17, 16 + numCandidatos + 1); cout << "Ingrese el numero del candidato por el cual desea votar: ";
    validarEntradaNumerica(voto);

    if (voto < 1 || voto > numCandidatos) {
        gotoxy(17, 16 + numCandidatos + 3); cout << "Voto no valido. Intente de nuevo.";
        system("cls");
        return;
    }
    candidatos[voto - 1].votos++;
    agregarVotante(vot.cedula);
    gotoxy(17, 17 + numCandidatos + 5); cout << "Voto registrado correctamente.";
    system("cls");
}

void mostrarResultados() {
    setColor(12);
    if (numCandidatos == 0) {
        gotoxy(17, 17); cout << "No es posible mostrar resultados ya que no se han registrado información de candidatos.";
        return;
    }

    int totalVotos = 0;
    for (int i = 0; i < numCandidatos; ++i) {
        totalVotos += candidatos[i].votos;
    }

    if (totalVotos == 0) {
        gotoxy(17, 17); cout << "No se han registrado votos.";
        return;
    }

    setColor(15);
    gotoxy(17, 3); cout << "RESULTADOS DE LA ELECCION";
    setColor(2);
    Candidato* ganador = &candidatos[0];
    for (int i = 0; i < numCandidatos; ++i) {
        double porcentaje = (static_cast<double>(candidatos[i].votos) / totalVotos) * 100;
        gotoxy(17, 8 + i); cout << "El candidato: " << candidatos[i].nombre << ", por el partido: [ " << candidatos[i].partido << " ] ha recibido " << candidatos[i].votos << " votos (" << fixed << setprecision(2) << porcentaje << "%)";
        if (candidatos[i].votos > ganador->votos) {
            ganador = &candidatos[i];
        }
    }
    setColor(15);
    gotoxy(17, 10 + numCandidatos); cout << "El ganador de la eleccion es: " << ganador->nombre << " (" << ganador->partido << ")";
    getch();
    system("cls");
}

void validarEntradaNumerica(int &entrada) {
    setColor(15);
    while (!(cin >> entrada)) {
        system("cls");
        setColor(12);
        gotoxy(17, 17); cout << "Acaba ingresar una letra o simbolo no soportado por el MENU. Por favor ingrese un numero.";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        Menu(); 
    }  
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void validarVotante(int& id) {	
    bool idValida = false;
    while (!idValida) {	
        idValida = true; // Assume ID is valid initially
        for (int i = 0; i < numVotantes; ++i) {
            if (votantesRegistrados[i] == id) {
                setColor(12);
                gotoxy(17, 18); cout << "Usted ya ha votado. Ingrese una cedula valida: ";
                cin >> id;
                idValida = false; // ID is not  
                }
		}
	}
}

void agregarVotante(const int& id) {
    if (numVotantes >= MAX_VOTANTES) {
        gotoxy(17, 3); cout << "Se ha alcanzado el número máximo de votantes.";
        return;
    }
    votantesRegistrados[numVotantes++] = id;
}

int main() {
    Menu();
    gotoxy(17, 18); return 0;
}
