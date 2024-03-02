#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Handle timestamps in the tachograph data files

The tachograph data files contain timestamps of the beginnings of days at UTC."""

import calendar
import time
from struct import pack, unpack


def format_timestamp(seconds):
    """Format timestamp as ISO date"""
    t = time.gmtime(seconds)
    if t.tm_hour == t.tm_min == t.tm_sec == 0:
        return time.strftime('%Y-%m-%d', t)
    else:
        return time.strftime('%Y-%m-%d %H.%M (%Z)', t)


def date_to_timestamp(datestring):
    """convert iso date to timestring"""
    t = time.strptime(datestring + ' UTC', '%Y-%m-%d %Z')
    return int(calendar.timegm(t))


def pack_timestamp(seconds):
    """pack timestamp into 32-bit big endian bytes"""
    datastring = pack('>I', seconds)
    return datastring


def unpack_timestamp(datastring):
    """unpack timestamp from 32-bit big endian bytes"""
    return unpack('>I', datastring)[0]
