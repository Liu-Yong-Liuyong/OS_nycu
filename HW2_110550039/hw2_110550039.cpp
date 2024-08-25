#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int wait_time;
    int turnaround_time;
    int enter_qt;
    bool runRR;

};

struct Compare_Arrival_RemainingTime {
    bool operator()(const Process& a, const Process& b) const {
        if(a.remaining_time > b.remaining_time){
            return true;
        }else if (a.remaining_time == b.remaining_time){
            if(a.arrival_time>b.arrival_time){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
};
struct Compare_Arrival_RemainingTimeRR {
    bool operator()(const Process& a, const Process& b) const {
        if(a.enter_qt > b.enter_qt){
            return true;
        }else if(a.enter_qt == b.enter_qt){
            return a.arrival_time<b.arrival_time;
        }else{
            return false;
        }

    }
};

void FCFS(std::vector<Process>& processes) {
    int current_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    for (Process& process : processes) {
        if (current_time < process.arrival_time) {
            current_time = process.arrival_time;
        }

        process.wait_time = current_time - process.arrival_time;
        process.turnaround_time = process.wait_time + process.burst_time;

        total_wait_time += process.wait_time;
        total_turnaround_time += process.turnaround_time;

        current_time += process.burst_time;
    }

    // Print results
    for (const Process& process : processes) {
        std::cout << process.wait_time << " " << process.turnaround_time << std::endl;
    }

    std::cout << total_wait_time << std::endl;
    std::cout << total_turnaround_time << std::endl;
}

void SRTF(std::vector<Process>& processes) {
    int n = processes.size();
    int current_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    std::priority_queue<Process, std::vector<Process>,Compare_Arrival_RemainingTime> pq;
    

    int completed = 0;
    bool pushed[n];
    for(int i=0;i<n;i++){
        pushed[i]=false;
    }
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && pushed[i]==false) {
                pq.push(processes[i]);
                pushed[i]=true;//do not push twice
            }
        }

        if (!pq.empty()) {
            Process shortest = pq.top();
            pq.pop();
            //std::cout << "id: " << shortest.id << std::endl;
            //std::cout << "Remaining Time: " << shortest.remaining_time << std::endl;
            shortest.remaining_time--;
            //std::cout << "current_time: " << current_time << std::endl;

            current_time++;
            
            for (int i = 0; i < n; i++) {
                if (processes[i].id == shortest.id) {
                    //std::cout << "process_now_id: " << shortest.id << std::endl;
                    processes[i].remaining_time--;
                }
            }

            if (shortest.remaining_time == 0) {
                //std::cout << "complete_id: " << shortest.id << std::endl;
                completed++;
        
                processes[shortest.id].wait_time = current_time - shortest.arrival_time - shortest.burst_time;
                processes[shortest.id].turnaround_time = current_time - shortest.arrival_time;
               
                total_wait_time +=processes[shortest.id].wait_time;
                total_turnaround_time +=  processes[shortest.id].turnaround_time;
            } else {
                //std::cout << "-----" << std::endl;
                pq.push(shortest);
              
            }
        } else {
            //std::cout << "+++++" << std::endl;
            current_time++;
            
        }
    }

    // Print results
    for (const Process& process : processes) {
        std::cout << process.wait_time << " " << process.turnaround_time << std::endl;
    }

    std::cout << total_wait_time << std::endl;
    std::cout << total_turnaround_time << std::endl;
}

void RR(std::vector<Process>& processes, int time_quantum) {
    int n = processes.size();
    int current_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    //std::queue<Process> q;
    std::priority_queue<Process, std::vector<Process>,Compare_Arrival_RemainingTimeRR> q;

    int completed = 0;
    int index = 0;

    while (completed < n) {
        while (index < n && processes[index].arrival_time <= current_time ) {
            q.push(processes[index]);
            index++;
        }
        if (q.empty()) {
            current_time++;
        } else {
            Process front = q.top();
            q.pop();
            //front.runRR = true;
            int slice = std::min(time_quantum, front.remaining_time);
            front.remaining_time -= slice;
            current_time += slice;

            for (int i = 0; i < n; i++) {
                if (processes[i].id == front.id) {
                    processes[i].remaining_time = front.remaining_time;
                }
            }

            if (front.remaining_time == 0) {
                completed++;
                processes[front.id].wait_time = current_time - front.arrival_time - front.burst_time;
                processes[front.id].turnaround_time = current_time - front.arrival_time;
                total_wait_time += processes[front.id].wait_time;
                total_turnaround_time += processes[front.id].turnaround_time;
            } else {
                front.enter_qt = current_time;
                q.push(front);
            }
        }
    }

    // Print results
    for (const Process& process : processes) {
        std::cout << process.wait_time << " " << process.turnaround_time << std::endl;
    }

    std::cout << total_wait_time << std::endl;
    std::cout << total_turnaround_time << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;

    std::vector<Process> processes(M);

    int mode[N]; 
    int time_quantum[N];
    

    for(int i=0 ; i<N ; i++){
        std::cin>>mode[i];
        std::cin>>time_quantum[i];
    }
    

    for (int i = 0; i < M; i++) {
        processes[i].id = i;
        std::cin >> processes[i].arrival_time >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].enter_qt=processes[i].arrival_time;
        processes[i].runRR=false;
    }
    if(N==1){
        if (mode[0] == 0) {
            FCFS(processes);
        } else if (mode[0] == 1) {
            SRTF(processes);
        } else {
            //std::cin >> time_quantum;
            RR(processes, time_quantum[0]);
        } 
    }
    
    if(N==2){
        int n = processes.size();
        //int current_time = 0;
        int total_wait_time = 0;
        int total_turnaround_time = 0;
        int current_time_rr = 0;
        //int current_time_srtf = 0;

        std::priority_queue<Process, std::vector<Process>,Compare_Arrival_RemainingTimeRR> q;
        std::priority_queue<Process, std::vector<Process>,Compare_Arrival_RemainingTime> pq;

        int completed = 0;
        int index = 0;

        while (completed < n) {
            while (index < n && processes[index].arrival_time <= current_time_rr ) {
                q.push(processes[index]);
                index++;
            }
            if (q.empty()) {
                if (!pq.empty()) {
                    Process shortest = pq.top();
                    pq.pop();
                    //std::cout << "id: " << shortest.id << std::endl;
                    //std::cout << "Remaining Time: " << shortest.remaining_time << std::endl;
                    shortest.remaining_time--;
                    //std::cout << "current_time: " << current_time_rr << std::endl;

                    current_time_rr++;
                    
                    for (int i = 0; i < n; i++) {
                        if (processes[i].id == shortest.id) {
                            //std::cout << "process_now_id: " << shortest.id << std::endl;
                            processes[i].remaining_time--;
                        }
                    }

                    if (shortest.remaining_time == 0) {
                        //std::cout << "complete_id: " << shortest.id << std::endl;
                        completed++;
                
                        processes[shortest.id].wait_time = current_time_rr - shortest.arrival_time - shortest.burst_time;
                        processes[shortest.id].turnaround_time = current_time_rr - shortest.arrival_time;
                    
                        total_wait_time +=processes[shortest.id].wait_time;
                        total_turnaround_time +=  processes[shortest.id].turnaround_time;
                    } else {
                        //std::cout << "-----" << std::endl;
                        pq.push(shortest);
                    
                    }
                } else {
                    //std::cout << "+++++" << std::endl;
                    current_time_rr ++;
                    
                }
                //current_time_rr ++;
            } else {
                Process front = q.top();
                q.pop();
                //front.runRR = true;
                //std::cout<<"RR: "<<front.id<<std::endl;
                int slice = std::min(time_quantum[0], front.remaining_time);
                front.remaining_time -= slice;
                current_time_rr += slice;

                for (int i = 0; i < n; i++) {
                    if (processes[i].id == front.id) {
                        processes[i].remaining_time = front.remaining_time;
                    }
                }

                if (front.remaining_time == 0) {
                    completed++;
                    processes[front.id].wait_time = current_time_rr - front.arrival_time - front.burst_time;
                    processes[front.id].turnaround_time = current_time_rr - front.arrival_time;
                    total_wait_time += processes[front.id].wait_time;
                    total_turnaround_time += processes[front.id].turnaround_time;
                    //current_time_srtf = current_time_rr;
                } else {
                    front.enter_qt = current_time_rr;
                    pq.push(front);
                }
            }
        }
        // Print results
        for (const Process& process : processes) {
            std::cout << process.wait_time << " " << process.turnaround_time << std::endl;
        }
        std::cout << total_wait_time << std::endl;
        std::cout << total_turnaround_time << std::endl;
    }


    

    
    

    

    return 0;
}




