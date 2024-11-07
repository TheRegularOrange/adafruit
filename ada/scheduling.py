class Process:
    def __init__(self, pid, burst_time, arrival_time, priority=0):
        self.pid = pid
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.arrival_time = arrival_time
        self.priority = priority
        self.completion_time = 0

def fcfs(processes):
    processes.sort(key=lambda x: x.arrival_time)
    time = 0
    for process in processes:
        if time < process.arrival_time:
            time = process.arrival_time
        time += process.burst_time
        process.completion_time = time
    return processes

def sjf_preemptive(processes):
    time = 0
    completed = 0
    n = len(processes)
    processes.sort(key=lambda x: x.arrival_time)
    while completed < n:
        ready_queue = [p for p in processes if p.arrival_time <= time and p.remaining_time > 0]
        if ready_queue:
            current_process = min(ready_queue, key=lambda x: x.remaining_time)
            current_process.remaining_time -= 1
            time += 1
            if current_process.remaining_time == 0:
                current_process.completion_time = time
                completed += 1
        else:
            time += 1
    return processes

def priority_non_preemptive(processes):
    processes.sort(key=lambda x: (x.arrival_time, x.priority))
    time = 0
    completed = 0
    n = len(processes)

    while completed < n:
        ready_queue = [p for p in processes if p.arrival_time <= time and p.burst_time > 0]
        if ready_queue:
            current_process = min(ready_queue, key=lambda x: x.priority)
            time += current_process.burst_time
            current_process.completion_time = time
            current_process.burst_time = 0
            completed += 1
        else:
            time += 1
    return processes

def round_robin(processes, time_quantum):
    queue = []
    time = 0
    completed = 0
    n = len(processes)
    
    processes.sort(key=lambda x: x.arrival_time)
    while completed < n:
        for process in processes:
            if process.arrival_time <= time and process.remaining_time >= 0:
                queue.append(process)
        
        if queue:
            current_process = queue.pop(0)
            if current_process.remaining_time > time_quantum:
                time += time_quantum
                current_process.remaining_time -= time_quantum
                queue.append(current_process)
            else:
                time += current_process.remaining_time
                current_process.completion_time = time
                current_process.remaining_time = 0
                completed += 1
        else:
            time += 1
            
    return processes

def print_processes(processes):
    print("PID\tBurst Time\tArrival Time\tCompletion Time")
    for p in processes:
        print(f"{p.pid}\t{p.burst_time}\t\t{p.arrival_time}\t\t{p.completion_time}")

if __name__ == "__main__":
    processes = [
        Process(pid=1, burst_time=8, arrival_time=0, priority=1),
        Process(pid=2, burst_time=4, arrival_time=1, priority=2),
        Process(pid=3, burst_time=9, arrival_time=2, priority=3),
        Process(pid=4, burst_time=5, arrival_time=3, priority=0),
    ]

    print("FCFS Scheduling:")
    fcfs_processes = fcfs(processes.copy())
    print_processes(fcfs_processes)

    print("\nSJF (Preemptive) Scheduling:")
    sjf_processes = sjf_preemptive(processes.copy())
    print_processes(sjf_processes)

    print("\nPriority (Non-Preemptive) Scheduling:")
    priority_processes = priority_non_preemptive(processes.copy())
    print_processes(priority_processes)

    print("\nRound Robin Scheduling (Time Quantum = 3):")
    rr_processes = round_robin(processes.copy(),3)
    print_processes(rr_processes)
