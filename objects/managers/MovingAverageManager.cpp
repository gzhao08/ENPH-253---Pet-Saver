#include "MovingAverageManager.h"

MovingAverageManager::MovingAverageManager(int size) {
  _size = size;
  _buffer = new int[_size];
  _index = 0; // Index of oldest datum
  _count = 0; // How many datum in buffer
  _sum = 0;
}

MovingAverageManager::~MovingAverageManager() {
  delete[] _buffer;
}

void MovingAverageManager::add(int value) {
  // Subtract the value that is being overwritten
  if (_count == _size) {
    _sum -= _buffer[_index];
  }

  // Overwrite buffer
  _buffer[_index] = value;
  _sum += value;

  // Shift index
  _index = (_index + 1) % _size;
  if (_count < _size) {
    _count++;
  }
}

int MovingAverageManager::getAverage() const {
  if (_count == 0) return 0;
  return _sum / _count;
}

void MovingAverageManager::reset() {
  _index = 0;
  _count = 0;
  _sum = 0;
  for (int i = 0; i < _size; i++) {
    _buffer[i] = 0;
  }
}