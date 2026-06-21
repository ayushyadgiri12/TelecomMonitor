#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "Models.h"
#include <vector>
#include <string>

// ---------------------------------------------------------
// SearchEngine
//
// Backed by std::vector<Subscriber> and std::vector<NetworkDevice>,
// each kept SORTED by their ID, searched with a hand-written
// BINARY SEARCH (Searching Algorithms requirement).
//
// Why binary search over sorted arrays?
//   The problem statement asks for algorithms to "retrieve
//   subscriber and infrastructure data". Subscriber/device
//   IDs are allocated sequentially and rarely change, so a
//   sorted contiguous array with O(log n) binary search gives
//   very fast, cache-friendly lookups with minimal memory
//   overhead - a good complement to the hash-based lookup
//   used for "quick subscriber verification" (a different,
//   higher-frequency access pattern handled separately by
//   SubscriberHashTable).
// ---------------------------------------------------------
class SearchEngine {
private:
    std::vector<Subscriber> subscribers;     // kept sorted by subscriberId
    std::vector<NetworkDevice> devices;      // kept sorted by deviceId

    // Insert into a sorted vector at the correct position - O(n)
    static void insertSortedSubscriber(std::vector<Subscriber>& vec, const Subscriber& s);
    static void insertSortedDevice(std::vector<NetworkDevice>& vec, const NetworkDevice& d);

public:
    void addSubscriber(const Subscriber& s);
    void addDevice(const NetworkDevice& d);

    // Binary search by ID - O(log n). Returns pointer or nullptr.
    const Subscriber* findSubscriber(const std::string& subscriberId) const;
    const NetworkDevice* findDevice(const std::string& deviceId) const;

    int subscriberCount() const { return (int)subscribers.size(); }
    int deviceCount() const { return (int)devices.size(); }
};

#endif // SEARCH_ENGINE_H
