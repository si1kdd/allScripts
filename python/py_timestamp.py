#!/usr/bin/env python3
# A small script to check timestamp now,
#
# Windows:
#       Use win32 structure directly
# Unix:
#       use datetime module
#
#
#
# @ Usage:
#       ./timestamp.py
# @ Autor:
#       si1kdd
#
# @ Status:
#       Unfinished ... would add Unix method later ...

import platform
import ctypes as ct
import datetime as dt


# Python get time under windows
class WIN_SYSTEMTIME(ct.Structure):
    _fields_ = [
        ('wYear',   ct.c_int16),
        ('wMonth',  ct.c_int16),
        ('wDayOfWeek', ct.c_int16),
        ('wDay',    ct.c_int16),
        ('wHour',   ct.c_int16),
        ('wMinute', ct.c_int16),
        ('wSecond', ct.c_int16),
        ('wMilliseconds', ct.c_int16)
    ]


def get_time():
    win32_time = WIN_SYSTEMTIME()
    ptr_win32_time = ct.pointer(win32_time)
    ct.windll.kernel32.GetLocalTime(ptr_win32_time)
    timestamp = "{}/{}/{} {}:{}:{}".format(
        str(win32_time.wYear),
        str(win32_time.wMonth),
        str(win32_time.wDay),
        str(win32_time.wHour),
        str(win32_time.wMinute),
        str(win32_time.wSecond)
    )
    return timestamp


print("[*] Timestamp: {}".format(get_time()))
