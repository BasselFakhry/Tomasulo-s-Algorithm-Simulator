# Tomasulo’s Algorithm Simulator

A lightweight simulator for Tomasulo’s dynamic scheduling algorithm to demonstrate instruction-level parallelism and CPU performance optimization.

---

This project implements a simulator for **Tomasulo’s algorithm**, a dynamic scheduling technique used to optimize CPU performance by exploiting instruction-level parallelism. The simulator models pipelined execution with reservation stations, register renaming, and hazard handling (structural, data, and WAW), and supports arithmetic and memory operations (ADD, SUB, MUL, DIV, LOAD, STORE). It outputs key performance metrics such as execution time, IPC, reservation station occupancy, buffer utilization, and stall statistics.
