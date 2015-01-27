PDP-8 Memory Simulation Code
Author: Meng Lei

Functions achieved:
1. Allocate page table with desired number of pages and capacity of page
2. Add entry, delete entry, retrieve entry

Things to work on:
1. Change linear linked lists to 2d array of pointers
2. Consider extremely large page table (using long instead of int)
3. What happens when trying to overwrite?
4. Display function (for the convenience of debugging)
5. Translated binary addressing fed into memory module, break it down
   in the form of page number + offset, restore to decimal

Things to test:
1. Load an entry
	1.1 Empty? Unoccupied?
	1.2 Correct EAddr? Correct value?
	1.3 What if addressing is completely wrong?
	1.4 What if address exceeds page table capacity?
2. Store
	2.1 Empty? Unoccupied?
	2.2 Correct EAddr? Correct value?
	2.3 What happens when trying to overwrite?
	2.4 What is address or value is completely wrong?
	2.5 What if address exceeds page table capacity?
	2.6 What if value exceeds PTE capacity?
3. Is page table completely deleted / unallocated after execution?