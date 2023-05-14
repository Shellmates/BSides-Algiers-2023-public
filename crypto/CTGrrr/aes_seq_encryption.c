#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BLOCK_SIZE 16
#define ROUNDS_NUMBER 10

#define PLAINTEXT_FILENAME "file"
#define CIPHERTEXT_FILENAME "enc"
#define KEY_FILENAME "key"
#define NONCE_FILENAME "nonce"
#define FLAG "shellmates{A3S_cTr_g03e5_BrrR_00}"

const unsigned char rcon[ROUNDS_NUMBER] =
    {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
    };

const unsigned char sbox[BLOCK_SIZE * BLOCK_SIZE] =
    {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

const unsigned char mixColMatrix[BLOCK_SIZE * BLOCK_SIZE][6] =
    {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, {0x02, 0x03, 0x09, 0x0b, 0x0d, 0x0e}, {0x04, 0x06, 0x12, 0x16, 0x1a, 0x1c},
        {0x06, 0x05, 0x1b, 0x1d, 0x17, 0x12}, {0x08, 0x0c, 0x24, 0x2c, 0x34, 0x38}, {0x0a, 0x0f, 0x2d, 0x27, 0x39, 0x36},
        {0x0c, 0x0a, 0x36, 0x3a, 0x2e, 0x24}, {0x0e, 0x09, 0x3f, 0x31, 0x23, 0x2a}, {0x10, 0x18, 0x48, 0x58, 0x68, 0x70},
        {0x12, 0x1b, 0x41, 0x53, 0x65, 0x7e}, {0x14, 0x1e, 0x5a, 0x4e, 0x72, 0x6c}, {0x16, 0x1d, 0x53, 0x45, 0x7f, 0x62},
        {0x18, 0x14, 0x6c, 0x74, 0x5c, 0x48}, {0x1a, 0x17, 0x65, 0x7f, 0x51, 0x46}, {0x1c, 0x12, 0x7e, 0x62, 0x46, 0x54},
        {0x1e, 0x11, 0x77, 0x69, 0x4b, 0x5a}, {0x20, 0x30, 0x90, 0xb0, 0xd0, 0xe0}, {0x22, 0x33, 0x99, 0xbb, 0xdd, 0xee},
        {0x24, 0x36, 0x82, 0xa6, 0xca, 0xfc}, {0x26, 0x35, 0x8b, 0xad, 0xc7, 0xf2}, {0x28, 0x3c, 0xb4, 0x9c, 0xe4, 0xd8},
        {0x2a, 0x3f, 0xbd, 0x97, 0xe9, 0xd6}, {0x2c, 0x3a, 0xa6, 0x8a, 0xfe, 0xc4}, {0x2e, 0x39, 0xaf, 0x81, 0xf3, 0xca},
        {0x30, 0x28, 0xd8, 0xe8, 0xb8, 0x90}, {0x32, 0x2b, 0xd1, 0xe3, 0xb5, 0x9e}, {0x34, 0x2e, 0xca, 0xfe, 0xa2, 0x8c},
        {0x36, 0x2d, 0xc3, 0xf5, 0xaf, 0x82}, {0x38, 0x24, 0xfc, 0xc4, 0x8c, 0xa8}, {0x3a, 0x27, 0xf5, 0xcf, 0x81, 0xa6},
        {0x3c, 0x22, 0xee, 0xd2, 0x96, 0xb4}, {0x3e, 0x21, 0xe7, 0xd9, 0x9b, 0xba}, {0x40, 0x60, 0x3b, 0x7b, 0xbb, 0xdb},
        {0x42, 0x63, 0x32, 0x70, 0xb6, 0xd5}, {0x44, 0x66, 0x29, 0x6d, 0xa1, 0xc7}, {0x46, 0x65, 0x20, 0x66, 0xac, 0xc9},
        {0x48, 0x6c, 0x1f, 0x57, 0x8f, 0xe3}, {0x4a, 0x6f, 0x16, 0x5c, 0x82, 0xed}, {0x4c, 0x6a, 0x0d, 0x41, 0x95, 0xff},
        {0x4e, 0x69, 0x04, 0x4a, 0x98, 0xf1}, {0x50, 0x78, 0x73, 0x23, 0xd3, 0xab}, {0x52, 0x7b, 0x7a, 0x28, 0xde, 0xa5},
        {0x54, 0x7e, 0x61, 0x35, 0xc9, 0xb7}, {0x56, 0x7d, 0x68, 0x3e, 0xc4, 0xb9}, {0x58, 0x74, 0x57, 0x0f, 0xe7, 0x93},
        {0x5a, 0x77, 0x5e, 0x04, 0xea, 0x9d}, {0x5c, 0x72, 0x45, 0x19, 0xfd, 0x8f}, {0x5e, 0x71, 0x4c, 0x12, 0xf0, 0x81},
        {0x60, 0x50, 0xab, 0xcb, 0x6b, 0x3b}, {0x62, 0x53, 0xa2, 0xc0, 0x66, 0x35}, {0x64, 0x56, 0xb9, 0xdd, 0x71, 0x27},
        {0x66, 0x55, 0xb0, 0xd6, 0x7c, 0x29}, {0x68, 0x5c, 0x8f, 0xe7, 0x5f, 0x03}, {0x6a, 0x5f, 0x86, 0xec, 0x52, 0x0d},
        {0x6c, 0x5a, 0x9d, 0xf1, 0x45, 0x1f}, {0x6e, 0x59, 0x94, 0xfa, 0x48, 0x11}, {0x70, 0x48, 0xe3, 0x93, 0x03, 0x4b},
        {0x72, 0x4b, 0xea, 0x98, 0x0e, 0x45}, {0x74, 0x4e, 0xf1, 0x85, 0x19, 0x57}, {0x76, 0x4d, 0xf8, 0x8e, 0x14, 0x59},
        {0x78, 0x44, 0xc7, 0xbf, 0x37, 0x73}, {0x7a, 0x47, 0xce, 0xb4, 0x3a, 0x7d}, {0x7c, 0x42, 0xd5, 0xa9, 0x2d, 0x6f},
        {0x7e, 0x41, 0xdc, 0xa2, 0x20, 0x61}, {0x80, 0xc0, 0x76, 0xf6, 0x6d, 0xad}, {0x82, 0xc3, 0x7f, 0xfd, 0x60, 0xa3},
        {0x84, 0xc6, 0x64, 0xe0, 0x77, 0xb1}, {0x86, 0xc5, 0x6d, 0xeb, 0x7a, 0xbf}, {0x88, 0xcc, 0x52, 0xda, 0x59, 0x95},
        {0x8a, 0xcf, 0x5b, 0xd1, 0x54, 0x9b}, {0x8c, 0xca, 0x40, 0xcc, 0x43, 0x89}, {0x8e, 0xc9, 0x49, 0xc7, 0x4e, 0x87},
        {0x90, 0xd8, 0x3e, 0xae, 0x05, 0xdd}, {0x92, 0xdb, 0x37, 0xa5, 0x08, 0xd3}, {0x94, 0xde, 0x2c, 0xb8, 0x1f, 0xc1},
        {0x96, 0xdd, 0x25, 0xb3, 0x12, 0xcf}, {0x98, 0xd4, 0x1a, 0x82, 0x31, 0xe5}, {0x9a, 0xd7, 0x13, 0x89, 0x3c, 0xeb},
        {0x9c, 0xd2, 0x08, 0x94, 0x2b, 0xf9}, {0x9e, 0xd1, 0x01, 0x9f, 0x26, 0xf7}, {0xa0, 0xf0, 0xe6, 0x46, 0xbd, 0x4d},
        {0xa2, 0xf3, 0xef, 0x4d, 0xb0, 0x43}, {0xa4, 0xf6, 0xf4, 0x50, 0xa7, 0x51}, {0xa6, 0xf5, 0xfd, 0x5b, 0xaa, 0x5f},
        {0xa8, 0xfc, 0xc2, 0x6a, 0x89, 0x75}, {0xaa, 0xff, 0xcb, 0x61, 0x84, 0x7b}, {0xac, 0xfa, 0xd0, 0x7c, 0x93, 0x69},
        {0xae, 0xf9, 0xd9, 0x77, 0x9e, 0x67}, {0xb0, 0xe8, 0xae, 0x1e, 0xd5, 0x3d}, {0xb2, 0xeb, 0xa7, 0x15, 0xd8, 0x33},
        {0xb4, 0xee, 0xbc, 0x08, 0xcf, 0x21}, {0xb6, 0xed, 0xb5, 0x03, 0xc2, 0x2f}, {0xb8, 0xe4, 0x8a, 0x32, 0xe1, 0x05},
        {0xba, 0xe7, 0x83, 0x39, 0xec, 0x0b}, {0xbc, 0xe2, 0x98, 0x24, 0xfb, 0x19}, {0xbe, 0xe1, 0x91, 0x2f, 0xf6, 0x17},
        {0xc0, 0xa0, 0x4d, 0x8d, 0xd6, 0x76}, {0xc2, 0xa3, 0x44, 0x86, 0xdb, 0x78}, {0xc4, 0xa6, 0x5f, 0x9b, 0xcc, 0x6a},
        {0xc6, 0xa5, 0x56, 0x90, 0xc1, 0x64}, {0xc8, 0xac, 0x69, 0xa1, 0xe2, 0x4e}, {0xca, 0xaf, 0x60, 0xaa, 0xef, 0x40},
        {0xcc, 0xaa, 0x7b, 0xb7, 0xf8, 0x52}, {0xce, 0xa9, 0x72, 0xbc, 0xf5, 0x5c}, {0xd0, 0xb8, 0x05, 0xd5, 0xbe, 0x06},
        {0xd2, 0xbb, 0x0c, 0xde, 0xb3, 0x08}, {0xd4, 0xbe, 0x17, 0xc3, 0xa4, 0x1a}, {0xd6, 0xbd, 0x1e, 0xc8, 0xa9, 0x14},
        {0xd8, 0xb4, 0x21, 0xf9, 0x8a, 0x3e}, {0xda, 0xb7, 0x28, 0xf2, 0x87, 0x30}, {0xdc, 0xb2, 0x33, 0xef, 0x90, 0x22},
        {0xde, 0xb1, 0x3a, 0xe4, 0x9d, 0x2c}, {0xe0, 0x90, 0xdd, 0x3d, 0x06, 0x96}, {0xe2, 0x93, 0xd4, 0x36, 0x0b, 0x98},
        {0xe4, 0x96, 0xcf, 0x2b, 0x1c, 0x8a}, {0xe6, 0x95, 0xc6, 0x20, 0x11, 0x84}, {0xe8, 0x9c, 0xf9, 0x11, 0x32, 0xae},
        {0xea, 0x9f, 0xf0, 0x1a, 0x3f, 0xa0}, {0xec, 0x9a, 0xeb, 0x07, 0x28, 0xb2}, {0xee, 0x99, 0xe2, 0x0c, 0x25, 0xbc},
        {0xf0, 0x88, 0x95, 0x65, 0x6e, 0xe6}, {0xf2, 0x8b, 0x9c, 0x6e, 0x63, 0xe8}, {0xf4, 0x8e, 0x87, 0x73, 0x74, 0xfa},
        {0xf6, 0x8d, 0x8e, 0x78, 0x79, 0xf4}, {0xf8, 0x84, 0xb1, 0x49, 0x5a, 0xde}, {0xfa, 0x87, 0xb8, 0x42, 0x57, 0xd0},
        {0xfc, 0x82, 0xa3, 0x5f, 0x40, 0xc2}, {0xfe, 0x81, 0xaa, 0x54, 0x4d, 0xcc}, {0x1b, 0x9b, 0xec, 0xf7, 0xda, 0x41},
        {0x19, 0x98, 0xe5, 0xfc, 0xd7, 0x4f}, {0x1f, 0x9d, 0xfe, 0xe1, 0xc0, 0x5d}, {0x1d, 0x9e, 0xf7, 0xea, 0xcd, 0x53},
        {0x13, 0x97, 0xc8, 0xdb, 0xee, 0x79}, {0x11, 0x94, 0xc1, 0xd0, 0xe3, 0x77}, {0x17, 0x91, 0xda, 0xcd, 0xf4, 0x65},
        {0x15, 0x92, 0xd3, 0xc6, 0xf9, 0x6b}, {0x0b, 0x83, 0xa4, 0xaf, 0xb2, 0x31}, {0x09, 0x80, 0xad, 0xa4, 0xbf, 0x3f},
        {0x0f, 0x85, 0xb6, 0xb9, 0xa8, 0x2d}, {0x0d, 0x86, 0xbf, 0xb2, 0xa5, 0x23}, {0x03, 0x8f, 0x80, 0x83, 0x86, 0x09},
        {0x01, 0x8c, 0x89, 0x88, 0x8b, 0x07}, {0x07, 0x89, 0x92, 0x95, 0x9c, 0x15}, {0x05, 0x8a, 0x9b, 0x9e, 0x91, 0x1b},
        {0x3b, 0xab, 0x7c, 0x47, 0x0a, 0xa1}, {0x39, 0xa8, 0x75, 0x4c, 0x07, 0xaf}, {0x3f, 0xad, 0x6e, 0x51, 0x10, 0xbd},
        {0x3d, 0xae, 0x67, 0x5a, 0x1d, 0xb3}, {0x33, 0xa7, 0x58, 0x6b, 0x3e, 0x99}, {0x31, 0xa4, 0x51, 0x60, 0x33, 0x97},
        {0x37, 0xa1, 0x4a, 0x7d, 0x24, 0x85}, {0x35, 0xa2, 0x43, 0x76, 0x29, 0x8b}, {0x2b, 0xb3, 0x34, 0x1f, 0x62, 0xd1},
        {0x29, 0xb0, 0x3d, 0x14, 0x6f, 0xdf}, {0x2f, 0xb5, 0x26, 0x09, 0x78, 0xcd}, {0x2d, 0xb6, 0x2f, 0x02, 0x75, 0xc3},
        {0x23, 0xbf, 0x10, 0x33, 0x56, 0xe9}, {0x21, 0xbc, 0x19, 0x38, 0x5b, 0xe7}, {0x27, 0xb9, 0x02, 0x25, 0x4c, 0xf5},
        {0x25, 0xba, 0x0b, 0x2e, 0x41, 0xfb}, {0x5b, 0xfb, 0xd7, 0x8c, 0x61, 0x9a}, {0x59, 0xf8, 0xde, 0x87, 0x6c, 0x94},
        {0x5f, 0xfd, 0xc5, 0x9a, 0x7b, 0x86}, {0x5d, 0xfe, 0xcc, 0x91, 0x76, 0x88}, {0x53, 0xf7, 0xf3, 0xa0, 0x55, 0xa2},
        {0x51, 0xf4, 0xfa, 0xab, 0x58, 0xac}, {0x57, 0xf1, 0xe1, 0xb6, 0x4f, 0xbe}, {0x55, 0xf2, 0xe8, 0xbd, 0x42, 0xb0},
        {0x4b, 0xe3, 0x9f, 0xd4, 0x09, 0xea}, {0x49, 0xe0, 0x96, 0xdf, 0x04, 0xe4}, {0x4f, 0xe5, 0x8d, 0xc2, 0x13, 0xf6},
        {0x4d, 0xe6, 0x84, 0xc9, 0x1e, 0xf8}, {0x43, 0xef, 0xbb, 0xf8, 0x3d, 0xd2}, {0x41, 0xec, 0xb2, 0xf3, 0x30, 0xdc},
        {0x47, 0xe9, 0xa9, 0xee, 0x27, 0xce}, {0x45, 0xea, 0xa0, 0xe5, 0x2a, 0xc0}, {0x7b, 0xcb, 0x47, 0x3c, 0xb1, 0x7a},
        {0x79, 0xc8, 0x4e, 0x37, 0xbc, 0x74}, {0x7f, 0xcd, 0x55, 0x2a, 0xab, 0x66}, {0x7d, 0xce, 0x5c, 0x21, 0xa6, 0x68},
        {0x73, 0xc7, 0x63, 0x10, 0x85, 0x42}, {0x71, 0xc4, 0x6a, 0x1b, 0x88, 0x4c}, {0x77, 0xc1, 0x71, 0x06, 0x9f, 0x5e},
        {0x75, 0xc2, 0x78, 0x0d, 0x92, 0x50}, {0x6b, 0xd3, 0x0f, 0x64, 0xd9, 0x0a}, {0x69, 0xd0, 0x06, 0x6f, 0xd4, 0x04},
        {0x6f, 0xd5, 0x1d, 0x72, 0xc3, 0x16}, {0x6d, 0xd6, 0x14, 0x79, 0xce, 0x18}, {0x63, 0xdf, 0x2b, 0x48, 0xed, 0x32},
        {0x61, 0xdc, 0x22, 0x43, 0xe0, 0x3c}, {0x67, 0xd9, 0x39, 0x5e, 0xf7, 0x2e}, {0x65, 0xda, 0x30, 0x55, 0xfa, 0x20},
        {0x9b, 0x5b, 0x9a, 0x01, 0xb7, 0xec}, {0x99, 0x58, 0x93, 0x0a, 0xba, 0xe2}, {0x9f, 0x5d, 0x88, 0x17, 0xad, 0xf0},
        {0x9d, 0x5e, 0x81, 0x1c, 0xa0, 0xfe}, {0x93, 0x57, 0xbe, 0x2d, 0x83, 0xd4}, {0x91, 0x54, 0xb7, 0x26, 0x8e, 0xda},
        {0x97, 0x51, 0xac, 0x3b, 0x99, 0xc8}, {0x95, 0x52, 0xa5, 0x30, 0x94, 0xc6}, {0x8b, 0x43, 0xd2, 0x59, 0xdf, 0x9c},
        {0x89, 0x40, 0xdb, 0x52, 0xd2, 0x92}, {0x8f, 0x45, 0xc0, 0x4f, 0xc5, 0x80}, {0x8d, 0x46, 0xc9, 0x44, 0xc8, 0x8e},
        {0x83, 0x4f, 0xf6, 0x75, 0xeb, 0xa4}, {0x81, 0x4c, 0xff, 0x7e, 0xe6, 0xaa}, {0x87, 0x49, 0xe4, 0x63, 0xf1, 0xb8},
        {0x85, 0x4a, 0xed, 0x68, 0xfc, 0xb6}, {0xbb, 0x6b, 0x0a, 0xb1, 0x67, 0x0c}, {0xb9, 0x68, 0x03, 0xba, 0x6a, 0x02},
        {0xbf, 0x6d, 0x18, 0xa7, 0x7d, 0x10}, {0xbd, 0x6e, 0x11, 0xac, 0x70, 0x1e}, {0xb3, 0x67, 0x2e, 0x9d, 0x53, 0x34},
        {0xb1, 0x64, 0x27, 0x96, 0x5e, 0x3a}, {0xb7, 0x61, 0x3c, 0x8b, 0x49, 0x28}, {0xb5, 0x62, 0x35, 0x80, 0x44, 0x26},
        {0xab, 0x73, 0x42, 0xe9, 0x0f, 0x7c}, {0xa9, 0x70, 0x4b, 0xe2, 0x02, 0x72}, {0xaf, 0x75, 0x50, 0xff, 0x15, 0x60},
        {0xad, 0x76, 0x59, 0xf4, 0x18, 0x6e}, {0xa3, 0x7f, 0x66, 0xc5, 0x3b, 0x44}, {0xa1, 0x7c, 0x6f, 0xce, 0x36, 0x4a},
        {0xa7, 0x79, 0x74, 0xd3, 0x21, 0x58}, {0xa5, 0x7a, 0x7d, 0xd8, 0x2c, 0x56}, {0xdb, 0x3b, 0xa1, 0x7a, 0x0c, 0x37},
        {0xd9, 0x38, 0xa8, 0x71, 0x01, 0x39}, {0xdf, 0x3d, 0xb3, 0x6c, 0x16, 0x2b}, {0xdd, 0x3e, 0xba, 0x67, 0x1b, 0x25},
        {0xd3, 0x37, 0x85, 0x56, 0x38, 0x0f}, {0xd1, 0x34, 0x8c, 0x5d, 0x35, 0x01}, {0xd7, 0x31, 0x97, 0x40, 0x22, 0x13},
        {0xd5, 0x32, 0x9e, 0x4b, 0x2f, 0x1d}, {0xcb, 0x23, 0xe9, 0x22, 0x64, 0x47}, {0xc9, 0x20, 0xe0, 0x29, 0x69, 0x49},
        {0xcf, 0x25, 0xfb, 0x34, 0x7e, 0x5b}, {0xcd, 0x26, 0xf2, 0x3f, 0x73, 0x55}, {0xc3, 0x2f, 0xcd, 0x0e, 0x50, 0x7f},
        {0xc1, 0x2c, 0xc4, 0x05, 0x5d, 0x71}, {0xc7, 0x29, 0xdf, 0x18, 0x4a, 0x63}, {0xc5, 0x2a, 0xd6, 0x13, 0x47, 0x6d},
        {0xfb, 0x0b, 0x31, 0xca, 0xdc, 0xd7}, {0xf9, 0x08, 0x38, 0xc1, 0xd1, 0xd9}, {0xff, 0x0d, 0x23, 0xdc, 0xc6, 0xcb},
        {0xfd, 0x0e, 0x2a, 0xd7, 0xcb, 0xc5}, {0xf3, 0x07, 0x15, 0xe6, 0xe8, 0xef}, {0xf1, 0x04, 0x1c, 0xed, 0xe5, 0xe1},
        {0xf7, 0x01, 0x07, 0xf0, 0xf2, 0xf3}, {0xf5, 0x02, 0x0e, 0xfb, 0xff, 0xfd}, {0xeb, 0x13, 0x79, 0x92, 0xb4, 0xa7},
        {0xe9, 0x10, 0x70, 0x99, 0xb9, 0xa9}, {0xef, 0x15, 0x6b, 0x84, 0xae, 0xbb}, {0xed, 0x16, 0x62, 0x8f, 0xa3, 0xb5},
        {0xe3, 0x1f, 0x5d, 0xbe, 0x80, 0x9f}, {0xe1, 0x1c, 0x54, 0xb5, 0x8d, 0x91}, {0xe7, 0x19, 0x4f, 0xa8, 0x9a, 0x83},
        {0xe5, 0x1a, 0x46, 0xa3, 0x97, 0x8d}
    };

void xor_blocks(unsigned char *block1, unsigned char *block2)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
        block1[i] ^= block2[i];
}

void get_ith_item(unsigned char *dest, unsigned char *buffer, int i, int l)
{
    for (int j = 0; j < l; j++)
        dest[j] = buffer[i * l + j];
}

void print_data(unsigned char *data, int length)
{
    for (int i = 0; i < length; i++)
        printf("%x ", data[i]);
    printf("\n");
}

unsigned char *read_from_file(char *file_name, int *file_size)
{
    FILE *f = fopen(file_name, "rb");
    unsigned char *buffer;

    fseek(f, 0, SEEK_END);
    *file_size = ftell(f);
    rewind(f);

    long to_allocate = *file_size + (*file_size % BLOCK_SIZE);
    buffer = (unsigned char *)calloc(to_allocate, sizeof(unsigned char));
    fread(buffer, *file_size, 1, f);
    fclose(f);

    return buffer;
}

void write_to_file(char *file_name, unsigned char *buffer, long n)
{
    FILE *f = fopen(file_name, "wb");

    fwrite(buffer, n, 1, f);
    fclose(f);
}


void rotword(unsigned char *tab)
{ // tab should be a word, thus it is 4 bytes in length
    unsigned char save = tab[0];
    int i;
    for (i = 0; i < 4 - 1; i++)
        tab[i] = tab[i + 1];

    tab[i] = save;
}

void subword(unsigned char *tab)
{ // tab should be a word, thus it is 4 bytes in length
    for (int i = 0; i < 4; i++)
        tab[i] = sbox[tab[i]];
}

unsigned char *key_expansion(unsigned char *key, int key_length)
{                            // key_length in bytes
    int Nr = ROUNDS_NUMBER;  // number of rounds
    int Nb = 4;              // number of words in a block
    int Nk = key_length / 4; // number of words in key

    unsigned char *w = (unsigned char *)malloc(4 * (Nr + 1) * Nb * sizeof(unsigned char));

    // first subkey is the key itself
    memcpy(w, key, BLOCK_SIZE);

    // compute next subkeys
    for (int i = Nk; i < Nb * (Nr + 1); i++)
    {
        int k = (i - 1) * 4;
        unsigned char temp[4] = {w[k], w[k + 1], w[k + 2], w[k + 3]}; // temp is equivalent to w[i-1]
        unsigned char c = 0;

        if (i % Nk == 0)
        {
            rotword(temp);
            subword(temp);
            c = rcon[i / Nk - 1];
        }
        else if (Nk > 6 && i % Nk == 4)
        {
            subword(temp);
        }

        for (int j = 0; j < 4; j++)
        {
            w[4 * i + j] = w[4 * (i - Nk) + j] ^ temp[j] ^ c; // this operation is equivalent to w[i] = w[i-Nk] xor temp xor c
            c = 0;
        }
    }

    return w;
}

void subytes(unsigned char *block)
{

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        block[i] = sbox[block[i]];
    }
}

void addRoundKey(unsigned char *block, unsigned char *subkey)
{

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        block[i] ^= subkey[i];
    }
}

void shiftRows(unsigned char *block)
{
    unsigned char tmp;

    tmp = block[1];
    block[1] = block[5];
    block[5] = block[9];
    block[9] = block[13];
    block[13] = tmp;

    tmp = block[10];
    block[10] = block[2];
    block[2] = tmp;

    tmp = block[14];
    block[14] = block[6];
    block[6] = tmp;

    tmp = block[3];
    block[3] = block[15];
    block[15] = block[11];
    block[11] = block[7];
    block[7] = tmp;
}

void mixColumns(unsigned char *block)
{
    unsigned char b[4];
    int i;

    for (i = 0; i < BLOCK_SIZE; i += 4)
    {
        b[0] = block[i];
        b[1] = block[i + 1];
        b[2] = block[i + 2];
        b[3] = block[i + 3];

        block[i + 0] = mixColMatrix[b[0]][0] ^ mixColMatrix[b[1]][1] ^ b[2] ^ b[3];
        block[i + 1] = b[0] ^ mixColMatrix[b[1]][0] ^ mixColMatrix[b[2]][1] ^ b[3];
        block[i + 2] = b[0] ^ b[1] ^ mixColMatrix[b[2]][0] ^ mixColMatrix[b[3]][1];
        block[i + 3] = mixColMatrix[b[0]][1] ^ b[1] ^ b[2] ^ mixColMatrix[b[3]][0];
    }
}

unsigned char *AES_CORE(unsigned char *block, unsigned char *expanded_key)
{
    int round = 0;

    // get copy of block in state variable
    unsigned char *state = (unsigned char *)malloc(BLOCK_SIZE * sizeof(unsigned char));
    memcpy(state, block, BLOCK_SIZE);

    // get first subkey from the expanded key
    unsigned char *subkey = (unsigned char *)malloc(BLOCK_SIZE * sizeof(unsigned char));
    get_ith_item(subkey, expanded_key, round, BLOCK_SIZE);

    addRoundKey(state, subkey);

    for (round = 1; round <= ROUNDS_NUMBER; round++)
    {
        subytes(state);
        shiftRows(state);
        if (round != ROUNDS_NUMBER)
            mixColumns(state);
        get_ith_item(subkey, expanded_key, round, BLOCK_SIZE);
        addRoundKey(state, subkey);
    }

    free(subkey);
    subkey = NULL;
    return state;
}

void encrypt_block(unsigned char *block, unsigned char *expanded_key, unsigned char *counter)
{
    unsigned char *p = AES_CORE(counter, expanded_key);
    xor_blocks(block, p);

    free(p);
    p = NULL;
}

unsigned char *generate_initial_counter()
{
    unsigned char *nonce = (unsigned char *)malloc(BLOCK_SIZE * sizeof(unsigned char));
    srand(time(0));

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        nonce[i] = rand() & 0xff;
    }

    return nonce;
}

void increment_counter(unsigned char *counter, int inc)
{
    int i = BLOCK_SIZE - 1;
    int stop = 0;
    while (i >= 0 && !stop)
    {
            counter[i] = (counter[i]+inc)%256;
            i--;   
    }

}

void next_counter(unsigned char *counter)
{
    increment_counter(counter, 2);
}

void insert_flag_in_file(const char* file_name, const char* new_text) {
    // open file for reading and writing
    FILE* fp = fopen(file_name, "r+");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("File opened.\n");
    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    printf("File opened.2\n");
    srand(time(NULL));
    long random_pos = rand() % file_size/2 + file_size/2;

    fseek(fp, random_pos, SEEK_SET);
    printf("File opened.3\n");

    char buffer[file_size - random_pos];
    fread(buffer, 1, file_size - random_pos, fp);

    char new_buffer[file_size - random_pos + strlen(new_text)];
    strncpy(new_buffer, buffer, random_pos);
    new_buffer[random_pos] = '\0';
    strcat(new_buffer, new_text);
    strcat(new_buffer, &buffer[0]);
    printf("File opened.3\n");

    fseek(fp, random_pos, SEEK_SET);

    printf("File opened.4\n");
    fwrite(new_buffer, 1, strlen(new_buffer), fp);

    printf("File opened.4\n");
    fclose(fp);
}

unsigned char *encrypt(unsigned char *plaintext, size_t size, unsigned char *key, unsigned char *counter) {
    size_t nblocks;
    unsigned char *expansion, *ciphertext, *block;
    expansion = key_expansion(key, BLOCK_SIZE);
    nblocks = size / BLOCK_SIZE + (size % BLOCK_SIZE != 0);
    ciphertext = (unsigned char*) malloc(nblocks * BLOCK_SIZE * sizeof(unsigned char));
    block = (unsigned char*) malloc(BLOCK_SIZE * sizeof(unsigned char));
    for (int i = 0; i < nblocks; i++) {
        get_ith_item(block, plaintext, i, BLOCK_SIZE);
        encrypt_block(block, expansion, counter);
        memcpy(ciphertext + BLOCK_SIZE*i, block, BLOCK_SIZE);
        next_counter(counter);
    }
    free(block);
    block = NULL;
    return ciphertext;
}


int main(int argc, char *argv[]) {
    int l = 0;
    unsigned char * key = read_from_file(KEY_FILENAME, &l);
    unsigned char *counter = generate_initial_counter();
    write_to_file(NONCE_FILENAME, counter, BLOCK_SIZE);
    int size = 0;
    // insert_flag_in_file(PLAINTEXT_FILENAME, FLAG);
    unsigned char * plaintext = read_from_file(PLAINTEXT_FILENAME, &size);
    int nb = size / BLOCK_SIZE + (size % BLOCK_SIZE != 0);
    unsigned char * ciphertext = encrypt(plaintext, size, key, counter);
    write_to_file(CIPHERTEXT_FILENAME, ciphertext, size);
    free(key);
    key = NULL;
    free(plaintext);
    plaintext = NULL;
    free(ciphertext);
    ciphertext = NULL;
    return 0;
}
