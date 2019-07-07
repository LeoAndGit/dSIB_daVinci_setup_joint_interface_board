# -*- coding: utf-8 -*-
'''
Author: Leo
Copyright 2019, github/LeoAndGit>

'''

import serial
import time
class MySerial(serial.Serial):
    def read_until(self, terminator=b"\n", size=None):
        """\
        Read until a termination sequence is found ('\n' by default), the size
        is exceeded or until timeout occurs.
        重写serial库的行读取函数，
        (删除了有关超时的部分）
        """
        lenterm = len(terminator)
        line = bytearray()
        while True:
            c = self.read(1)
            if c:
                line += c
                if line[-lenterm:] == terminator:
                    break
                if size is not None and len(line) >= size:
                    break
            else:
                break
        return bytes(line)


