#ifndef INCIDENT_MANAGER_H
#define INCIDENT_MANAGER_H

#include "Models.h"
#include <vector>
#include <string>

// ---------------------------------------------------------
// IncidentManager
//
// Backed by a std::vector<Incident>, ranked using a
// hand-written MERGE SORT (Sorting Algorithms requirement).
//
// Why merge sort?
//   Incidents must be prioritized by severity (and, for ties,
//   by how long they have been waiting) so the network
//   operations team always works on the most critical fault
//   first. Merge sort gives a STABLE O(n log n) sort -
//   stability matters here because two incidents of the same
//   severity should retain their reporting order (oldest
//   first), which an unstable sort (e.g. plain quicksort)
//   does not guarantee.
// ---------------------------------------------------------
class IncidentManager {
private:
    std::vector<Incident> incidents;

    // Recursive merge sort - sorts by severity DESC (5 = critical first),
    // and for equal severity, preserves original (report) order (stable).
    static void mergeSort(std::vector<Incident>& arr, int left, int right);
    static void merge(std::vector<Incident>& arr, int left, int mid, int right);

public:
    // Report a new incident - O(1)
    void reportIncident(const Incident& inc);

    // Return incidents ranked by priority (most critical first) - O(n log n)
    std::vector<Incident> prioritized() const;

    int size() const { return (int)incidents.size(); }
};

#endif // INCIDENT_MANAGER_H
