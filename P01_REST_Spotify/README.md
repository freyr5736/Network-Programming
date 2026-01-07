# REST API Implementation in C++

This project was built to become familiar with making REST API calls using C++. It uses the Spotify API to perform `GET` and `POST` requests and follows an object-oriented design to maintain a clean and organized flow.

The application runs on the command-line interface (CLI) and is capable of fetching details from the API.

---

## Features

* Fetch artist information
* Fetch song details
* Retrieve authentication tokens
* View the current token
* Read credential data from a file

---

## Implementation Details

This implementation uses `libcurl` to handle REST API calls. It currently supports client-side interactions and basic server-side authentication.

Advanced operations such as `update` and `delete` requests are planned but have not yet been implemented.

Helper functions are included to simplify data retrieval, and the overall project structure is kept clean by separating functionality into different files for better readability and maintainability.

---
