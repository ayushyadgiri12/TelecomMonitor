#include "IncidentManager.h"

void IncidentManager::reportIncident(const Incident& inc) {
    incidents.push_back(inc);
}

// Merge step: combines two sorted halves [left..mid] and [mid+1..right]
// into a single sorted run, sorted by severity DESCENDING.
// Using <= (not <) when comparing equal severities preserves the
// original relative order of equal elements -> stability.
void IncidentManager::merge(std::vector<Incident>& arr, int left, int mid, int right) {
    std::vector<Incident> leftHalf(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<Incident> rightHalf(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < (int)leftHalf.size() && j < (int)rightHalf.size()) {
        if (leftHalf[i].severity >= rightHalf[j].severity) {
            arr[k++] = leftHalf[i++];
        } else {
            arr[k++] = rightHalf[j++];
        }
    }
    while (i < (int)leftHalf.size()) arr[k++] = leftHalf[i++];
    while (j < (int)rightHalf.size()) arr[k++] = rightHalf[j++];
}

void IncidentManager::mergeSort(std::vector<Incident>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

std::vector<Incident> IncidentManager::prioritized() const {
    std::vector<Incident> copy = incidents;
    if (!copy.empty()) mergeSort(copy, 0, (int)copy.size() - 1);
    return copy;
}
