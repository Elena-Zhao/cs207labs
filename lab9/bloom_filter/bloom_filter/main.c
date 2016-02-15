//
//  main.c
//  bloom_filter
//
//  Created by Elena Zhao on 2/14/16.
//  Copyright (c) 2016 Qing Zhao. All rights reserved.
//

#include <stdio.h>
#include "bloom.h"

void gen_rand(int* array, int size, int modulus)
{
    int x;
    for (x = 0; x< size; x++)
    {
        array[x] = rand() % modulus;
    }
}

void run_test3(int* array1, int* array2, int arraysize)
{
    int bloomsize = 1000;
    bloom_filter_t bloomfilter;
    bloom_init(&bloomfilter, bloomsize);
    int x = 0;
    
    //set the bits in bloomfilter based on array1
    for (x= 0; x< arraysize; x++)
    {
        bloom_add(&bloomfilter, array1[x]);
    }
    
    //First, count all the bits that are set
    int totalbits = 0;
    for (x = 0; x< bloomsize; x++)
    {
        totalbits += get_bit(&bloomfilter, x);
    }
    printf("Total bits set: %i\n",totalbits);
    
    int array2bits = 0;
    //Next, count all the bits in the second array that are set in bloomfiter
    for (x = 0; x< arraysize; x++)
    {
        array2bits += bloom_check(&bloomfilter, array2[x]);
    }
    printf("Array2 bits set: %i\n",array2bits);
    
    bloom_destroy(&bloomfilter);
    
}

int main()
{
    //Part 1. Evaluating Hash Functions
    int bloomsize = 100;
    int x;
    bloom_filter_t bloomfilter;
    bloom_init(&bloomfilter, bloomsize);
    
    
    printf ("Hash1: %llu %llu %llu %llu %llu %llu\n", hash1(&bloomfilter, 0), hash1(&bloomfilter, 1),
            hash1(&bloomfilter, 2),hash1(&bloomfilter, 3), hash1(&bloomfilter, 13),
             hash1(&bloomfilter, 97));
    
    printf ("Hash2: %llu %llu %llu %llu %llu %llu\n",hash2(&bloomfilter, 0), hash2(&bloomfilter, 1),
            hash2(&bloomfilter, 2),hash2(&bloomfilter, 3),hash2(&bloomfilter, 13),
            hash2(&bloomfilter, 97));
    
    bloom_destroy(&bloomfilter);
    
    //Part 2:
    printf("\nDoing Smoke Test.\n");
    bloomsize = 1000;
    bloom_init(&bloomfilter, bloomsize);
    
    
    for (x= 0; x< 70; x++)
    {
        bloom_add(&bloomfilter, x);
    }
    
    int totalbits = 0;
    for (x = 0; x< bloomsize; x++)
    {
        totalbits += get_bit(&bloomfilter, x);
    }
    printf("Total bits set: %i\n",totalbits);
    bloom_destroy(&bloomfilter);
    
    //Part 3
    printf("\nDoing N_HASHES Test.\n");
    
    int array1[100];
    int array2[100];
    gen_rand(array1, 100, 1000000);
    gen_rand(array2, 100, 1000000);
    run_test3(array1, array2, 100);
    
}


/*
 Results
 N_Hashes = 1
 Total bits set: 90
 Array2 bits set: 8
 N_HASHES = 2
 Total bits set: 168
 Array2 bits set: 24
 N_HASHES = 3
 Total bits set: 220
 Array2 bits set: 55
 N_HASHES = 4
 Total bits set: 246
 Array2 bits set: 87
 N_HASHES = 5
 Total bits set: 290
 Array2 bits set: 147
 */