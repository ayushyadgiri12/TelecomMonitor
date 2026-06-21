# Smart Telecom Network Monitoring & Service Optimization System

**B.Tech CSE 2025-29 | Data Structures & Algorithms with C++ | Semester II**
**ITM Skills University | Case Study 147**

---

## 1. Project Title
Smart Telecom Network Monitoring & Service Optimization System

## 2. Problem Statement
Telecommunication providers require a platform to monitor network performance, manage infrastructure, detect faults, and optimize communication services. The system must:
- Prioritize network incidents (sorting algorithms).
- Retrieve subscriber and infrastructure data quickly (searching algorithms).
- Manage configuration rollback operations (stack).
- Process service requests and fault reports (queue).
- Store telecom databases using BST & AVL trees.
- Analyze network connectivity using BFS, DFS & Dijkstra's algorithm.
- Enable quick subscriber verification using hashing.

The solution is implemented in C++, with each design choice justified and related to real-world telecom systems.

## 3. Objectives
- Rank network incidents by severity so the most critical faults are addressed first (Incident Manager - merge sort).
- Provide fast retrieval of subscriber and device records (Search Engine - binary search).
- Allow configuration changes on devices to be safely rolled back (Config Rollback - stack).
- Process subscriber service/fault requests fairly, in the order received (Service Queue).
- Store the telecom device database in a self-balancing tree for guaranteed fast access (AVL Database).
- Analyze the network topology for connectivity, isolated segments, and least-latency routing (Network Graph - BFS/DFS/Dijkstra).
- Instantly verify whether a phone number belongs to an active subscriber (Subscriber Hash Table).

## 4. System Overview / Architecture

The system is a menu-driven C++ console application. Seven independent modules each address one requirement from the problem statement, sharing a common set of data models (`Models.h`).

```
                        main.cpp (menu-driven console UI)
   __________________________|_____________________________________________
  |            |            |             |            |          |         |
IncidentMgr  SearchEngine  ConfigRollback ServiceQueue  AVLDatabase NetworkGraph SubHashTable
(merge sort) (binary       (stack per     (FIFO queue)  (AVL tree   (BFS, DFS,   (hash table
              search)        device)                     of devices) Dijkstra)    w/ chaining)
```

### File / Folder Structure
```
TelecomMonitor/
├── include/
│   ├── Models.h               # Subscriber, NetworkDevice, Incident, ConfigSnapshot, ServiceRequest
│   ├── IncidentManager.h       # Merge sort - incident prioritization
│   ├── SearchEngine.h          # Binary search - subscriber & device lookup
│   ├── ConfigRollback.h        # Stack - per-device config rollback
│   ├── ServiceRequestQueue.h   # FIFO queue - service/fault requests
│   ├── AVLDeviceDatabase.h     # AVL tree - device database
│   ├── NetworkGraph.h          # Graph + BFS/DFS/Dijkstra - connectivity
│   └── SubscriberHashTable.h   # Hash table with chaining - subscriber verification
├── src/
│   ├── IncidentManager.cpp
│   ├── SearchEngine.cpp
│   ├── ConfigRollback.cpp
│   ├── ServiceRequestQueue.cpp
│   ├── AVLDeviceDatabase.cpp
│   ├── NetworkGraph.cpp
│   ├── SubscriberHashTable.cpp
│   └── main.cpp                 # menu-driven demo
├── data/
│   └── sample_data_notes.md
├── screenshots/
│   └── sample_run_output.txt
└── README.md
```

## 5. Data Structures and Algorithms Used

| # | Feature                | Data Structure / Algorithm                              | Justification |
|---|-------------------------|-----------------------------------------------------------|----------------|
| 1 | Incident Manager        | `std::vector<Incident>` + hand-written **Merge Sort**     | Incidents are ranked by severity for the operations team. Merge sort is a **stable** O(n log n) sort, so incidents of equal severity keep their original (oldest-first) order - important for fairness. |
| 2 | Search Engine           | `std::vector` kept sorted + **Binary Search**             | Subscriber/device IDs are sequential and rarely change, so a sorted array with O(log n) binary search gives fast, cache-friendly lookups for "retrieve subscriber and infrastructure data". |
| 3 | Config Rollback         | `std::stack<ConfigSnapshot>` per device                    | Configuration changes must be undone in LIFO order - the most recent change is rolled back first. O(1) push/pop. |
| 4 | Service Request Queue   | `std::queue<ServiceRequest>`                                | Service/fault requests are processed strictly FIFO - fairness for subscriber complaints, like a call-center ticket queue. O(1) enqueue/dequeue. |
| 5 | AVL Device Database     | **AVL Tree** (self-balancing BST)                          | A plain BST can degrade to O(n) if devices are inserted in sorted-ID order (common, since IDs are sequential). An AVL tree guarantees O(log n) height via rotations, so search/insert stay O(log n) even in the worst case. |
| 6 | Network Graph           | Adjacency list (`unordered_map<id, vector<Link>>`) + **BFS, DFS, Dijkstra** | Models the physical network topology. BFS finds reachable devices / outage isolation (fewest hops). DFS explores a full connected component for diagnostics. Dijkstra finds the lowest-latency path for QoS routing. |
| 7 | Subscriber Hash Table   | Hand-written **hash table with chaining** (phone number key) | Subscriber verification is the highest-frequency operation in the system (every call/SMS/session). A hash table gives O(1) average lookup regardless of subscriber count, with rehashing to maintain performance as load grows. |

## 6. Implementation Approach

1. **Models.h** defines plain structs (`Subscriber`, `NetworkDevice`, `Incident`, `ConfigSnapshot`, `ServiceRequest`) shared by every module.
2. **IncidentManager** stores incidents in a vector and produces a priority-ordered copy via a recursive **merge sort**, comparing by `severity` descending and preserving insertion order for ties (stability).
3. **SearchEngine** maintains two sorted vectors (subscribers by `subscriberId`, devices by `deviceId`). New records are inserted at their sorted position; lookups use **binary search** for O(log n) retrieval.
4. **ConfigRollback** keeps one `std::stack<ConfigSnapshot>` per device in an `unordered_map`. `saveConfig()` pushes a new snapshot; `rollback()` pops the most recent one.
5. **ServiceRequestQueue** wraps a `std::queue<ServiceRequest>`. `submitRequest()` enqueues; `processNext()` dequeues the longest-waiting ticket.
6. **AVLDeviceDatabase** implements a full **AVL tree** with left/right rotations (LL, RR, LR, RL cases) keyed by `deviceId`, supporting O(log n) insert/search and in-order traversal for a sorted device listing.
7. **NetworkGraph** stores a bidirectional adjacency list of device links (weighted by latency). It implements **BFS** (queue-based, fewest-hops reachability), **DFS** (stack-based, full connected component), and **Dijkstra's algorithm** (binary-heap priority queue, lowest-total-latency path).
8. **SubscriberHashTable** implements a hash table with chaining (`vector<list<Subscriber>>`), a polynomial rolling hash on phone numbers, and automatic rehashing (doubling) once the load factor exceeds 0.75.
9. **main.cpp** wires everything together behind a numbered menu, pre-loading sample subscribers, devices, incidents, and a small network topology.

## 7. Time and Space Complexity Analysis

| Feature | Operation | Time Complexity | Space Complexity |
|---|---|---|---|
| Incident Manager | Report incident | O(1) | O(i) |
| Incident Manager | Prioritize (merge sort) | O(i log i) | O(i) |
| Search Engine | Insert (sorted vector) | O(n) | O(n) |
| Search Engine | Binary search | O(log n) | O(1) |
| Config Rollback | Save / Rollback | O(1) | O(c) |
| Service Queue | Submit / Process next | O(1) | O(r) |
| AVL Database | Insert / Search | O(log d) | O(d) |
| AVL Database | In-order traversal | O(d) | O(d) |
| Network Graph | Add link | O(1) | O(V + E) |
| Network Graph | BFS / DFS | O(V + E) | O(V) |
| Network Graph | Dijkstra | O((V + E) log V) | O(V) |
| Subscriber Hash Table | Insert / Verify | O(1) average | O(s) |

Where: i = number of incidents, n = number of subscribers/devices in the search engine, c = number of saved configs for a device, r = number of pending service requests, d = number of devices in the AVL tree, V/E = graph vertices/edges, s = number of subscribers.

## 8. Execution Steps

### Prerequisites
- A C++17-compatible compiler (e.g., `g++`).

### Build
```bash
g++ -std=c++17 -Wall -Iinclude src/*.cpp -o telecom
```

### Run
```bash
./telecom
```

A numbered menu (1-15) lets you exercise each feature, plus `0` to exit. Sample subscribers, devices, incidents, and a network topology are pre-loaded.

## 9. Sample Inputs and Outputs

### Sample pre-loaded data

**Subscribers** (ID, Name, Phone, Plan, Status):
```
SUB1001, Aarav Sharma, 9000000001, Postpaid-5G, ACTIVE
SUB1002, Priya Nair,   9000000002, Prepaid-4G,  ACTIVE
SUB1003, Rohan Mehta,  9000000003, Prepaid-4G,  SUSPENDED
SUB1004, Sneha Iyer,   9000000004, Postpaid-5G, ACTIVE
```

**Network Devices** (ID, Name, Location, Status, Load%):
```
DEV2001, Panvel Tower A,    Panvel,   UP,       72
DEV2002, Kharghar Router,   Kharghar, UP,       45
DEV2003, Belapur Switch,    Belapur,  DEGRADED, 91
DEV2004, Vashi Tower B,     Vashi,    UP,       60
DEV2005, Nerul Core Router, Nerul,    DOWN,     0
```

**Network Links** (Device A, Device B, Latency ms):
```
DEV2001 - DEV2002, 5.0
DEV2002 - DEV2003, 3.5
DEV2003 - DEV2004, 8.0
DEV2001 - DEV2004, 12.0
DEV2004 - DEV2005, 4.0
DEV2002 - DEV2005, 15.0
```

**Incidents** (ID, Device, Description, Severity, Time):
```
INC301, DEV2003, High_packet_loss,          4, 08:10
INC302, DEV2005, Core_router_offline,       5, 08:15
INC303, DEV2001, Minor_signal_degradation,  2, 08:20
INC304, DEV2003, Repeated_timeouts,         4, 08:25
```

### Sample Output - Option 2 (Incident Manager - prioritized via merge sort)
```
--- Incidents Prioritized (most critical first, merge sort) ---
ID        Device    Sev     Description                 Time
INC302    DEV2005   5       Core_router_offline         08:15
INC301    DEV2003   4       High_packet_loss            08:10
INC304    DEV2003   4       Repeated_timeouts            08:25
INC303    DEV2001   2       Minor_signal_degradation    08:20
```
Note that INC301 (08:10) is listed before INC304 (08:25) despite both having severity 4 - the merge sort preserved their original reporting order (stability).

### Sample Output - Option 12 (BFS from DEV2001)
```
BFS order: DEV2001 -> DEV2002 -> DEV2004 -> DEV2003 -> DEV2005
```

### Sample Output - Option 13 (DFS from DEV2001)
```
DFS order: DEV2001 -> DEV2002 -> DEV2003 -> DEV2004 -> DEV2005
```

### Sample Output - Option 14 (Dijkstra, DEV2001 -> DEV2005)
```
Route: DEV2001 -> DEV2004 -> DEV2005
Total latency: 16 ms
```
(Direct DEV2001->DEV2002->DEV2005 would cost 5+15=20ms; via DEV2004 it is 12+4=16ms - Dijkstra correctly picks the lower-latency route.)

### Sample Output - Option 15 (Verify subscriber by phone)
```
Enter phone number: 9000000002
VERIFIED: Priya Nair (SUB1002) - subscription ACTIVE.

Enter phone number: 9000000003
Subscriber Rohan Mehta (SUB1003) found but status is SUSPENDED - verification FAILED.
```

## 10. Screenshots
See `screenshots/sample_run_output.txt` for a captured run demonstrating incident prioritization, network topology display, and the Dijkstra shortest-latency path. (Replace with actual terminal screenshots before final submission.)

## 11. Results and Observations
- The **merge sort** in the Incident Manager correctly ranks incidents by severity while preserving the reporting order of equal-severity incidents (stability), so older critical incidents are addressed first.
- **Binary search** in the Search Engine returns subscriber and device records in O(log n), confirmed against the small sample set and scalable to large subscriber bases.
- The **AVL tree** stays balanced (height stays close to log2(n)) even though devices were inserted in sorted ID order - confirming the rebalancing rotations work correctly.
- **BFS** correctly identifies all devices reachable from a starting node (useful for detecting network segments isolated by an outage).
- **DFS** explores the full connected component, useful for diagnostics across an entire network segment.
- **Dijkstra's algorithm** correctly identifies the lowest-total-latency route between two devices, even when it requires more hops than a direct link.
- The **hash table** with chaining verifies subscribers in O(1) average time and correctly distinguishes ACTIVE from SUSPENDED subscribers.
- The **stack-based rollback** and **FIFO service queue** behave exactly as expected for undo and fairness workflows respectively.

## 12. Conclusion
This system demonstrates how a combination of sorting (merge sort), searching (binary search), linear structures (stack, queue), balanced trees (AVL), graphs (BFS/DFS/Dijkstra), and hashing can address the real operational needs of a telecom network monitoring platform: prioritizing faults, retrieving records quickly, safely reversing configuration changes, processing service tickets fairly, maintaining a reliable device database, analyzing connectivity, and verifying subscribers instantly. Each data structure was chosen to match the access pattern and scale of its real-world use case, similar to systems used by telecom operators for network operations centers (NOC) and subscriber management.

## GitHub Repository Link
`<TO BE FILLED IN BEFORE SUBMISSION>`

## Student Details
- Name: Ayush yadgiri
- Course: B.Tech CSE 2025-29
- Subject: Data Structures & Algorithms with C++ (Semester II)
- Case Study: #147 - Smart Telecom Network Monitoring & Service Optimization System
