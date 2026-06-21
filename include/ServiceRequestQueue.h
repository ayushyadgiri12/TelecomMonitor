#ifndef SERVICE_REQUEST_QUEUE_H
#define SERVICE_REQUEST_QUEUE_H

#include "Models.h"
#include <queue>
#include <string>

// ---------------------------------------------------------
// ServiceRequestQueue
//
// Backed by a std::queue<ServiceRequest> (FIFO).
//
// Why a queue?
//   The problem statement requires the system to "process
//   service requests and fault reports". A FIFO queue ensures
//   subscriber complaints/fault reports are handled in the
//   order they were received - a fairness guarantee similar
//   to a real telecom call-center ticketing system. O(1)
//   enqueue (submitRequest) and O(1) dequeue (processNext).
// ---------------------------------------------------------
class ServiceRequestQueue {
private:
    std::queue<ServiceRequest> requests;

public:
    // Submit a new service/fault request - O(1)
    void submitRequest(const ServiceRequest& req);

    // Process the next request (front of the queue) - O(1)
    bool processNext(ServiceRequest& out);

    // Peek at the next request without removing it
    bool peekNext(ServiceRequest& out) const;

    int size() const { return (int)requests.size(); }
    bool empty() const { return requests.empty(); }
};

#endif // SERVICE_REQUEST_QUEUE_H
