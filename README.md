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

Sure, I'll provide detailed explanations of the functions without showing the code. These descriptions will help users understand the purpose and usage of each function in your Customized Virtual File System (CVFS) project.

---

### Function Descriptions

#### `man`
**Usage:** `man(command_name)`
**Description:** Displays a manual entry for the specified command, including a description and usage syntax. If the command is not recognized, an error message is shown.

#### `InitializeSuperBlock`
**Usage:** `InitializeSuperBlock()`
**Description:** Initializes the superblock of the virtual file system, setting the total number of inodes and marking all inodes as free.

#### `CreateDILB`
**Usage:** `CreateDILB()`
**Description:** Creates the Disk Inode List Block (DILB), which is a linked list of inodes. This function sets up the inodes required for managing files in the virtual file system.

#### `CreateFile`
**Usage:** `CreateFile(name, permission)`
**Description:** Creates a new regular file with the given name and permission. The permissions can be read, write, or both. If successful, it returns the file descriptor of the newly created file.

#### `rm_File`
**Usage:** `rm_File(name)`
**Description:** Removes a file with the specified name. It decreases the link count of the inode and, if no links remain, it frees the inode and associated resources.

#### `ReadFile`
**Usage:** `ReadFile(fd, buffer, size)`
**Description:** Reads data from an open file, specified by its file descriptor (`fd`), into a buffer. The function reads up to `size` bytes from the current read offset.

#### `WriteFile`
**Usage:** `WriteFile(fd, buffer, size)`
**Description:** Writes data to an open file, specified by its file descriptor (`fd`), from a buffer. The function writes up to `size` bytes to the current write offset.

#### `OpenFile`
**Usage:** `OpenFile(name, mode)`
**Description:** Opens an existing file with the given name in the specified mode (read, write, or both). If successful, it returns the file descriptor of the opened file.

#### `CloseFile`
**Usage:** `CloseFile(fd)`
**Description:** Closes an open file specified by its file descriptor (`fd`). It decreases the reference count of the inode and, if no references remain, it frees the file table entry.

#### `CloseAllFiles`
**Usage:** `CloseAllFiles()`
**Description:** Closes all open files, resetting the file table and freeing associated resources.

#### `LseekFile`
**Usage:** `LseekFile(fd, offset, whence)`
**Description:** Changes the file offset for an open file specified by its file descriptor (`fd`). The `whence` parameter specifies how the offset should be interpreted (start, current, or end).

#### `ls`
**Usage:** `ls()`
**Description:** Lists all files in the virtual file system, displaying their names and other relevant details.

#### `stat`
**Usage:** `stat(name)`
**Description:** Displays information about a file specified by its name, such as its size, permissions, and inode number.

#### `fstat`
**Usage:** `fstat(fd)`
**Description:** Displays information about an open file specified by its file descriptor (`fd`), similar to the `stat` function.

#### `truncate`
**Usage:** `truncate(name)`
**Description:** Truncates a file to zero length, effectively removing all its data while keeping the file structure intact.

#### `Get_Inode`
**Usage:** `Get_Inode(name)`
**Description:** Retrieves the inode associated with the specified file name. This is a utility function used internally to manage file metadata.

#### `GetFDFromName`
**Usage:** `GetFDFromName(name)`
**Description:** Retrieves the file descriptor for a file specified by its name. This is a utility function used internally to handle file operations.

---

### Project Structure

- **`CVFS.cpp`**: The main source file containing the implementation of the virtual file system.

### Contributing

Contributions are welcome! Please fork this repository and submit pull requests.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Acknowledgments

This project was created by Jitesh Anil Varade as an educational tool to understand file system internals.

---
