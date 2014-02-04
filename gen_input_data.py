#!/usr/bin/env python3

import random
import sys
import string

class DataTypeRangeError(Exception): pass

usage_prompt = "./gen_input_data.py <total_word_cnt> <data_type: i | s>"

try:
    total_word_cnt = int(sys.argv[1])
    data_type = sys.argv[2]
    if data_type not in "is":
        raise DataTypeRangeError("Unknown data type '{0}'".format(data_type))
except IndexError:
    print("ERROR: missing total_word_cnt or data_type field.")
    print(usage_prompt)
    sys.exit()
except ValueError:
    print("ERROR: total_word_cnt should be integer.")
    print(usage_prompt)
    sys.exit()
except DataTypeRangeError as err:
    print("ERROR:", err)
    sys.exit()
    

for word_cnt in range(total_word_cnt):
    if data_type == "i":
        print(random.randrange(0, 100))
    elif data_type == "s":
        for char_cnt in range(10):
            print(random.choice(string.ascii_lowercase), end="")
        print()
    else:
        pass
