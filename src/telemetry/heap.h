#ifndef PHOTONIC_TELEMETRY_HEAP_H
#define PHOTONIC_TELEMETRY_HEAP_H

#include "telemetry/float16.h"

#include <vector>

namespace photonic
{

typedef unsigned char byte;
typedef unsigned int uint;

/**
	All valid telemetry data types. Used by TelemetryHeap for type
	reinterpretation.
*/
union TelemetryTypes {
	byte b;
	short s;
	int i;
	long long l;
	float16 f16;
	float f;
	double d;
};

/**
	Abstraction of a memory handler; reads and writes data on the byte level to
	storage of some kind. See telemetry/teensy_heap.h for an example
	implementation.
*/
class HeapIO {
public:
	/**
		@brief tears down the handler
	*/
	virtual ~HeapIO() = default;

	/**
		Writes a single byte to an address in memory.

		@param addr address
		@param b byte to write
		@return if writing succeeded
	*/
	virtual bool write(int addr, byte b) = 0;

	/**
		Reads a single byte from an address in memory.

		@param addr address
		@return byte at addr
	*/
	virtual byte read(int addr) = 0;
};

/**
	A byte-addressable block of memory in a telemetry heap.
*/
class HeapBlock {
protected:
	HeapIO *io;
	unsigned int addr_start, addr_end, index;

public:
	/**
		Creates an empty block.

		@param addr_start block start address
		@param addr_end block end address
		@param io memory handler
	*/
	HeapBlock(unsigned int addr_start, unsigned int addr_end, HeapIO *io);

	/**
		@brief writes byte b to address addr; addr should be on [this->addr_start,
			     this->addr_end)
	*/
	bool write(unsigned int addr, byte b);

	/**
		@brief reads byte from address addr; addr should be on [this->addr_start,
			     this->addr_end)
	*/
	byte read(unsigned int addr);

	/**
		@brief pushes byte b on top of the last pushed byte as though the block
		       were a stack
	*/
	bool push(byte b);

	/**
		@brief pops and removes the last pushed byte as though the block were a
		       stack
	*/
	byte pop();

	/**
		@brief gets the start address of the block
	*/
	unsigned int get_start();

	/**
		@brief gets the end address of the block
	*/
	unsigned int get_end();
};

/**
	A collection of blocks identified by ID. Ideally, only one of these is created
	per flight computer. Individual blocks represent different telemetry
	distinctions, e.g. pressure logs, altitude logs, etc.

	Telemetry is stored in Little Endian. Best practice sees all block start and
	end addresses as multiples of 2.
*/
class TelemetryHeap {
protected:
	HeapIO *io;
	unsigned int addr_start, addr_end;
	std::vector<HeapBlock*> blocks;

	bool bad_block_id(unsigned int id);

	bool bad_heap();

public:
	/**
		Creates an empty heap.

		@param addr_start smallest heap address
		@param addr_end largest heap address
		@param io memory handler
	*/
	TelemetryHeap(unsigned int addr_start, unsigned int addr_end, HeapIO *io);

	/**
		Automatically configures the heap based on microcontroller model specified
		via photonic_configure.
	*/
	TelemetryHeap();

	/**
		@brief tears down the heap by deallocating the memory handler and all
		       added heap blocks
	*/
	~TelemetryHeap();

	/**
		@brief automatically configures the heap based on the microcontroller
		       specified through photonic_configure
	*/
	void auto_configure();

	/**
		Defines a region of the heap as a new block, and return its ID.

		@param addr_start block start address
		@param addr_end block end address
		@return block ID
	*/
	unsigned int add_block(unsigned int addr_start, unsigned int addr_end);

	/**
		@brief adds a new block immediately after the previous block
	*/
	unsigned int add_block(unsigned int size);

	/**
		Retrieves all data stored within a block. Client is responsible for type
		interpretation and deallocation of the generated array. Array will be of
		length get_block_size(block_id) bytes, and contain
		get_block_size(block_id) / sizeof(T) elements of a given type T.

		@param block_id block ID
		@return pointer to array of retrieved data
	*/
	void* read_block(unsigned int block_id);

	/**
		@brief retrieves data stored within a memory range
	*/
	void* read_block(unsigned int addr_start, unsigned int addr_end);

	/**
		@brief decompresses a block of short floats into native floats
	*/
	float* decompress(unsigned int block_id);

	/**
		@brief retrieves compressed floats stored within a memory range
	*/
	float* decompress(unsigned int addr_start, unsigned int addr_end);

	/**
		@brief pushes a byte onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, byte b);

	/**
		@brief pushes a short onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, short s);

	/**
		@brief pushes an int onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, int i);

	/**
		@brief pushes a long onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, long long l);

	/**
		@brief pushes a float16 onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, float16 f16);

	/**
		@brief pushes a float onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, float f);

	/**
		@brief pushes a double onto a block and returns if the operation succeeded
	*/
	bool log(unsigned int block_id, double d);

	/**
		@brief compresses a float into a float16 and pushes it onto a block and
		       returns if the operation succeeded
	*/
	bool logc(unsigned int block_id, float f);

	/**
		@briefs gets the size of a block in bytes
	*/
	unsigned int get_block_size(unsigned int block_id);
};

}; // end namespace photonic

#endif
