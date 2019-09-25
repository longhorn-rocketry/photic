/**
 * Structures for organizing onboard telemetry storage. In the interest of
 * speed and reduced STL dependencies, THERE IS NO BOUNDS CHECKING.
 */
#ifndef PHOTIC_TELEMETRY_HEAP_H
#define PHOTIC_TELEMETRY_HEAP_H

#include "float16.h"

#include <vector>

namespace photic
{

typedef unsigned char byte;
typedef unsigned int block_id_t;

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
 * Abstraction of a memory handler. Reads and writes data on the byte level to
 * storage of some kind.
 */
class HeapIO {
public:
	/**
	 * @brief Tears down the handler.
	 */
	virtual ~HeapIO() = default;

	/**
	 * @brief Writes a byte k_b to address k_addr and returns if successful.
	 */
	virtual bool write(int k_addr, byte k_b) = 0;

	/**
	 * @brief Reads the byte at address k_addr.
	 */
	virtual byte read(int k_addr) = 0;
};

/**
 * A byte-addressable block of memory in a telemetry heap.
 */
class HeapBlock {
protected:
	HeapIO* m_io;
	unsigned int m_addr_start, m_addr_end, m_index;

public:
	/**
	 * Creates an empty block.
	 * @param   k_addr_start start address
	 * @param   k_addr_end   end address
	 * @param   k_io         memory handler
	 */
	HeapBlock(unsigned int k_addr_start, unsigned int k_addr_end, HeapIO* k_io);

	bool write(unsigned int k_addr, byte k_b);

	byte read(unsigned int k_addr);

	/**
	 * @brief Pushes byte k_b on top of the last pushed byte as though the block
	 *        were a stack.
	 */
	bool push(byte k_b);

	/**
	 * @brief Pops and removes the last pushed byte as though the block were a
	 *	      stack.
	 */
	byte pop();

	/**
	 * @brief Gets the start address of the block.
	 */
	unsigned int get_start();

	/**
	 * @brief Gets the end address of the block.
	 */
	unsigned int get_end();
};

/**
 * A collection of heap blocks identified by ID. Ideally, only one of these is
 * created per storage device on the flight computer. Individual blocks likely
 * represent different telemetry sources, e.g. pressure, altitude, etc.
 *
 * Telemetry is stored in Little Endian. Best practice sees all block start and
 * end addresses as multiples of 2.
 */
class TelemetryHeap {
protected:
	HeapIO* m_io;
	unsigned int m_addr_start, m_addr_end;
	std::vector<HeapBlock*> m_blocks;

	bool bad_block_id(unsigned int k_id);

	bool bad_heap();

public:
	/**
	 * @brief Default constructor attempts to configure the IO automatically
	 *        based on ROCKET_MICROCONTROLLER_MODEL.
	 */
	TelemetryHeap();

	/**
	 * Creates an empty heap.
	 *
	 * @param   k_addr_start smallest heap address
	 * @param   k_addr_end   largest heap address
	 * @param   k_io         memory handler
	 */
	TelemetryHeap(unsigned int k_addr_start,
		            unsigned int k_addr_end,
								HeapIO *k_io);

	/**
	 * @brief Tears down the heap by deallocating the memory handler and all
	 *        added heap blocks.
	 */
	~TelemetryHeap();

	/**
	 * Defines a region of the heap as a new block and returns a UID for it.
	 *
	 * @param   k_addr_start block start address
	 * @param   k_addr_end   block end address
	 *
	 * @ret     block UID
	 */
	unsigned int add_block(unsigned int k_addr_start, unsigned int k_addr_end);

	/**
	 * @brief Adds a new block of size k_size immediately after the previous one.
	 */
	unsigned int add_block(unsigned int k_size);

	/**
	 * Reads all data stored within a block. Client is responsible for type
	 * interpretation and deallocating of the generated array. Array will be of
	 * length get_block_size(k_block_id) bytes, and contain
	 * get_block_size(k_block_id) / sizeof(T) elements of a given type T.
	 *
	 * @param k_block_id   UID of block to retrieve
	 */
	void* read_block(block_id_t k_block_id);

	/**
	 * @brief Retrieves data stored within a memory range.
	 */
	void* read_block(unsigned int k_addr_start, unsigned int k_addr_end);

	/**
	 * @brief Decompresses a block of float16s into float32s.
	 */
	float* decompress(block_id_t k_block_id);

	/**
	 * @brief Decompresses a memory range of float16s into float32s.
	 */
	float* decompress(unsigned int k_addr_start, unsigned int k_addr_end);

	/**
	 * @brief Gets the size of a block by ID in bytes.
	 */
	unsigned int get_block_size(block_id_t k_block_id);

	/**
	 * Methods for pushing various integral types onto a block.
	 */

	bool log(block_id_t k_block_id, byte k_b);

	bool log(block_id_t k_block_id, short k_s);

	bool log(block_id_t k_block_id, int k_i);

	bool log(block_id_t k_block_id, long long k_l);

	bool log(block_id_t k_block_id, float16 k_f16);

	bool log(block_id_t k_block_id, float k_f);

	bool log(block_id_t k_block_id, double k_d);

	/**
	 * @brief Identical to logging a float32 but it is compressed to a float16
	 *        before being pushed.
	 */
	bool logc(block_id_t k_block_id, float k_f);
};

} // end namespace photic

#endif
