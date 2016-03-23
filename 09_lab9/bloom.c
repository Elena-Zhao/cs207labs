//
//  bloom.c
//  
//
//  Created by Qing Zhao on 2/14/16.
//
//

#include "bloom.h"

//Bloom.c
//Global Constants
static uint64_t HASH1_INITIAL_VALUE =  5381;
static uint64_t HASH1_M = 33;
static uint64_t HASH2_INITIAL_VALUE =  0;
static uint64_t HASH2_M = 31;

void set_bit(bloom_filter_t *B, index_t i)
{
    uint64_t array_position = i/64;
    uint64_t bit_position = i%64;
    
    
    if (array_position >= B->size)
    {
        printf("ERROR at %i\n",i);
    }
    
    //This is the bitstring to OR with the array
    uint64_t bitstring = 1;
    bitstring = bitstring << bit_position;
    
    B->table[array_position] = 	B->table[array_position] ^ bitstring;
}

index_t get_bit(bloom_filter_t *B, index_t i)
{
    uint64_t array_position = i/64;
    
    uint64_t bit_position = i%64;
    
    //This is the bitstring to AND with the array to get a single bit back
    uint64_t bitstring = 1;
    bitstring = bitstring << bit_position;
    
    bitstring = B->table[array_position] & bitstring;
    bitstring = bitstring >> bit_position;
    return bitstring;
}

//Reference: http://burtleburtle.net/bob/hash/integer.html

index_t hash1(bloom_filter_t *B, key_t a)
{
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a%B->size;
}

//Reference: http://stackoverflow.com/questions/6943493/hash-table-with-64-bit-values-as-key

index_t hash2(bloom_filter_t *B, key_t key)
{
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key%B->size;
}

//Constructor function of bloom filter
void bloom_init(bloom_filter_t *B, index_t size_in_bits)
{
    B->size = size_in_bits;
    B->count = 0;
    
    uint64_t arraysize = size_in_bits/64;
    
    if (size_in_bits%64 != 0)
    {
        arraysize++;
    }
    
    B->table = (index_t*)malloc(arraysize*sizeof(index_t));
    printf("Bloom Array with size %i has been initialised.\n", arraysize);
    
    int x;
    for (x = 0; x < arraysize; x++)
    {
        B->table[x] = 0;
    }
    
}

//Destructor function of bloom filter
void bloom_destroy(bloom_filter_t *B)
{
    free(B->table);
    printf("Bloom Array Destroyed.\n");

}

//Add a key into bloom_filter_t; produce and set N_HASHES number of bits in the table for k
void bloom_add(bloom_filter_t *B, key_t k)
{
    int x = 0;
    
    for (x=0; x<N_HASHES; x++)
    {
        int insert_position =  (hash1(B,k) + x*hash2(B,k))%B->size;
        set_bit(B,insert_position);
    }
}

//check for a key; check N_HASHES number of bits in the table for k
int bloom_check(bloom_filter_t *B, key_t k)
{
    int x = 0;
    int results = 0;
    for (x=0; x<N_HASHES; x++)
    {
        int insert_position =  (hash1(B,k) + x*hash2(B,k))%B->size;
        results += get_bit(B,insert_position);
    }	
    return results;
}

//Testing File
void get_random(int* array, int size, int modulus)
{
    int x;
    for (x = 0; x< size; x++)
    {
        array[x] = rand() % modulus;
    }
}

void tradeoff_eval(int* array1, int* array2, int arraysize)
{
    //First, have the function create a new Bloom filter with 1000 elements and
    int bloomsize = 1000;
    bloom_filter_t bloomfilter;
    bloom_init(&bloomfilter, bloomsize);
    int x = 0;
    
    //add all the elements of the first input array to it.
    for (x= 0; x< arraysize; x++)
    {
        bloom_add(&bloomfilter, array1[x]);
    }
    
    //Second, create a loop that counts the number of bits,
    int totalbits = 0;
    for (x = 0; x< bloomsize; x++)
    {
        totalbits += get_bit(&bloomfilter, x);
    }
    printf("Total bits set (occupancy): %i\n",totalbits);
    
    int array2bits = 0;
    //Finally, create a loop that checks whether the numbers in the second array are in the table.
    for (x = 0; x< arraysize; x++)
    {
        array2bits += bloom_check(&bloomfilter, array2[x]);
    }
    printf("Array2 bits set (false positive): %i\n",array2bits);
    
    bloom_destroy(&bloomfilter);
    
}

int main()
{
    //1. Evaluating Hash Functions
    int bloomsize = 100;
    int x;
    bloom_filter_t bloomfilter;
    bloom_init(&bloomfilter, bloomsize);
    
    
    printf ("Hash1: %i %i %i %i %i %i\n",hash1(&bloomfilter, 0),hash1(&bloomfilter, 1),
            hash1(&bloomfilter, 2),hash1(&bloomfilter, 3),hash1(&bloomfilter, 13),
            hash1(&bloomfilter, 97));
    
    printf ("Hash2: %i %i %i %i %i %i\n",hash2(&bloomfilter, 0),hash2(&bloomfilter, 1),
            hash2(&bloomfilter, 2),hash2(&bloomfilter, 3),hash2(&bloomfilter, 13),
            hash2(&bloomfilter, 97));
    
    bloom_destroy(&bloomfilter);
    
    //2. Smoke test
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
    
    //3. Understanding the trade-offs
    printf("\nDoing N_HASHES Test.\n");
    
    int array1[100];
    int array2[100];
    get_random(array1, 100, 1000000);
    get_random(array2, 100, 1000000);
    tradeoff_eval(array1, array2, 100);
    
}
