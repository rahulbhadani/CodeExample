#!/usr/bin/env python
# coding: utf-8

# author: Rahul Bhadani

def my_generator():
    yield "Hello"
    yield "World!"
    yield "How"
    yield "are"
    yield "you"

def even_numbers(end_limit):
    for n in range(end_limit):
        if(n%2 == 0):
            yield n

generator_obj = my_generator()

print("Type of generator object is ")
print(generator_obj)

for g in generator_obj:
    print(g)

get_even = even_numbers(15)

print(list(get_even))
