# Reasons for Code to Crash

* Segmentation fault
    * Accessing out of bounds memory that the process doesn't have permission to see.
* Memory Corruption
    * Global
        * Global array overflow
    * Heap
        * Stack overflows
        * memory allocation failures
    * Stack
        * Heap overflows
        * overwrite a stack pointer.
* Corrupt object pointer
* Corrupt vtable pointer 
* Uncaught Exception
    * An exception has raised beyond the scope of the main function.
* Out of Resources - ulimit
    * Your process asks for memory that the OS can't allocate.
* Dynamic Memory Allocation failure
    * Check that new doesn't return NULL
