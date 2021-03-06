// contains constants for ece486 pdp8 project
#pragma once

namespace pdp8
{
	// general constants
	const bool DEBUG = 0;           // 1 for on, 0 for off
	const int REGISTERSIZE = 12;

	// memory constants
	const int PAGESIZE = 5;
	const int LINESIZE = 7;

	// BitTwiddle class constants
	const bool IO_verbose = 0;
    #define rotatebit_DEBUG

	// parser constants
	const int OFFSET_SIZE = 7;      // size of offset used for bitset
	const int MAX_NUMBERS = 3;      // number of numbers to find per line
	const int NUM_OPCODE_BITS = 3;  // number of bits used for opcode

	// main file constants
	const int NUM_PAGES = 32;          // number of pages in memory
	const int PAGE_CAPACITY = 128;     // number of addresses per page
	const int STARTING_ADDRESS = 128;  // where program is stored
}

