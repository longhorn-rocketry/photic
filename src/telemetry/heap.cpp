#include "../core/core.h"
#include "arduino_heap.h"
#include "heap.h"

using namespace photonic;

HeapBlock::HeapBlock(unsigned int addr_start, unsigned int addr_end,
		HeapIO *io) {
	this->addr_start = addr_start;
	this->addr_end = addr_end;
	this->io = io;
	index = 0;
}

bool HeapBlock::write(unsigned int addr, byte b) {
	return io->write(addr, b);
}

byte HeapBlock::read(unsigned int addr) {
	return io->read(addr);
}

bool HeapBlock::push(byte b) {
	unsigned int addr_target = addr_start + index;
	if (addr_target < addr_end) {
		write(addr_target, b);
		index++;
		return true;
	} else
		return false;
}

byte HeapBlock::pop() {
	if (index > 0) {
		index--;
		return read(addr_start + index);
	} else
		return 0;
}

unsigned int HeapBlock::get_start() {
	return addr_start;
}

unsigned int HeapBlock::get_end() {
	return addr_end;
}

bool TelemetryHeap::bad_block_id(unsigned int id) {
	return id >= blocks.size();
}

bool TelemetryHeap::bad_heap() {
	return io == nullptr || addr_end - addr_start <= 0;
}

TelemetryHeap::TelemetryHeap(unsigned int addr_start, unsigned int addr_end,
		HeapIO *io) {
	this->addr_start = addr_start;
	this->addr_end = addr_end;
	this->io = io;
}

TelemetryHeap::TelemetryHeap() {
	auto_configure();
}

void TelemetryHeap::auto_configure() {
	switch(__rocket_microcontroller) {
		case TEENSY_36:
			addr_start = 0;
			addr_end = 4096;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_35:
			addr_start = 0;
			addr_end = 4096;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_32:
			addr_start = 0;
			addr_end = 2048;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_31:
			addr_start = 0;
			addr_end = 2048;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_30:
			addr_start = 0;
			addr_end = 2048;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_LC:
			addr_start = 0;
			addr_end = 128;
			io = new ArduinoHeapIO();
			break;

		case TEENSYPP_20:
			addr_start = 0;
			addr_end = 4096;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_20:
			addr_start = 0;
			addr_end = 1024;
			io = new ArduinoHeapIO();
			break;

		case TEENSYPP_10:
			addr_start = 0;
			addr_end = 2048;
			io = new ArduinoHeapIO();
			break;

		case TEENSY_10:
			addr_start = 0;
			addr_end = 512;
			io = new ArduinoHeapIO();
			break;

		default:
			addr_start = addr_end = 0;
			io = nullptr;
	}
}

TelemetryHeap::~TelemetryHeap() {
	if (io != nullptr)
		delete io;
	for (unsigned int i = 0; i < blocks.size(); i++)
		delete blocks[i];
}

unsigned int TelemetryHeap::add_block(unsigned int addr_start,
		unsigned int addr_end) {
	if (bad_heap())
		return -1;

	blocks.push_back(new HeapBlock(addr_start, addr_end, io));
	return blocks.size() - 1;
}

unsigned int TelemetryHeap::add_block(unsigned int size) {
	if (bad_heap())
		return -1;

	int addr_last = blocks.size() == 0 ? 0 : blocks[blocks.size() - 1]->get_end();
	blocks.push_back(new HeapBlock(addr_last, addr_last + size, io));
	return blocks.size() - 1;
}

void* TelemetryHeap::read_block(unsigned int block_id) {
	if (bad_block_id(block_id))
		return nullptr;

	HeapBlock *block = blocks[block_id];
 	unsigned int block_start = block->get_start(), block_end = block->get_end();
	byte *arr = new byte[block_end - block_start];
	for (unsigned int i = block_start; i < block_end; i++)
		arr[i - block_start] = block->read(i);
	return (void*)arr;
}

void* TelemetryHeap::read_block(unsigned int addr_start,
		unsigned int addr_end) {
	if (bad_heap())
		return nullptr;

	byte *arr = new byte[addr_end - addr_start];
	for (unsigned int i = addr_start; i < addr_end; i++)
		arr[i - addr_start] = io->read(i);
	return (void*)arr;
}

float* TelemetryHeap::decompress(unsigned int block_id) {
	if (bad_block_id(block_id))
		return nullptr;

	HeapBlock *block = blocks[block_id];
 	unsigned int block_start = block->get_start(), block_end = block->get_end();
	float *arr = new float[(block_end - block_start) / 2];
	for (unsigned int i = block_start; i < block_end; i += 2) {
		byte b1 = block->read(i), b0 = block->read(i + 1);
		float16 f16 = (b1 << 8) | b0;
		float f = Float16Compressor::decompress(f16);
		arr[(i - block_start) / 2] = f;
	}
	return arr;
}

float* TelemetryHeap::decompress(unsigned int addr_start,
		unsigned int addr_end) {
	if (bad_heap())
		return nullptr;

	float *arr = new float[(addr_end - addr_start) / 2];
	for (unsigned int i = addr_start; i < addr_end; i += 2) {
		byte b1 = io->read(i), b0 = io->read(i + 1);
		float16 f16 = (b1 << 8) | b0;
		float f = Float16Compressor::decompress(f16);
		arr[(i - addr_start) / 2] = f;
	}
	return arr;
}

bool TelemetryHeap::log(unsigned int block_id, byte b) {
	return blocks[block_id]->push(b);
}

bool TelemetryHeap::log(unsigned int block_id, short s) {
	byte b0 = s & 0xFF, b1 = (s >> 8) & 0xFF;
	HeapBlock *block = blocks[block_id];
	return block->push(b1) && block->push(b0);
}

bool TelemetryHeap::log(unsigned int block_id, int i) {
	byte b0 = i & 0xFF, b1 = (i >> 8) & 0xFF, b2 = (i >> 16) & 0xFF,
			b3 = (i >> 24) & 0xFF;
	HeapBlock *block = blocks[block_id];
	return block->push(b3) && block->push(b2) && block->push(b1) &&
			block->push(b0);
}

bool TelemetryHeap::log(unsigned int block_id, long long l) {
	byte b0 = l & 0xFF, b1 = (l >> 8) & 0xFF, b2 = (l >> 16) & 0xFF,
			b3 = (l >> 24) & 0xFF, b4 = (l >> 32) & 0xFF, b5 = (l >> 40) & 0xFF,
			b6 = (l >> 48) & 0xFF, b7 = (l >> 56);
	HeapBlock *block = blocks[block_id];
	return block->push(b7) && block->push(b6) && block->push(b5) &&
			block->push(b4) && block->push(b3) && block->push(b2) &&
			block->push(b1) && block->push(b0);
}

bool TelemetryHeap::log(unsigned int block_id, float16 f16) {
	TelemetryTypes types;
	types.f16 = f16;
	return log(block_id, types.s);
}

bool TelemetryHeap::log(unsigned int block_id, float f) {
	TelemetryTypes types;
	types.f = f;
	return log(block_id, types.i);
}

bool TelemetryHeap::log(unsigned int block_id, double d) {
	TelemetryTypes types;
	types.d = d;
	return log(block_id, types.l);
}

bool TelemetryHeap::logc(unsigned int block_id, float f) {
	float16 f16 = Float16Compressor::compress(f);
	return log(block_id, f16);
}

unsigned int TelemetryHeap::get_block_size(unsigned int block_id) {
	HeapBlock *block = blocks[block_id];
	return block->get_end() - block->get_start();
}
