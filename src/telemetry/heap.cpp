#include "../core/core.h"
#include "arduino_heap.h"
#include "heap.h"

namespace photic {

HeapBlock::HeapBlock(unsigned int k_addr_start,
	                   unsigned int k_addr_end,
		                 HeapIO *k_io)
{
	m_addr_start = k_addr_start;
	m_addr_end = k_addr_end;
	m_io = k_io;
	m_index = 0;
}

bool HeapBlock::write(unsigned int k_addr, byte k_b) {
	return m_io->write(k_addr, k_b);
}

byte HeapBlock::read(unsigned int k_addr) {
	return m_io->read(k_addr);
}

bool HeapBlock::push(byte k_b) {
	unsigned int addr_target = m_addr_start + m_index;
	if (addr_target < m_addr_end) {
		write(addr_target, k_b);
		m_index++;
		return true;
	} else
		return false;
}

byte HeapBlock::pop() {
	if (m_index > 0) {
		m_index--;
		return read(m_addr_start + m_index);
	} else
		return 0;
}

unsigned int HeapBlock::get_start() {
	return m_addr_start;
}

unsigned int HeapBlock::get_end() {
	return m_addr_end;
}

bool TelemetryHeap::bad_block_id(unsigned int k_id) {
	return k_id >= m_blocks.size();
}

bool TelemetryHeap::bad_heap() {
	return m_io == nullptr || m_addr_end - m_addr_start <= 0;
}

TelemetryHeap::TelemetryHeap(unsigned int k_addr_start,
	                           unsigned int k_addr_end,
		                         HeapIO* k_io)
{
	m_addr_start = k_addr_start;
	m_addr_end = k_addr_end;
	m_io = k_io;
}

TelemetryHeap::TelemetryHeap() {
	switch(microcontroller_model) {
		case TEENSY_36:
			m_addr_start = 0;
			m_addr_end = 4096;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_35:
			m_addr_start = 0;
			m_addr_end = 4096;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_32:
			m_addr_start = 0;
			m_addr_end = 2048;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_31:
			m_addr_start = 0;
			m_addr_end = 2048;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_30:
			m_addr_start = 0;
			m_addr_end = 2048;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_LC:
			m_addr_start = 0;
			m_addr_end = 128;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSYPP_20:
			m_addr_start = 0;
			m_addr_end = 4096;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_20:
			m_addr_start = 0;
			m_addr_end = 1024;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSYPP_10:
			m_addr_start = 0;
			m_addr_end = 2048;
			m_io = new ArduinoHeapIO();
			break;

		case TEENSY_10:
			m_addr_start = 0;
			m_addr_end = 512;
			m_io = new ArduinoHeapIO();
			break;

		default:
			m_addr_start = m_addr_end = 0;
			m_io = nullptr;
	}
}

TelemetryHeap::~TelemetryHeap() {
	if (m_io != nullptr)
		delete m_io;
	for (unsigned int i = 0; i < m_blocks.size(); i++)
		delete m_blocks[i];
}

unsigned int TelemetryHeap::add_block(unsigned int k_addr_start,
	  	                                unsigned int k_addr_end)
{
	if (bad_heap())
		return -1;

	m_blocks.push_back(new HeapBlock(k_addr_start, k_addr_end, m_io));
	return m_blocks.size() - 1;
}

block_id_t TelemetryHeap::add_block(unsigned int k_size) {
	if (bad_heap())
		return -1;

	unsigned int addr_last = m_blocks.size() == 0 ? 0 :
	                         m_blocks[m_blocks.size() - 1]->get_end();
	m_blocks.push_back(new HeapBlock(addr_last, addr_last + k_size, m_io));
	return m_blocks.size() - 1;
}

void* TelemetryHeap::read_block(block_id_t k_block_id) {
	if (bad_block_id(k_block_id))
		return nullptr;

	HeapBlock* block = m_blocks[k_block_id];
 	unsigned int block_start = block->get_start(), block_end = block->get_end();
	byte* arr = new byte[block_end - block_start];

	for (unsigned int i = block_start; i < block_end; i++)
		arr[i - block_start] = block->read(i);

	return (void*)arr;
}

void* TelemetryHeap::read_block(unsigned int k_addr_start,
		                            unsigned int k_addr_end)
{
	if (bad_heap())
		return nullptr;

	byte* arr = new byte[k_addr_end - k_addr_start];
	for (unsigned int i = k_addr_start; i < k_addr_end; i++)
		arr[i - k_addr_start] = m_io->read(i);
	return (void*)arr;
}

float* TelemetryHeap::decompress(block_id_t k_block_id) {
	if (bad_block_id(k_block_id))
		return nullptr;

	HeapBlock* block = m_blocks[k_block_id];
 	unsigned int block_start = block->get_start(), block_end = block->get_end();
	float* arr = new float[(block_end - block_start) / 2];

	for (unsigned int i = block_start; i < block_end; i += 2) {
		byte b1 = block->read(i), b0 = block->read(i + 1);
		float16 f16 = (b1 << 8) | b0;
		float f = float16_util::decompress(f16);
		arr[(i - block_start) / 2] = f;
	}

	return arr;
}

float* TelemetryHeap::decompress(unsigned int k_addr_start,
		                             unsigned int k_addr_end)
{
	if (bad_heap())
		return nullptr;

	float* arr = new float[(k_addr_end - k_addr_start) / 2];

	for (unsigned int i = k_addr_start; i < k_addr_end; i += 2) {
		byte b1 = m_io->read(i);
		byte b0 = m_io->read(i + 1);
		float16 f16 = (b1 << 8) | b0;
		float f = float16_util::decompress(f16);
		arr[(i - k_addr_start) / 2] = f;
	}

	return arr;
}

bool TelemetryHeap::log(block_id_t k_block_id, byte k_b) {
	return m_blocks[k_block_id]->push(k_b);
}

bool TelemetryHeap::log(block_id_t k_block_id, short k_s) {
	byte b0 = k_s & 0xFF;
	byte b1 = (k_s >> 8) & 0xFF;
	HeapBlock* block = m_blocks[k_block_id];

	return block->push(b1) && block->push(b0);
}

bool TelemetryHeap::log(block_id_t k_block_id, int k_i) {
	byte b0 = k_i & 0xFF;
	byte b1 = (k_i >> 8) & 0xFF;
	byte b2 = (k_i >> 16) & 0xFF;
	byte b3 = (k_i >> 24) & 0xFF;
	HeapBlock* block = m_blocks[k_block_id];

	return block->push(b3) && block->push(b2) && block->push(b1) &&
			   block->push(b0);
}

bool TelemetryHeap::log(block_id_t k_block_id, long long l) {
	byte b0 = l & 0xFF;
	byte b1 = (l >> 8) & 0xFF;
	byte b2 = (l >> 16) & 0xFF;
	byte b3 = (l >> 24) & 0xFF;
	byte b4 = (l >> 32) & 0xFF;
	byte b5 = (l >> 40) & 0xFF;
	byte b6 = (l >> 48) & 0xFF;
	byte b7 = (l >> 56) & 0xFF;
	HeapBlock* block = m_blocks[k_block_id];

	return block->push(b7) && block->push(b6) && block->push(b5) &&
			   block->push(b4) && block->push(b3) && block->push(b2) &&
			   block->push(b1) && block->push(b0);
}

bool TelemetryHeap::log(block_id_t k_block_id, float16 k_f16) {
	TelemetryTypes types;
	types.f16 = k_f16;

	return log(k_block_id, types.s);
}

bool TelemetryHeap::log(block_id_t k_block_id, float k_f) {
	TelemetryTypes types;
	types.f = k_f;

	return log(k_block_id, types.i);
}

bool TelemetryHeap::log(block_id_t k_block_id, double k_d) {
	TelemetryTypes types;
	types.d = k_d;

	return log(k_block_id, types.l);
}

bool TelemetryHeap::logc(block_id_t k_block_id, float k_f) {
	float16 f16 = float16_util::compress(k_f);

	return log(k_block_id, f16);
}

unsigned int TelemetryHeap::get_block_size(block_id_t k_block_id) {
	HeapBlock* block = m_blocks[k_block_id];

	return block->get_end() - block->get_start();
}

} // end namespace photic
