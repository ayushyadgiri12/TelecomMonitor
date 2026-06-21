#include "ServiceRequestQueue.h"

void ServiceRequestQueue::submitRequest(const ServiceRequest& req) {
    requests.push(req);
}

bool ServiceRequestQueue::processNext(ServiceRequest& out) {
    if (requests.empty()) return false;
    out = requests.front();
    requests.pop();
    return true;
}

bool ServiceRequestQueue::peekNext(ServiceRequest& out) const {
    if (requests.empty()) return false;
    out = requests.front();
    return true;
}
