BEAUTY & COSMETICS
Inventory, Search & Order Management System

CS201 – Data Structures and Algorithms
Final Project


STUDENT INFORMATION
-------------------

Name: Joy Mae Gade
Program: Bachelor of Science in Computer Engineering
Year and Section: 3A
Course: CS201 – Data Structures and Algorithms
Project Type: Final Project
Programming Language: C++


PROJECT DESCRIPTION
-------------------

BEAUTY & COSMETICS is a C++ console-based Inventory, Search & Order Management System designed to manage beauty and cosmetic products.

The system allows users to view available products, search and filter the inventory, manage shopping carts and wishlists, and process customer orders.

It also provides inventory management features such as adding, updating, and removing products, as well as undo and redo operations.

This project demonstrates the practical application of different data structures and algorithms discussed in the Data Structures and Algorithms course.


PROJECT OBJECTIVES
------------------

1. Apply different data structures in a real-world application.
2. Demonstrate searching and sorting algorithms using C++.
3. Develop a simple and organized inventory management system.
4. Implement product searching, filtering, and management.
5. Demonstrate how stacks and queues work.
6. Practice object-oriented programming and dynamic memory management.


SYSTEM FEATURES
---------------

1. Product Catalog
   - Displays the available beauty and cosmetic products.
   - Shows product ID, name, category, brand, price, and stock quantity.

2. Filter by Brand
   - Allows users to view products according to their brand.
   - Helps users find products from a specific brand.

3. Keyword Search
   - Searches products using keywords.
   - Makes finding a specific product easier.

4. Price Range Filter
   - Displays products within a selected price range.
   - Uses searching techniques to locate matching prices.

5. Shopping Cart
   - Allows users to add and manage products in their cart.
   - Uses a doubly linked list.

6. Checkout
   - Processes customer orders.
   - Uses a queue to manage checkout requests.

7. Admin Panel
   - Provides inventory management operations.
   - Allows supported product record changes.

8. Undo and Redo
   - Allows supported inventory changes to be undone and restored.
   - Uses stacks.

9. Partner Brand Showcase
   - Displays partner brands in a repeating sequence.
   - Uses a circular linked list.

10. Sort Inventory
    - Organizes products using available sorting options.
    - Demonstrates insertion sort and quick sort.


DATA STRUCTURES IMPLEMENTED
---------------------------

1. Dynamic Array
   - Stores the product inventory.
   - Automatically adjusts its capacity when needed.

2. Doubly Linked List
   - Used to manage shopping cart items.
   - Each node contains a link to the previous and next nodes.

3. Singly Linked List
   - Used to manage wishlist items.
   - Each node points to the next node.

4. Circular Linked List
   - Used for the partner brand showcase.
   - The last node connects back to the first node.

5. Stack
   - Used for undo and redo operations.
   - Follows the Last In, First Out (LIFO) principle.

6. Queue
   - Used for checkout processing.
   - Follows the First In, First Out (FIFO) principle.

7. Binary Search Tree
   - Used for searching products by their IDs.
   - Organizes data through left and right child nodes.


ALGORITHMS IMPLEMENTED
----------------------

1. Linear Search
   - Checks each product one by one until the target is found.

2. Binary Search
   - Searches sorted data by repeatedly dividing the search area.

3. Insertion Sort
   - Arranges elements by inserting each element into its correct position.

4. Quick Sort
   - Sorts elements by selecting a pivot and dividing the data into smaller groups.


TECHNOLOGIES AND TOOLS
----------------------

Programming Language: C++
Compiler: GCC (g++)
Code Editor: Visual Studio Code
Operating System: Windows
Terminal: Windows PowerShell
Version Control: Git and GitHub


PROJECT REQUIREMENTS
--------------------

Before running the program, make sure you have:

1. A Windows computer.
2. Visual Studio Code or another C++ code editor.
3. GCC C++ compiler installed.
4. A terminal such as Windows PowerShell.
5. The project source file named main.cpp.


PROJECT FOLDER STRUCTURE
------------------------

DSA FINAL PROJECT
|
|-- main.cpp
|-- main.exe
|-- README.md

Note:
main.exe is generated after compiling main.cpp. You do not need to create it manually.


HOW TO RUN THE PROGRAM
----------------------

METHOD 1: USING VS CODE INTEGRATED TERMINAL

Step 1. Open Visual Studio Code.

Step 2. Open your DSA FINAL PROJECT folder.

Step 3. Open the terminal by clicking:

Terminal > New Terminal

Step 4. Make sure the terminal is inside your project folder.

Step 5. Compile the program by typing:

g++ .\main.cpp -o main.exe

Step 6. Press Enter and wait for the compilation to finish.

Step 7. Run the program by typing:

.\main.exe

Step 8. Press Enter to start the system.


METHOD 2: USING A SEPARATE WINDOWS POWERSHELL

Step 1. Open Windows PowerShell.

Step 2. Navigate to your project folder by typing:

cd "C:\Users\Joy Mae\OneDrive\Documents\DSA FINAL PROJECT"

Step 3. Press Enter.

Step 4. Compile the program:

g++ main.cpp -o main.exe

Step 5. Press Enter and wait for compilation.

Step 6. Run the program:

.\main.exe

Step 7. Press Enter to start the system.


IMPORTANT COMMANDS
------------------

Compile the program:

g++ main.cpp -o main.exe

Run the program:

.\main.exe

Compile using the VS Code integrated terminal:

g++ .\main.cpp -o main.exe

Run using the VS Code integrated terminal:

.\main.exe


TROUBLESHOOTING
---------------

1. 'g++' is not recognized
   - Make sure GCC is installed.
   - Check if the GCC bin folder is included in your Windows PATH.
   - Restart VS Code or PowerShell after updating PATH.

2. Permission Denied
   - Make sure the program is not currently running.
   - Close the running program before compiling again.
   - If necessary, stop the process using:

   taskkill /F /IM main.exe

3. The program does not run
   - Make sure you are inside the correct project folder.
   - Check that main.cpp exists in the folder.
   - Compile the program again before running it.

4. Changes in the code do not appear
   - Save main.cpp.
   - Compile the program again.
   - Run the newly generated main.exe.


SAMPLE PRODUCT BRANDS
---------------------

The system includes sample beauty and cosmetic products from brands such as:

- Maybelline
- L'Oreal Paris
- Cetaphil
- Laneige


PROJECT FILES
-------------

main.cpp
Contains the complete C++ source code and system implementation.

main.exe
The executable file generated after compiling the source code.

README.md
Contains the project description, features, data structures, algorithms, requirements, and instructions for running the program.


IMPORTANT NOTES
---------------

- Always save changes to main.cpp before compiling.
- Compile the program again whenever the source code is updated.
- The main.exe file is generated automatically after successful compilation.
- Keep the project files organized inside the same folder.
- The program should be run from the project directory.


CONCLUSION
----------

BEAUTY & COSMETICS demonstrates how data structures and algorithms can be applied to an inventory, search, and order management system.

Through this project, different concepts such as dynamic arrays, linked lists, stacks, queues, binary search trees, searching algorithms, and sorting algorithms are applied to create an organized and functional C++ application.

This project also provides an opportunity to understand how data structures and algorithms can solve practical problems in software development.


DEVELOPER
---------

Joy Mae Gade
Bachelor of Science in Computer Engineering
3A

CS201 – Data Structures and Algorithms
Final Project