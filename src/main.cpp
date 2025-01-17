/**
 * @file      main.cpp
 * @brief     Stream compaction test program
 * @authors   Kai Ninomiya
 * @date      2015
 * @copyright University of Pennsylvania
 */

#include <cstdio>
#include <stream_compaction/cpu.h>
#include <stream_compaction/naive.h>
#include <stream_compaction/efficient.h>
#include <stream_compaction/thrust.h>
#include "testing_helpers.hpp"

const int SIZE = 4194304; // feel free to change the size of array
const int NPOT = SIZE - 3; // Non-Power-Of-Two
int *a = new int[SIZE];
int *b = new int[SIZE];
int *c = new int[SIZE];
int *d = new int[SIZE];

int main(int argc, char* argv[]) {
    // Scan tests

    printf("\n");
    printf("****************\n");
    printf("** SCAN TESTS **\n");
    printf("****************\n");

    genArray(SIZE - 1, a, 50, 1);  // Leave a 0 at the end to test that edge case
    a[SIZE - 1] = 0;
    printArray(SIZE, a, true);

    // initialize b using StreamCompaction::CPU::scan you implement
    // We use b for further comparison. Make sure your StreamCompaction::CPU::scan is correct.
    // At first all cases passed because b && c are all zeroes.
    zeroArray(SIZE, b);
    printDesc("cpu scan, power-of-two");
    StreamCompaction::CPU::scan(SIZE, b, a);
    printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
    printArray(SIZE, b, true);

    zeroArray(SIZE, c);
    printDesc("cpu scan, non-power-of-two");
    StreamCompaction::CPU::scan(NPOT, c, a);
    printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
    printArray(NPOT, b, true);
    printCmpResult(NPOT, b, c);

    zeroArray(SIZE, c);
    printDesc("naive scan, power-of-two");
    StreamCompaction::Naive::scan(SIZE, c, a);
    printElapsedTime(StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(SIZE, c, true);
    printCmpResult(SIZE, b, c);

    /* For bug-finding only: Array of 1s to help find bugs in stream compaction or scan
    onesArray(SIZE, c);
    printDesc("1s array for finding bugs");
    StreamCompaction::Naive::scan(SIZE, c, a);
    printArray(SIZE, c, true); */

    zeroArray(SIZE, c);
    printDesc("naive scan, non-power-of-two");
    StreamCompaction::Naive::scan(NPOT, c, a);
    printElapsedTime(StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(SIZE, c, true);
    printCmpResult(NPOT, b, c);

    //zeroArray(SIZE, c);
    printDesc("work-efficient scan, power-of-two");
    StreamCompaction::Efficient::scan(SIZE, c, a);
    printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");

    //for (int i = 0; i < 512; i++) {
    //    std::cout << a[i] << " ";
    //}
    //std::cout << std::endl;
    //std::cout << std::endl;
    //
    //for (int i = 0; i < 512; i++) {
    //    std::cout << c[i] << " ";
    //}
    //std::cout << std::endl;   
    //std::cout << std::endl;
    // 
    //printArray(SIZE, c, true);
    printCmpResult(SIZE, b, c);

    zeroArray(SIZE, c);
    printDesc("work-efficient scan, non-power-of-two");
    StreamCompaction::Efficient::scan(NPOT, c, a);
    printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(NPOT, c, true);
    printCmpResult(NPOT, b, c);

    zeroArray(SIZE, c);
    printDesc("thrust scan, power-of-two");
    StreamCompaction::Thrust::scan(SIZE, c, a);
    printElapsedTime(StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(SIZE, c, true);
    printCmpResult(SIZE, b, c);

    zeroArray(SIZE, c);
    printDesc("thrust scan, non-power-of-two");
    StreamCompaction::Thrust::scan(NPOT, c, a);
    printElapsedTime(StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(NPOT, c, true);
    printCmpResult(NPOT, b, c);

    printf("\n");
    printf("*****************************\n");
    printf("** STREAM COMPACTION TESTS **\n");
    printf("*****************************\n");

    // Compaction tests

    genArray(SIZE - 1, a, 4, 1);  // Leave a 0 at the end to test that edge case
    a[SIZE - 1] = 0;
    printArray(SIZE, a, true);

    int count, expectedCount, expectedNPOT;

    // initialize b using StreamCompaction::CPU::compactWithoutScan you implement
    // We use b for further comparison. Make sure your StreamCompaction::CPU::compactWithoutScan is correct.
    zeroArray(SIZE, b);
    printDesc("cpu compact without scan, power-of-two");
    count = StreamCompaction::CPU::compactWithoutScan(SIZE, b, a);
    printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
    expectedCount = count;
    printArray(count, b, true);
    printCmpLenResult(count, expectedCount, b, b);

    zeroArray(SIZE, c);
    printDesc("cpu compact without scan, non-power-of-two");
    count = StreamCompaction::CPU::compactWithoutScan(NPOT, c, a);
    printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
    expectedNPOT = count;
    printArray(count, c, true);
    printCmpLenResult(count, expectedNPOT, b, c);

    zeroArray(SIZE, c);
    printDesc("cpu compact with scan, power-of-two");
    count = StreamCompaction::CPU::compactWithScan(SIZE, c, a);
    printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
    //printArray(count, c, true);
    printCmpLenResult(count, expectedCount, b, c);

    zeroArray(SIZE, c);
    printDesc("cpu compact with scan, non-power-of-two");
    count = StreamCompaction::CPU::compactWithScan(NPOT, c, a);
    printElapsedTime(StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation(), "(std::chrono Measured)");
    //printArray(count, c, true);
    printCmpLenResult(count, expectedNPOT, b, c);

    zeroArray(SIZE, c);
    printDesc("work-efficient compact, power-of-two");
    count = StreamCompaction::Efficient::compact(SIZE, c, a);
    printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(count, c, true);
    printCmpLenResult(count, expectedCount, b, c);

    zeroArray(SIZE, c);
    printDesc("work-efficient compact, non-power-of-two");
    count = StreamCompaction::Efficient::compact(NPOT, c, a);
    printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(count, c, true);
    printCmpLenResult(count, expectedNPOT, b, c);


    printf("\n");
    printf("**********************\n");
    printf("** RADIX SORT TESTS **\n");
    printf("**********************\n");

    genArray(SIZE - 1, a, 50, 1);  // Leave a 0 at the end to test that edge case
    a[SIZE - 1] = 0;
    printArray(SIZE, a, true);

    zeroArray(SIZE, b);
    printDesc("thrust sort, power-of-two");
    StreamCompaction::Thrust::sort(SIZE, b, a);
    printElapsedTime(StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    printArray(SIZE, b, true);

    zeroArray(SIZE, d);
    printDesc("thrust sort, non-power-of-two");
    StreamCompaction::Thrust::sort(NPOT, d, a);
    printElapsedTime(StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    printArray(NPOT, d, true);

    zeroArray(SIZE, c);
    printDesc("radix sort, power-of-two");
    StreamCompaction::Efficient::radixSort(SIZE, c, a);
    printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(SIZE, c, true);
    printCmpResult(SIZE, b, c);

    //for (int i = 0; i < 64; i++) {
    //    std::cout << a[i] << " ";
    //}
    //std::cout << std::endl;
    //for (int i = 0; i < 64; i++) {
    //    std::cout << c[i] << " ";
    //}
    //std::cout << std::endl;

    zeroArray(SIZE, c);
    printDesc("radix sort, non-power-of-two");
    StreamCompaction::Efficient::radixSort(NPOT, c, a);
    printElapsedTime(StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation(), "(CUDA Measured)");
    //printArray(NPOT, c, true);
    printCmpResult(NPOT, d, c);

    //genArray(SIZE - 1, a, 50, 1);
    //a[SIZE - 1] = 0;
    //zeroArray(SIZE, c);
    //StreamCompaction::CPU::scan(SIZE, c, a);
    //std::cout << StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Thrust::scan(SIZE, c, a);
    //std::cout << StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Naive::scan(SIZE, c, a);
    //std::cout << StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Efficient::scan(SIZE, c, a);
    //std::cout << StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;

    //std::cout << std::endl;
    //std::cout << std::endl;

    //genArray(SIZE - 1, a, 50, 1);
    //a[SIZE - 1] = 0;
    //zeroArray(SIZE, c);
    //StreamCompaction::Efficient::radixSort(SIZE, c, a);
    //std::cout << StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Thrust::sort(SIZE, c, a);
    //std::cout << StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;

    //std::cout << std::endl;
    //std::cout << std::endl;

    //genArray(SIZE - 1, a, 4, 1);  // Leave a 0 at the end to test that edge case
    //a[SIZE - 1] = 0;
    //zeroArray(SIZE, c);
    //StreamCompaction::CPU::compactWithoutScan(SIZE, c, a);
    //std::cout << StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::CPU::compactWithScan(SIZE, c, a);
    //std::cout << StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Efficient::compact(SIZE, c, a);
    //std::cout << StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;

    //std::cout << std::endl;
    //std::cout << "Non power of two." << std::endl;    genArray(SIZE - 1, a, 50, 1);
    //std::cout << std::endl;

    //genArray(SIZE - 1, a, 50, 1);
    //a[SIZE - 1] = 0;
    //zeroArray(SIZE, c);
    //StreamCompaction::CPU::scan(NPOT, c, a);
    //std::cout << StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Thrust::scan(NPOT, c, a);
    //std::cout << StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Naive::scan(NPOT, c, a);
    //std::cout << StreamCompaction::Naive::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Efficient::scan(NPOT, c, a);
    //std::cout << StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;

    //std::cout << std::endl;
    //std::cout << std::endl;

    //genArray(SIZE - 1, a, 50, 1);
    //a[SIZE - 1] = 0;
    //zeroArray(SIZE, c);
    //StreamCompaction::Efficient::radixSort(NPOT, c, a);
    //std::cout << StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Thrust::sort(NPOT, c, a);
    //std::cout << StreamCompaction::Thrust::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;

    //std::cout << std::endl;
    //std::cout << std::endl;

    //genArray(SIZE - 1, a, 4, 1);  // Leave a 0 at the end to test that edge case
    //a[SIZE - 1] = 0;
    //zeroArray(SIZE, c);
    //StreamCompaction::CPU::compactWithoutScan(NPOT, c, a);
    //std::cout << StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::CPU::compactWithScan(NPOT, c, a);
    //std::cout << StreamCompaction::CPU::timer().getCpuElapsedTimeForPreviousOperation() << std::endl;
    //zeroArray(SIZE, c);
    //StreamCompaction::Efficient::compact(NPOT, c, a);
    //std::cout << StreamCompaction::Efficient::timer().getGpuElapsedTimeForPreviousOperation() << std::endl;

    //system("pause"); // stop Win32 console from closing on exit
    //delete[] a;
    //delete[] b;
    //delete[] c;
    //delete[] d;
}
