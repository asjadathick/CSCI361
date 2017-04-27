#!/bin/bash
g++ main.cpp -o affine

#Encrypt
./affine -key B 3 -encrypt 4970512

#Decrypt
./affine -key B 3 -decrypt 54A320B

#Invalid
./affine -key 4 C -decrypt 1087AC5
