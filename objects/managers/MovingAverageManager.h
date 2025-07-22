#pragma once

// Local Libraries:

class MovingAverageManager {
    public:
        MovingAverageManager(int size);
        ~MovingAverageManager();
        void add(int value);
        int getAverage() const;
        void reset();

    private:
        int _size;
        int *_buffer;
        int _index;
        int _count;
        int _sum;
};