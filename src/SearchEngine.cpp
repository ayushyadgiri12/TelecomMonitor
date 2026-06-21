#include "SearchEngine.h"

void SearchEngine::insertSortedSubscriber(std::vector<Subscriber>& vec, const Subscriber& s) {
    auto it = vec.begin();
    while (it != vec.end() && it->subscriberId < s.subscriberId) ++it;
    vec.insert(it, s);
}

void SearchEngine::insertSortedDevice(std::vector<NetworkDevice>& vec, const NetworkDevice& d) {
    auto it = vec.begin();
    while (it != vec.end() && it->deviceId < d.deviceId) ++it;
    vec.insert(it, d);
}

void SearchEngine::addSubscriber(const Subscriber& s) {
    insertSortedSubscriber(subscribers, s);
}

void SearchEngine::addDevice(const NetworkDevice& d) {
    insertSortedDevice(devices, d);
}

const Subscriber* SearchEngine::findSubscriber(const std::string& subscriberId) const {
    int lo = 0, hi = (int)subscribers.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (subscribers[mid].subscriberId == subscriberId) return &subscribers[mid];
        if (subscribers[mid].subscriberId < subscriberId) lo = mid + 1;
        else hi = mid - 1;
    }
    return nullptr;
}

const NetworkDevice* SearchEngine::findDevice(const std::string& deviceId) const {
    int lo = 0, hi = (int)devices.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (devices[mid].deviceId == deviceId) return &devices[mid];
        if (devices[mid].deviceId < deviceId) lo = mid + 1;
        else hi = mid - 1;
    }
    return nullptr;
}
