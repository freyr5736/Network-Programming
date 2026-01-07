# Simple gRPC Implementation in C++

This is a simple gRPC implementation developed in a Linux environment to understand how gRPC works and how services are implemented in C++.

The project was built by following a YouTube tutorial to learn the fundamentals of gRPC, including service definition, server setup, and request handling.

---

## Overview

The implementation creates a local gRPC server and runs it inside a Docker container. It includes a basic setup with a single test function, intended purely for experimentation and familiarity with the gRPC environment.

The server can be launched locally, and the exposed function can be called from within Docker by sending values to it.

---

## Capabilities

* Local gRPC server implementation
* Docker-based execution
* Single test function for validation
* Supports extending multiple functions within the gRPC environment

---

## Project Details

* Built and tested on Linux
* Uses CMake for build configuration
* Includes all necessary files such as:

  * `.proto` definitions
  * CMake configuration files
  * Source files required to run the project

This project serves as a foundational setup for experimenting with gRPC services and can be extended to include additional RPC methods as needed.

---
