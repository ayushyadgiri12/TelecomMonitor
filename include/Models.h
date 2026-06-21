#ifndef MODELS_H
#define MODELS_H

#include <string>

// ---------------------------------------------------------
// Core data models shared across all TelecomMonitor modules
// ---------------------------------------------------------

// A subscriber/customer record
struct Subscriber {
    std::string subscriberId; // unique ID, e.g. "SUB1001"
    std::string name;
    std::string phone;
    std::string plan;     // e.g. "Prepaid-4G", "Postpaid-5G"
    std::string status;   // "ACTIVE", "SUSPENDED", "INACTIVE"

    Subscriber() {}
    Subscriber(std::string id, std::string n, std::string ph, std::string pl, std::string st)
        : subscriberId(id), name(n), phone(ph), plan(pl), status(st) {}
};

// A network device / infrastructure node (tower, router, switch, etc.)
struct NetworkDevice {
    std::string deviceId; // unique ID, e.g. "DEV2001"
    std::string name;
    std::string location;
    std::string status;     // "UP", "DOWN", "DEGRADED"
    int loadPercent;         // current load percentage (0-100)

    NetworkDevice() : loadPercent(0) {}
    NetworkDevice(std::string id, std::string n, std::string loc, std::string st, int load)
        : deviceId(id), name(n), location(loc), status(st), loadPercent(load) {}
};

// A reported network incident / fault
struct Incident {
    std::string incidentId;
    std::string deviceId;       // affected device
    std::string description;
    int severity;                // 1 (low) - 5 (critical)
    std::string timestamp;

    Incident() : severity(0) {}
    Incident(std::string id, std::string dev, std::string desc, int sev, std::string time)
        : incidentId(id), deviceId(dev), description(desc), severity(sev), timestamp(time) {}
};

// A saved configuration snapshot for a device (for rollback)
struct ConfigSnapshot {
    std::string deviceId;
    std::string configData;  // simplified config string
    std::string timestamp;

    ConfigSnapshot() {}
    ConfigSnapshot(std::string dev, std::string cfg, std::string time)
        : deviceId(dev), configData(cfg), timestamp(time) {}
};

// A service / fault request from a subscriber
struct ServiceRequest {
    std::string ticketId;
    std::string subscriberId;
    std::string issue;
    std::string timestamp;

    ServiceRequest() {}
    ServiceRequest(std::string tId, std::string subId, std::string iss, std::string time)
        : ticketId(tId), subscriberId(subId), issue(iss), timestamp(time) {}
};

#endif // MODELS_H
