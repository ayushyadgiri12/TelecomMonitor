#include "SubscriberHashTable.h"

SubscriberHashTable::SubscriberHashTable(int initialBuckets) : count(0) {
    buckets.resize(initialBuckets);
}

// Polynomial rolling hash (base 31), then mod bucket count
size_t SubscriberHashTable::hashFunction(const std::string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + (unsigned char)c;
    }
    return hash % buckets.size();
}

void SubscriberHashTable::rehash() {
    std::vector<std::list<Subscriber>> oldBuckets = buckets;
    buckets.assign(oldBuckets.size() * 2, std::list<Subscriber>());

    for (auto& bucket : oldBuckets) {
        for (auto& sub : bucket) {
            size_t idx = hashFunction(sub.phone);
            buckets[idx].push_back(sub);
        }
    }
}

void SubscriberHashTable::insertSubscriber(const Subscriber& s) {
    // Rehash if load factor would exceed 0.75 after this insert
    if ((double)(count + 1) / buckets.size() > 0.75) {
        rehash();
    }

    size_t idx = hashFunction(s.phone);
    for (auto& existing : buckets[idx]) {
        if (existing.phone == s.phone) {
            existing = s; // update existing record
            return;
        }
    }
    buckets[idx].push_back(s);
    count++;
}

bool SubscriberHashTable::verifySubscriber(const std::string& phone, Subscriber& out) const {
    size_t idx = hashFunction(phone);
    for (const auto& sub : buckets[idx]) {
        if (sub.phone == phone) {
            out = sub;
            return sub.status == "ACTIVE";
        }
    }
    return false;
}
