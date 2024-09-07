#include <iostream>
#include <string> 
using namespace std;


class Process {
public:
    int id;
    int at;   // Arrival Time
    int bt;   // Burst Time
    int prio; // Priority

    int wt;   // Waiting Time
    int tat;  // Turnaround Time
    int ct;   //f Completion Time
    int rt;   // Response Time
    bool flag;

    // Constructor por defecto
    Process()
        : id(0), at(0), bt(0), prio(0), wt(0), tat(0), ct(0), rt(0), flag(true) {}

    // Constructor parametrizado
    Process(int id, int at, int bt, int prio, int tat = 0, int wt = 0, int ct = 0, int lwt = 0, int rt = 0, bool flag = true)
        : id(id), at(at), bt(bt), prio(prio), tat(tat), wt(wt), ct(ct), rt(rt), flag(flag) {}

};
