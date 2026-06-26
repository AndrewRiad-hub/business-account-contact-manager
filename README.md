# Business Account & Contact Manager (BACM System)

A C++20 console application for managing business accounts and their contacts. BACM System stores account and contact data in CSV files, supports add/edit/delete workflows, provides paginated viewing, sorts accounts alphabetically with a binary search tree, and includes an undo stack for recent add/delete actions.

## GitHub repo description

Console-based C++ account and contact manager with CSV persistence, hash-table lookup, BST sorting, pagination, undo support, and benchmark testing.

## Features

- Add, edit, delete, and open business accounts
- Add, edit, and delete contacts under each account
- Save and reload data through `accounts.csv` and `contacts.csv`
- Paginated display for larger account/contact lists
- Alphabetical account display using a binary search tree
- Fast account lookup using an unordered hash map
- Undo support for recent account/contact add and delete actions
- Benchmark mode comparing linear search, hash lookup, and BST traversal
- U.S. state-code validation for address input

## Tech stack

- Language: C++20
- Interface: Console/terminal menu
- Data storage: CSV files
- Project support: Visual Studio project files, CMake, and Makefile

## Repository structure

```text
.
├── Account.*                  # Account model and account display helpers
├── AccountManager.*           # Main application controller and CSV persistence
├── Address.*                  # Address input/display logic
├── Contact.*                  # Contact model and display helpers
├── CompanyBST.*               # Binary search tree for company-name ordering
├── CsvUtils.*                 # CSV escaping and parsing helpers
├── DateTimeUtils.*            # Timestamp helper
├── InputUtils.*               # Console input helpers
├── StateValidator.*           # U.S. state-code validation
├── BACM System.cpp            # Main menu and entry point
├── CMakeLists.txt             # Cross-platform CMake build config
├── Makefile                   # Simple g++ build helper
└── sample_data/               # Optional sample CSV files
```

## Getting started

### Option 1: Build with Visual Studio

1. Open `BACM System.slnx` or `BACM System.vcxproj` in Visual Studio.
2. Select `Debug` or `Release` and `x64`.
3. Build and run the project.

### Option 2: Build with CMake

```bash
cmake -S . -B build
cmake --build build
./build/bacm_system
```

On Windows, the executable may be located under a configuration folder, such as `build/Debug/bacm_system.exe`.

### Option 3: Build with Make/g++

```bash
make
./bacm_system
```

Or compile manually:

```bash
g++ -std=c++20 -Wall -Wextra -pedantic Account.cpp AccountManager.cpp Address.cpp CompanyBST.cpp Contact.cpp CsvUtils.cpp DateTimeUtils.cpp InputUtils.cpp StateValidator.cpp "BACM System.cpp" -o bacm_system
```

## CSV data files

The app reads and writes two runtime files in the project folder:

- `accounts.csv`
- `contacts.csv`

These files are ignored by Git because they can contain personal or customer data. On first run, the program can start empty and create them when data is saved.

To test with sample data:

```bash
cp sample_data/accounts.example.csv accounts.csv
cp sample_data/contacts.example.csv contacts.csv
```

Then run the program and choose menu option `13` to reload CSV files, or restart the program.

## Main menu options

```text
1.  Add account
2.  Delete account
3.  Open account by ID
4.  Edit account company/address fields
5.  Add contact to account
6.  Delete contact from account
7.  Edit contact in account
8.  Display all accounts
9.  Display accounts sorted by company name using BST
10. Undo last add/delete action using stack
11. Run benchmark test
12. Manually save CSV files
13. Reload CSV files
0.  Exit
```

## GitHub topics

Suggested topics:

```text
cpp cpp20 console-application csv data-structures binary-search-tree hash-table stack account-management contact-management visual-studio cmake
```

## Notes

- Do not commit `.vs/`, `x64/`, `.obj`, `.pdb`, `.exe`, or generated CSV files.
- The original Visual Studio cache/build files were intentionally removed from the GitHub-ready version.
- The Visual Studio project file was cleaned to remove a stale reference to `QuickSortUtils.h`, which was not present in the uploaded project.

## License

This project is licensed under the MIT License. See `LICENSE` for details.
