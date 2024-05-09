#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

#define ll long long int

struct Process {
    ll id;
    ll arrivalTime;
    ll burstTime;
    ll endTime = 0;
    ll TAT = 0; // Turnaround time
    ll waitingTime = 0;
    ll originalBurstTime;
};

void readFile(const string& filename, vector<Process>& processes) {
    ifstream file(filename);
    if (!file) {
        cout << "Unable to open file\n";
        exit(1);
    }

    ll n, arrival, burst;
    file >> n;
    for (int i = 0; i < n; i++) {
        file >> arrival >> burst;
        processes.push_back({ i + 1, arrival, burst, 0, 0, 0, burst });
    }
    file.close();
}

void FCFS(vector<Process>& processes, ll contextSwitch) {
    ll currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        p.waitingTime = currentTime - p.arrivalTime;
        currentTime += p.burstTime + contextSwitch;
        p.endTime = currentTime - contextSwitch;
        p.TAT = p.endTime - p.arrivalTime;
    }
}

void SRT(vector<Process>& processes, ll contextSwitch) {
    auto comp = [](Process* a, Process* b) { return a->burstTime > b->burstTime; };//sorting function, lamda function
    priority_queue<Process*, vector<Process*>, decltype(comp)> pq(comp);//max_heap,the root is maximum element

    ll currentTime = 0, idx = 0;
    while (!pq.empty() || idx < processes.size()) {
        while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
            pq.push(&processes[idx++]);
        }

        if (!pq.empty()) {
            Process* current = pq.top();
            pq.pop();
            currentTime += 1; // Execute for one unit of time
            current->burstTime--;

            // Re-check for new arrivals before continuing with the current process
            while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
                pq.push(&processes[idx++]);
            }

            if (current->burstTime > 0) {
                pq.push(current);
            }
            else {
                current->endTime = currentTime;
                current->TAT = currentTime - current->arrivalTime;
                current->waitingTime = current->TAT - current->originalBurstTime;
            }

            currentTime += contextSwitch; // Account for context switch
        }
        else {
            if (idx < processes.size()) {
                currentTime = processes[idx].arrivalTime;
            }
        }
    }
}

void RR(vector<Process>& processes, ll quantum, ll contextSwitch) {
    queue<Process*> q;
    ll currentTime = 0, idx = 0;
    while (!q.empty() || idx < processes.size()) {
        while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
            q.push(&processes[idx++]);
        }

        if (!q.empty()) {
            Process* current = q.front();
            q.pop();

            ll timeSlice = min(quantum, current->burstTime);// 4 .... 3 
            current->burstTime -= timeSlice;
            currentTime += timeSlice;

            while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
                q.push(&processes[idx++]);
            }

            if (current->burstTime > 0) {
                q.push(current);
            }
            else {
                current->endTime = currentTime;
                current->TAT = currentTime - current->arrivalTime;
                current->waitingTime = current->TAT - current->originalBurstTime;
            }

            currentTime += contextSwitch; // Account for context switch
        }
        else {
            if (idx < processes.size()) {
                currentTime = processes[idx].arrivalTime;
            }
        }
    }
}

void printResults(const vector<Process>& processes) {
    double totalTAT = 0, totalWaiting = 0;
    cout << "Process\tArrival\tBurst\tEnd\tWaiting\tTAT\n";
    for (const auto& p : processes) {
        cout << p.id << "\t" << p.arrivalTime << "\t" << p.originalBurstTime << "\t"
            << p.endTime << "\t" << p.waitingTime << "\t" << p.TAT << endl;
        totalTAT += p.TAT;
        totalWaiting += p.waitingTime;
    }

    cout << "Average Waiting Time: " << totalWaiting / processes.size() << endl;
    cout << "Average Turnaround Time: " << totalTAT / processes.size() << endl;
}

int main() {
    string filename = "processes.txt"; // File should be in the format: number_of_processes followed by each process's arrival_time and burst_time
    vector<Process> processes;
    readFile(filename, processes);

    ll contextSwitch = 2; // Context switch time
    ll quantum = 3; // Quantum time for RR
    ll choise;
        cout << "Select your Choice:\n0.FCFS\n1.SRT\n2.RR\n3.Exit" << endl;
        cin >> choise;
        switch (choise) {
        case 0: {
            cout << "This is the FSCS algorithm for your values:" << endl;
            FCFS(processes, contextSwitch);
            printResults(processes);
            break;
        }
        case 1: {
            cout << "This is the SRT algorithm for your values:" << endl;
            SRT(processes, contextSwitch);
            printResults(processes);
            break;
        }
        case 2: {
            cout << "This is the FSCS algorithm for your values:" << endl;
            RR(processes, quantum, contextSwitch);
            printResults(processes);
            break;
        }
        case 3: {
            cout << "BYE BYE" << endl;
        }
        default: {
            cout << "--------------------------------------------------------" << endl;
            cout << "Invalid Choise" << endl;
            break;
        }
        }
    
    return 0;
    }