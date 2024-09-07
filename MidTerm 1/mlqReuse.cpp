#include <iostream>
#include <vector>
#include <queue>
#include "cola.cpp"

// Función para imprimir los elementos de una cola
void imprimirCola1(Cola c, const std::string& nombre) {
    cout << "Contenido de la cola de " << nombre << ":\n";

    // Verifica si la cola está vacía
    if (c.cola.empty()) {
        cout << "Cola vacía.\n";
    } else {
        cout << "Procesos en la cola:\n";
        std::queue<Process*> temp = c.cola;  
        // Itera y muestra los elementos de la cola
        while (!temp.empty()) {
            Process* p = temp.front();
            temp.pop();
            cout <<"ID: " << p->id << ", AT: "<< p->at << ", CT: " << p->ct << ", RT: " << p->rt << ", TAT: " << p->tat << '\n';
        }
    }
}

// Otra función para imprimir los elementos de una cola (incluye tiempo de espera)
void imprimirCola2(Cola c, const std::string& nombre) {
    cout << "Contenido de la cola de " << nombre << ":\n";

    // Verifica si la cola está vacía
    if (c.cola.empty()) {
        cout << "Cola vacía.\n";
    } else {
        cout << "Procesos en la cola:\n";
        std::queue<Process*> temp = c.cola;  
        // Itera y muestra los elementos de la cola (incluye tiempo de espera)
        while (!temp.empty()) {
            Process* p = temp.front();
            temp.pop();
            cout <<"ID: " << p->id << ", AT: "<< p->at <<", WT: " << p->wt << ", CT: " << p->ct << ", RT: " << p->rt << ", TAT: " << p->tat << '\n';
        }
    }
}

// Clase MLQ (Multi-Level Queue)
class MLQ {
    public:
        vector<Cola> queues;   // Vector de colas
        int currentTime;
        Process* currentProcess;
        int quantum;
        int remainingQuantum;

        // Constructor que inicializa los atributos
        MLQ(int quantum) : currentTime(0), currentProcess(nullptr), quantum(quantum), remainingQuantum(quantum), queues(queues){}
        
        // Agrega una cola al vector de colas
        void addCola(Cola queue) {
            queues.push_back(queue);
        }

        // Método para manejar la cola en FCFS (First Come, First Served)
        void stepFCFS(int i) {
            // Si no hay un proceso actual, toma el primer proceso de la cola
            if (currentProcess == nullptr && !queues[i].cola.empty()) {
                currentProcess = queues[i].cola.front();
                queues[i].cola.pop(); 
                // Si el proceso es nuevo, actualiza su tiempo de respuesta y espera
                if (currentProcess->flag) {
                    currentProcess->rt = currentTime;
                    currentProcess->wt = currentTime - currentProcess->at;
                    currentProcess->flag = false;
                }
            }
            // Si hay un proceso actual, disminuye su tiempo restante
            if (currentProcess != nullptr) {
                currentProcess->bt -= 1;
                currentTime += 1;
                // Si el proceso ha terminado, actualiza sus tiempos finales y lo elimina
                if (currentProcess->bt == 0) {
                    currentProcess->ct = currentTime;
                    currentProcess->tat = currentProcess->ct - currentProcess->at;
                    currentProcess = nullptr;
                }
            }
        }

        // Método para manejar la cola en RR (Round Robin)
        void stepRR(int i) {
            // Si no hay un proceso actual, toma el primer proceso de la cola
            if (currentProcess == nullptr && !queues[i].cola.empty()) {
                currentProcess = queues[i].cola.front();
                queues[i].cola.pop();
                // Si el proceso es nuevo, actualiza su tiempo de respuesta y espera
                if (currentProcess->flag) {
                    currentProcess->wt = currentTime - currentProcess->at;
                    currentProcess->rt = currentTime;
                    currentProcess->flag = false;
                }
            }
            // Si hay un proceso actual, disminuye su tiempo restante y el quantum
            if (currentProcess != nullptr) {
                currentProcess->bt -= 1;
                remainingQuantum -= 1;
                currentTime += 1;

                // Si el proceso ha terminado, actualiza sus tiempos finales y lo elimina
                if (currentProcess->bt == 0) {
                    currentProcess->ct = currentTime;
                    currentProcess->tat = currentProcess->ct - currentProcess->at;
                    currentProcess = nullptr;
                    remainingQuantum = quantum;
                // Si el quantum ha terminado, reinsertar el proceso en la cola
                } else if (remainingQuantum == 0) {
                    queues[i].cola.push(currentProcess);
                    if(queues[i].cola.front() != currentProcess){
                        currentProcess->wt += currentTime - currentProcess->rt - (quantum - remainingQuantum);
                    }
                    currentProcess = nullptr;
                    remainingQuantum = quantum;
                }
            }
        }

        // Método para manejar el MLQ
        void iMLQ() {
            while (!queues[0].cola.empty() || !queues[1].cola.empty() || !queues[2].cola.empty() || currentProcess != nullptr) {
                // Procesar las colas según su algoritmo
                if (!queues[0].cola.empty() || currentProcess != nullptr) {
                    if(queues[0].algoritmo.tipo == "RR"){
                        stepRR(0);
                    } else {
                        stepFCFS(0);
                    }
                } else if (queues[0].cola.empty() && (!queues[1].cola.empty() || currentProcess != nullptr)) {
                    if(queues[1].algoritmo.tipo == "RR")
                        stepRR(1);
                    else 
                        stepFCFS(1);
                } else if ((queues[0].cola.empty() && queues[1].cola.empty() && (!queues[2].cola.empty() || currentProcess != nullptr)))
                    if(queues[2].algoritmo.tipo == "RR")
                        stepRR(2);
                    else 
                        stepFCFS(2);
            }
        }
};

// Clase MLFQ (Multi-Level Feedback Queue)
class MLFQ {
    public:
        vector<Cola> queues;   
        int currentTime;
        Process* currentProcess;
        int quantum;
        int remainingQuantum;

        // Constructor que inicializa los atributos
        MLFQ(int numColas, int quantum) : currentTime(0), currentProcess(nullptr), quantum(quantum), remainingQuantum(quantum), queues(queues) {}

        // Agrega una cola al vector de colas
        void addCola(Cola queue) {
            queues.push_back(queue);
        }

        // Método para manejar la cola en FCFS (First Come, First Served)
        void stepFCFS2() {
            // Si no hay un proceso actual, toma el primer proceso de la cola
            if (currentProcess == nullptr && !queues[2].cola.empty()) {
                currentProcess = queues[2].cola.front();
                queues[2].cola.pop(); 
                // Si el proceso es nuevo, actualiza su tiempo de respuesta y espera
                if (currentProcess->flag) {
                    currentProcess->rt = currentTime;
                    currentProcess->wt = currentTime - currentProcess->at;
                    currentProcess->flag = false;
                }
            }

            // Si hay un proceso actual, disminuye su tiempo restante
            if (currentProcess != nullptr) {
                currentProcess->bt -= 1;
                currentTime += 1;
                // Si el proceso ha terminado, actualiza sus tiempos finales y lo elimina
                if (currentProcess->bt == 0) {
                    currentProcess->ct = currentTime;
                    currentProcess->tat = currentProcess->ct - currentProcess->at;
                    currentProcess = nullptr;
                }
            }
        }

        // Método para manejar la cola en RR (Round Robin)
        void stepRR2(int i) {
            // Si no hay un proceso actual, toma el primer proceso de la cola
            if (currentProcess == nullptr && !queues[i].cola.empty()) {
                currentProcess = queues[i].cola.front();
                queues[i].cola.pop();
                // Si el proceso es nuevo, actualiza su tiempo de respuesta y espera
                if (currentProcess->flag) {
                    currentProcess->wt = currentTime - currentProcess->at;
                    currentProcess->rt = currentTime;
                    currentProcess->flag = false;
                }
            }

            // Si hay un proceso actual, disminuye su tiempo restante y el quantum
            if (currentProcess != nullptr) {
                currentProcess->bt -= 1;
                remainingQuantum -= 1;
                currentTime += 1;

                // Si el proceso ha terminado, actualiza sus tiempos finales y lo elimina
                if (currentProcess->bt == 0) {
                    currentProcess->ct = currentTime;
                    currentProcess->tat = currentProcess->ct - currentProcess->at;
                    currentProcess = nullptr;
                    remainingQuantum = quantum;
                // Si el quantum ha terminado, reinsertar el proceso en la cola
                } else if (remainingQuantum == 0) {
                    if (queues[i].cola == queues[0].cola){
                        queues[1].cola.push(currentProcess);
                        if(queues[1].cola.front() != currentProcess){
                            currentProcess->wt += currentTime - currentProcess->rt - (quantum - remainingQuantum);
                        }
                        currentProcess = nullptr;
                        remainingQuantum = quantum;
                    } else if (queues[i].cola == queues[1].cola){
                        queues[2].cola.push(currentProcess);
                        if(queues[2].cola.front() != currentProcess){
                            currentProcess->wt += currentTime - currentProcess->rt - (quantum - remainingQuantum);
                        }
                        currentProcess = nullptr;
                        remainingQuantum = quantum;  
                    }
                }
            }
        }

        // Método para manejar el MLFQ
        void iMLFQ() {
            while (!queues[0].cola.empty() || !queues[1].cola.empty() || !queues[2].cola.empty() || currentProcess != nullptr) {
                // Procesar las colas según su algoritmo
                if (!queues[0].cola.empty() || currentProcess != nullptr) {
                    stepRR2(0);
                } else if (queues[0].cola.empty() && (!queues[1].cola.empty() || currentProcess != nullptr)) {
                    stepRR2(1);
                } else if (queues[0].cola.empty() && queues[1].cola.empty() && (!queues[2].cola.empty() || currentProcess != nullptr)){
                    stepFCFS2();
                }
            }
        }
};

int main() {
    // Definición de colas
    Cola A1, B2, C3;
    Cola A11, B22, C33;

    int n;
    string type;
    
    // Asignación de tipo de algoritmo a cada cola
    for(int i = 0; i < 3; i++) {
        cin >> type;
        if(i == 0)
            A1.algoritmo.tipo = type;
        if(i == 1)
            B2.algoritmo.tipo = type;
        if(i == 2)
            C3.algoritmo.tipo = type;
    }

    // Lectura del número de procesos
    cin >> n;
    cin.ignore();
    
    int id, at, bt, q;
    
    // Lectura de procesos y asignación a colas según su prioridad
    for(int j = 0; j < n; j++) {
        cin >> id >> at >> bt >> q;
        Process* proceso = new Process(id, at, bt, q);
        Process* proceso1 = new Process(id, at, bt, q);

        A11.cola.push(proceso1);
        if(q == 1){   
            A1.cola.push(proceso);
        }
        if(q == 2){
            B2.cola.push(proceso);
        }
        if(q == 3){
            C3.cola.push(proceso);
        }
    }

    // Creación de los planificadores
    MLQ scheduler(4);
    MLFQ scheduler2(3, 3);

    // Asignación de colas a los planificadores
    scheduler.addCola(A1);
    scheduler.addCola(B2);
    scheduler.addCola(C3);

    scheduler2.addCola(A11);
    scheduler2.addCola(B22);
    scheduler2.addCola(C33);

    // Ejecución de los planificadores
    scheduler.iMLQ();
    scheduler2.iMLFQ();

    // Impresión de resultados
    cout << "MLQ: " << endl;
    imprimirCola2(A1, "A1");
    imprimirCola2(B2, "B2");
    imprimirCola2(C3, "C3");

    cout << "MLFQ: " << endl;
    imprimirCola1(A11, "Resultados Procesos: ");
}
