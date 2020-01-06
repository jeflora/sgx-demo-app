# SGX Demo Application - The Statistics Gatherer

This simple application aims to act as a sample for Intel® SGX application structure and implementation.

This part of the application receives the user's guesses from the guessing game enigine and stores them, furthermore it also sends the number of guesses made until the moment when an admin asks for it.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and learning purposes.

### Example asciicast

[![asciicast](https://asciinema.org/a/213171.svg)](https://asciinema.org/a/213171)

### Project Structure

An Intel® SGX project contains two parts, an [untrusted part](./App) and a [trusted part](./Enclave).

**Project Tree**
```
stats-keeper/
├── App (Untrusted)
│   └── App.cpp
├── Enclave (Trusted)
│   ├── Enclave.config.xml
│   ├── Enclave.cpp
│   ├── Enclave.edl
│   └── Enclave_private.pem
├── Makefile
└── README.md
```

#### Untrusted Part

The [App](./App) folder holds the header and source code files for the untrusted part of the project.

To extend the number of features available in this application, you can add more of them in [App.cpp](./App/App.cpp) or create new source files.

#### Trusted Part

The [Enclave](./Enclave) folder holds the header and source code and enclave's configuration files for the trusted part of the project.

To add more enclaves, you can create folders similar to [Enclave](./Enclave), containing the enclave code, [interface definition](./Enclave/Enclave.edl), the enclave's [configuration](./Enclave/Enclave.config.xml) and its [private key](./Enclave/Enclave_private.pem).

To generate a private key for an Enclave use:
```
$ openssl genrsa -aes128 -out <Enclave_private.pem> -3 3072
```

### Prerequisites

To use this application, you need to have installed. To install these components, please follow the instructions available in the links provided below.

* [Intel® SGX for Linux](https://github.com/intel/linux-sgx)
* [Intel® SGX driver for Linux](https://github.com/01org/linux-sgx-driver)


### Installing

* Build and run the application:

```
$ cd stats-keeper/
$ make SGX_MODE=SIM # for simulation mode
$ # or
$ make SGX_MODE=HW  # for hardware mode
$ ./app.out
```

**NB:** Keep in mind that to run this project in hardware mode, it requires an Intel® Core 6th generation or more with Intel® SGX enabled.

## Built With

* [Intel® SGX](https://software.intel.com/en-us/sgx) - An Intel® architecture extension designed to increase the security of application code and data.

## Authors

* **José Flora**   - <jeflora@dei.uc.pt>
* **Nuno Antunes** - <nmsa@dei.uc.pt>
