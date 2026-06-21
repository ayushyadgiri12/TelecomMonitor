#include "Models.h"
#include "IncidentManager.h"
#include "SearchEngine.h"
#include "ConfigRollback.h"
#include "ServiceRequestQueue.h"
#include "AVLDeviceDatabase.h"
#include "NetworkGraph.h"
#include "SubscriberHashTable.h"

#include <iostream>
#include <iomanip>
#include <limits>

// ---------------------------------------------------------
// Loads sample data so the demo is immediately usable:
//  - Subscribers (Search Engine / Hash Table)
//  - Network devices (Search Engine / AVL Database)
//  - Incidents (Incident Manager)
//  - Network topology (Network Graph)
// ---------------------------------------------------------
void loadSampleData(SearchEngine& search, AVLDeviceDatabase& avl, IncidentManager& incidents,
                     NetworkGraph& graph, SubscriberHashTable& hashTable) {
    // Subscribers: id, name, phone, plan, status
    Subscriber subs[] = {
        Subscriber("SUB1001", "Aarav Sharma",  "9000000001", "Postpaid-5G", "ACTIVE"),
        Subscriber("SUB1002", "Priya Nair",    "9000000002", "Prepaid-4G",  "ACTIVE"),
        Subscriber("SUB1003", "Rohan Mehta",   "9000000003", "Prepaid-4G",  "SUSPENDED"),
        Subscriber("SUB1004", "Sneha Iyer",    "9000000004", "Postpaid-5G", "ACTIVE"),
    };
    for (auto& s : subs) {
        search.addSubscriber(s);
        hashTable.insertSubscriber(s);
    }

    // Network devices: id, name, location, status, loadPercent
    NetworkDevice devices[] = {
        NetworkDevice("DEV2001", "Panvel Tower A",   "Panvel",   "UP",       72),
        NetworkDevice("DEV2002", "Kharghar Router",  "Kharghar", "UP",       45),
        NetworkDevice("DEV2003", "Belapur Switch",   "Belapur",  "DEGRADED", 91),
        NetworkDevice("DEV2004", "Vashi Tower B",     "Vashi",    "UP",       60),
        NetworkDevice("DEV2005", "Nerul Core Router","Nerul",    "DOWN",     0),
    };
    for (auto& d : devices) {
        search.addDevice(d);
        avl.insertDevice(d);
    }

    // Incidents: id, deviceId, description, severity (1-5), timestamp
    incidents.reportIncident(Incident("INC301", "DEV2003", "High_packet_loss", 4, "08:10"));
    incidents.reportIncident(Incident("INC302", "DEV2005", "Core_router_offline", 5, "08:15"));
    incidents.reportIncident(Incident("INC303", "DEV2001", "Minor_signal_degradation", 2, "08:20"));
    incidents.reportIncident(Incident("INC304", "DEV2003", "Repeated_timeouts", 4, "08:25"));

    // Network topology: deviceA, deviceB, latency(ms)
    graph.addLink("DEV2001", "DEV2002", 5.0);
    graph.addLink("DEV2002", "DEV2003", 3.5);
    graph.addLink("DEV2003", "DEV2004", 8.0);
    graph.addLink("DEV2001", "DEV2004", 12.0);
    graph.addLink("DEV2004", "DEV2005", 4.0);
    graph.addLink("DEV2002", "DEV2005", 15.0);
}

void pause() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void printMenu() {
    std::cout << "\n============ Smart Telecom Network Monitoring & Service Optimization ============\n";
    std::cout << " 1. Incident Manager  - Report a new incident\n";
    std::cout << " 2. Incident Manager  - Show prioritized incidents (merge sort)\n";
    std::cout << " 3. Search Engine     - Find subscriber by ID (binary search)\n";
    std::cout << " 4. Search Engine     - Find device by ID (binary search)\n";
    std::cout << " 5. Config Rollback   - Save a config snapshot (stack push)\n";
    std::cout << " 6. Config Rollback   - Rollback last config change (stack pop)\n";
    std::cout << " 7. Service Queue     - Submit a service/fault request\n";
    std::cout << " 8. Service Queue     - Process next request (FIFO)\n";
    std::cout << " 9. AVL Database      - Find device + show tree height\n";
    std::cout << "10. AVL Database      - Show all devices sorted (in-order)\n";
    std::cout << "11. Network Graph     - Display topology\n";
    std::cout << "12. Network Graph     - BFS from a device\n";
    std::cout << "13. Network Graph     - DFS from a device\n";
    std::cout << "14. Network Graph     - Shortest-latency path (Dijkstra)\n";
    std::cout << "15. Hash Table        - Verify subscriber by phone (O(1))\n";
    std::cout << " 0. Exit\n";
    std::cout << "====================================================================================\n";
    std::cout << "Enter choice: ";
}

int main() {
    SearchEngine search;
    AVLDeviceDatabase avl;
    IncidentManager incidents;
    NetworkGraph graph;
    SubscriberHashTable hashTable;
    ConfigRollback rollback;
    ServiceRequestQueue serviceQueue;

    loadSampleData(search, avl, incidents, graph, hashTable);

    int choice;
    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string id, dev, desc, time;
                int sev;
                std::cout << "Incident ID: "; std::cin >> id;
                std::cout << "Device ID: "; std::cin >> dev;
                std::cout << "Description (no spaces): "; std::cin >> desc;
                std::cout << "Severity (1-5): "; std::cin >> sev;
                std::cout << "Timestamp (e.g. 09:30): "; std::cin >> time;
                incidents.reportIncident(Incident(id, dev, desc, sev, time));
                std::cout << "Incident logged.\n";
                break;
            }
            case 2: {
                std::vector<Incident> ranked = incidents.prioritized();
                std::cout << "\n--- Incidents Prioritized (most critical first, merge sort) ---\n";
                std::cout << std::left << std::setw(10) << "ID" << std::setw(10) << "Device"
                          << std::setw(8) << "Sev" << std::setw(28) << "Description" << "Time\n";
                for (const auto& inc : ranked) {
                    std::cout << std::left << std::setw(10) << inc.incidentId << std::setw(10) << inc.deviceId
                              << std::setw(8) << inc.severity << std::setw(28) << inc.description
                              << inc.timestamp << "\n";
                }
                break;
            }
            case 3: {
                std::string id;
                std::cout << "Enter Subscriber ID (e.g. SUB1001): ";
                std::cin >> id;
                const Subscriber* s = search.findSubscriber(id);
                if (s) {
                    std::cout << "Found: " << s->name << " | Phone: " << s->phone
                              << " | Plan: " << s->plan << " | Status: " << s->status << "\n";
                } else {
                    std::cout << "No subscriber found with ID " << id << "\n";
                }
                break;
            }
            case 4: {
                std::string id;
                std::cout << "Enter Device ID (e.g. DEV2001): ";
                std::cin >> id;
                const NetworkDevice* d = search.findDevice(id);
                if (d) {
                    std::cout << "Found: " << d->name << " | Location: " << d->location
                              << " | Status: " << d->status << " | Load: " << d->loadPercent << "%\n";
                } else {
                    std::cout << "No device found with ID " << id << "\n";
                }
                break;
            }
            case 5: {
                std::string dev, cfg, time;
                std::cout << "Device ID: "; std::cin >> dev;
                std::cout << "Config data (no spaces): "; std::cin >> cfg;
                std::cout << "Timestamp: "; std::cin >> time;
                rollback.saveConfig(ConfigSnapshot(dev, cfg, time));
                std::cout << "Configuration saved (history depth for " << dev << ": "
                          << rollback.historyCount(dev) << ")\n";
                break;
            }
            case 6: {
                std::string dev;
                std::cout << "Device ID to rollback: "; std::cin >> dev;
                ConfigSnapshot snap;
                if (rollback.rollback(dev, snap)) {
                    std::cout << "Rolled back configuration '" << snap.configData
                              << "' (saved at " << snap.timestamp << ") for device " << dev << "\n";
                } else {
                    std::cout << "No configuration history found for device " << dev << "\n";
                }
                break;
            }
            case 7: {
                std::string ticket, sub, issue, time;
                std::cout << "Ticket ID: "; std::cin >> ticket;
                std::cout << "Subscriber ID: "; std::cin >> sub;
                std::cout << "Issue (no spaces): "; std::cin >> issue;
                std::cout << "Timestamp: "; std::cin >> time;
                serviceQueue.submitRequest(ServiceRequest(ticket, sub, issue, time));
                std::cout << "Service request submitted.\n";
                break;
            }
            case 8: {
                ServiceRequest req;
                if (serviceQueue.processNext(req)) {
                    std::cout << "Processing ticket " << req.ticketId << " for subscriber "
                              << req.subscriberId << " (Issue: " << req.issue
                              << ", Reported: " << req.timestamp << ")\n";
                } else {
                    std::cout << "No pending service requests.\n";
                }
                break;
            }
            case 9: {
                std::string id;
                std::cout << "Enter Device ID (e.g. DEV2003): ";
                std::cin >> id;
                const NetworkDevice* d = avl.findDevice(id);
                if (d) {
                    std::cout << "Found: " << d->name << " | Location: " << d->location
                              << " | Status: " << d->status << " | Load: " << d->loadPercent << "%\n";
                } else {
                    std::cout << "No device found with ID " << id << "\n";
                }
                std::cout << "Current AVL tree height: " << avl.treeHeight() << "\n";
                break;
            }
            case 10: {
                std::vector<NetworkDevice> all = avl.allDevicesSorted();
                std::cout << "\n--- Devices (AVL in-order traversal, sorted by Device ID) ---\n";
                std::cout << std::left << std::setw(10) << "ID" << std::setw(20) << "Name"
                          << std::setw(12) << "Location" << std::setw(10) << "Status" << "Load%\n";
                for (const auto& d : all) {
                    std::cout << std::left << std::setw(10) << d.deviceId << std::setw(20) << d.name
                              << std::setw(12) << d.location << std::setw(10) << d.status
                              << d.loadPercent << "\n";
                }
                break;
            }
            case 11:
                graph.displayTopology();
                break;

            case 12: {
                std::string start;
                std::cout << "Start Device ID: "; std::cin >> start;
                std::vector<std::string> order = graph.bfs(start);
                std::cout << "BFS order: ";
                for (size_t i = 0; i < order.size(); ++i) {
                    std::cout << order[i];
                    if (i + 1 < order.size()) std::cout << " -> ";
                }
                std::cout << (order.empty() ? "(device not found)" : "") << "\n";
                break;
            }
            case 13: {
                std::string start;
                std::cout << "Start Device ID: "; std::cin >> start;
                std::vector<std::string> order = graph.dfs(start);
                std::cout << "DFS order: ";
                for (size_t i = 0; i < order.size(); ++i) {
                    std::cout << order[i];
                    if (i + 1 < order.size()) std::cout << " -> ";
                }
                std::cout << (order.empty() ? "(device not found)" : "") << "\n";
                break;
            }
            case 14: {
                std::string src, dst;
                std::cout << "Source Device ID: "; std::cin >> src;
                std::cout << "Destination Device ID: "; std::cin >> dst;
                double latency;
                std::vector<std::string> path = graph.shortestLatencyPath(src, dst, latency);
                if (path.empty()) {
                    std::cout << "No route found.\n";
                } else {
                    std::cout << "Route: ";
                    for (size_t i = 0; i < path.size(); ++i) {
                        std::cout << path[i];
                        if (i + 1 < path.size()) std::cout << " -> ";
                    }
                    std::cout << "\nTotal latency: " << latency << " ms\n";
                }
                break;
            }
            case 15: {
                std::string phone;
                std::cout << "Enter phone number (e.g. 9000000001): ";
                std::cin >> phone;
                Subscriber s;
                bool active = hashTable.verifySubscriber(phone, s);
                if (s.subscriberId.empty()) {
                    std::cout << "No subscriber registered with that phone number.\n";
                } else if (active) {
                    std::cout << "VERIFIED: " << s.name << " (" << s.subscriberId
                              << ") - subscription ACTIVE.\n";
                } else {
                    std::cout << "Subscriber " << s.name << " (" << s.subscriberId
                              << ") found but status is " << s.status << " - verification FAILED.\n";
                }
                break;
            }
            case 0:
                std::cout << "Exiting Telecom Monitoring System. Goodbye!\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

        if (choice != 0) pause();

    } while (choice != 0);

    return 0;
}
