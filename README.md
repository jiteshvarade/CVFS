---

# Customized Virtual File System (CVFS)

## Overview

The Customized Virtual File System (CVFS) project is designed to simulate the behavior of a file system, providing an educational tool to understand file system internals. This project allows the creation, management, and manipulation of files within a virtualized environment, helping users comprehend how file systems handle files, inodes, and related structures.

## Features

- **Inode Management:** Create and manage inodes for different files.
- **File Operations:** Support for basic file operations such as create, read, write, and delete.
- **File Permissions:** Implement file permissions to control access levels.
- **Command Support:** Various commands to interact with the virtual file system, with descriptions available via the `man` function.

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- Standard C++ libraries

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/your-username/CVFS.git
    ```
2. Navigate to the project directory:
    ```sh
    cd CVFS
    ```
3. Compile the code:
    ```sh
    g++ CVFS.cpp -o cvfs
    ```

### Usage

Run the compiled executable:
```sh
./cvfs
```

### Commands

- **create:** Create a new regular file.
  ```sh
  create <File_name> <Permission>
  ```
  Example:
  ```sh
  create testfile 1
  ```

- **read:** Read data from a regular file.
  ```sh
  read <File_name> <No_of_Bytes_To_Read>
  ```
  Example:
  ```sh
  read testfile 100
  ```

- **write:** Write data to a regular file.
  ```sh
  write <File_name>
  ```
  Example:
  ```sh
  write testfile
  ```

- **delete:** Delete a file.
  ```sh
  delete <File_name>
  ```
  Example:
  ```sh
  delete testfile
  ```

### Project Structure

- **`CVFS.cpp`**: The main source file containing the implementation of the virtual file system.

### Contributing

Contributions are welcome! Please fork this repository and submit pull requests.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Acknowledgments

This project was created by Jitesh Anil Varade as an educational tool to understand file system internals.

---
