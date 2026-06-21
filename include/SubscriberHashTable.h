#ifndef SUBSCRIBER_HASH_TABLE_H
#define SUBSCRIBER_HASH_TABLE_H

#include "Models.h"
#include <vector>
#include <list>
#include <string>

// ---------------------------------------------------------
// SubscriberHashTable
//
// A hand-written HASH TABLE WITH CHAINING, keyed by subscriber
// phone number, implementing the "Hashing: Enable quick
// subscriber verification" requirement.
//
// Why hashing for subscriber verification?
//   Verification (e.g. "does this phone number belong to an
//   active subscriber?") is the HIGHEST-FREQUENCY lookup in
//   the system - performed on every call, SMS, or data
//   session. A hash table gives O(1) average-case lookup,
//   independent of how many subscribers exist, which is
//   essential at telecom scale (millions of subscribers).
//   Collisions are handled via chaining (a linked list per
//   bucket), and the table doubles + rehashes once the load
//   factor exceeds 0.75 to keep operations close to O(1).
// ---------------------------------------------------------
class SubscriberHashTable {
private:
    std::vector<std::list<Subscriber>> buckets;
    int count;

    // Simple polynomial string hash function
    size_t hashFunction(const std::string& key) const;
    void rehash();

public:
    explicit SubscriberHashTable(int initialBuckets = 16);

    // Insert/update a subscriber, keyed by phone number - O(1) average
    void insertSubscriber(const Subscriber& s);

    // Verify a subscriber by phone number - O(1) average.
    // Returns true and fills `out` if found and ACTIVE.
    bool verifySubscriber(const std::string& phone, Subscriber& out) const;

    int bucketCount() const { return (int)buckets.size(); }
    int size() const { return count; }
    double loadFactor() const { return (double)count / buckets.size(); }
};

#endif // SUBSCRIBER_HASH_TABLE_H
