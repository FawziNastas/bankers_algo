# Banker's Algorithm Project

## Overview
This project is an implementation of the Banker's Algorithm, which is a resource allocation and deadlock avoidance algorithm developed by Edsger Dijkstra. It is used in computing systems to ensure safe allocation of resources and prevent deadlock, similarly to how a bank assesses whether it can issue loans without risking its ability to meet all customers' needs.

## Project Functionality
The algorithm checks if a system can grant requested resources while remaining in a safe state. It ensures that resources are allocated in such a way that all processes can complete their tasks without entering a deadlock. Key data structures used include:
- **Max**: Maximum demand of each process.
- **Allocation**: Current allocation of resources to each process.
- **Need**: Remaining resource needs for each process.

### Key Features:
- Multithreaded execution of the algorithm.
- Safe state verification before allocation.
- Handling of resource requests and releases.
- Prevention of deadlocks.

## Screenshots
Screenshots demonstrating the execution of the program can be found in the "Screenshots of Execution" section of the report.

## Conclusion
The project demonstrates how the Banker's Algorithm can effectively prevent deadlock by maintaining a safe state in resource allocation. However, it also highlights limitations, such as its inability to handle dynamic processes and its impracticality for modern operating systems due to the need for predetermined resource requests.

## Appendix
The full implementation can be found in the accompanying source code, written in C and utilizing pthreads for multithreading.
