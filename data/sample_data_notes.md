# Sample Data Notes

The application loads the following sample data automatically on startup
(see `loadSampleData()` in `src/main.cpp`).

## Subscribers (Search Engine / Hash Table)
| Subscriber ID | Name         | Phone       | Plan         | Status    |
|----------------|--------------|-------------|--------------|-----------|
| SUB1001        | Aarav Sharma | 9000000001  | Postpaid-5G  | ACTIVE    |
| SUB1002        | Priya Nair   | 9000000002  | Prepaid-4G   | ACTIVE    |
| SUB1003        | Rohan Mehta  | 9000000003  | Prepaid-4G   | SUSPENDED |
| SUB1004        | Sneha Iyer   | 9000000004  | Postpaid-5G  | ACTIVE    |

## Network Devices (Search Engine / AVL Database)
| Device ID | Name              | Location | Status   | Load% |
|-----------|-------------------|----------|----------|-------|
| DEV2001   | Panvel Tower A    | Panvel   | UP       | 72    |
| DEV2002   | Kharghar Router   | Kharghar | UP       | 45    |
| DEV2003   | Belapur Switch    | Belapur  | DEGRADED | 91    |
| DEV2004   | Vashi Tower B     | Vashi    | UP       | 60    |
| DEV2005   | Nerul Core Router | Nerul    | DOWN     | 0     |

## Incidents (Incident Manager)
| Incident ID | Device  | Description               | Severity | Time  |
|-------------|---------|----------------------------|----------|-------|
| INC301      | DEV2003 | High_packet_loss           | 4        | 08:10 |
| INC302      | DEV2005 | Core_router_offline        | 5        | 08:15 |
| INC303      | DEV2001 | Minor_signal_degradation    | 2        | 08:20 |
| INC304      | DEV2003 | Repeated_timeouts           | 4        | 08:25 |

## Network Topology (Network Graph)
| Device A | Device B | Latency (ms) |
|----------|----------|----------------|
| DEV2001  | DEV2002  | 5.0            |
| DEV2002  | DEV2003  | 3.5            |
| DEV2003  | DEV2004  | 8.0            |
| DEV2001  | DEV2004  | 12.0           |
| DEV2004  | DEV2005  | 4.0            |
| DEV2002  | DEV2005  | 15.0           |

These values can be changed directly in `loadSampleData()` in `src/main.cpp`
to test the system with different scenarios.
