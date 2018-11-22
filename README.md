# SGX Demo Application

This simple application aims to act as a sample for Intel® SGX application structure and implementation.

This project approaches some techniques used with Intel® SGX, such as:
* Creating and Destroying Enclaves
* Enclaves Definition Language
* Data Sealing

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and learning purposes.

### Project Structure

This project is divided into two parts: [The Guessing Game](./guess-the-number/README.md) and [The Statistics Gather](./stats-keeper/README.md)

**Project Tree**
```
sgx-demo-app/
├── LICENSE
├── README.md
├── data
│   └── README.md
├── guess-the-number
│   ├── App
│   │   └── App.cpp
│   ├── Enclave
│   │   ├── Enclave.config.xml
│   │   ├── Enclave.cpp
│   │   ├── Enclave.edl
│   │   └── Enclave_private.pem
│   ├── Makefile
│   └── README.md
└── stats-keeper
    ├── App
    │   └── App.cpp
    ├── Enclave
    │   ├── Enclave.config.xml
    │   ├── Enclave.cpp
    │   ├── Enclave.edl
    │   └── Enclave_private.pem
    ├── Makefile
    └── README.md
```

### Prerequisites

To use this application, you need to have installed. To install these components, please follow the instructions available in the links provided below.

* [Intel® SGX for Linux](https://github.com/intel/linux-sgx)
* [Intel® SGX driver for Linux](https://github.com/01org/linux-sgx-driver)


### Installing

To install the application use the commands below:

* Clone this project using:

```
$ git clone https://github.com/jeflora/sgx-demo-app.git
```

* Build and run the application:

```
$ cd stats-keeper/
$ make SGX_MODE=SIM # for simulation mode
$ # or
$ make SGX_MODE=HW  # for hardware mode
$ ./app.out
```

```
$ cd guess-the-number/
$ make SGX_MODE=SIM # for simulation mode
$ # or
$ make SGX_MODE=HW  # for hardware mode
$ ./app.out
```

**NB:** Keep in mind that to run this project in hardware mode, it requires an Intel® Core 6th generation or more with Intel® SGX enabled.

## Built With

* [Intel® SGX](https://software.intel.com/en-us/sgx) - An Intel® architecture extension designed to increase the security of application code and data.

## Authors

* **José Flora**   - <jeflora@student.dei.uc.pt>
* **Nuno Antunes** - <nmsa@dei.uc.pt>
