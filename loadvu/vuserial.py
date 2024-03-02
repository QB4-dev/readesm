#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Tool to read tachograph data from a vehicle unit connected serially"""
## Copyright(C) 2011 Andreas Gölzer

# This is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.

# It is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.

from time import sleep
from struct import pack, unpack
import serial
from serial import SerialException


def hexchunk(data):
    """convert data in a string to hexadecimal representation

    For example: "\x76\x6e" to "76 6e"."""
    rv = ''
    for byte in data:
        rv += ' ' + hex(ord(byte))
    return rv


class MalformedHeader(Exception):
    """Exception for malformed Header"""
    def __init__(self, received):
        self.received = received

    def __str__(self):
        response = hexchunk(self.received)
        if len(self.received) == 0:
            response = 'nothing'
        return 'Expected header 0x80 0xF0 0xEE, but got ' + response + '.'


class CommunicationError(Exception):
    """Exception for errors in the vu-protocol (checksum wrong 3 times etc.)"""
    def __init__(self, description):
        self.description = description

    def __str__(self):
        return self.description


class WrongChecksum(Exception):
    """wrong checksum, should be caught and the wrongly transmitted message rerequested"""
    def __str__(self):
        return 'Received message had wrong checksum'


class VuErrorMessage(Exception):
    """Error Messages from the Vehicle unit decoded according to the legislation"""
    def __init__(self, code, requestSID):
        self.code = code
        self.requestSID = requestSID

    def __str__(self):
        errors = {
            0x10: 'General reject',
            0x11: 'Service not supported',
            0x12: 'Sub function not supported',
            0x13: 'Incorrect Message Length',
            0x22: 'Conditions not correct or Request sequence error',
            0x31: 'Request out of range',
            0x50: 'Upload not accepted',
            0x78: 'Response pending',
            0xFA: 'Data not available'
        }
        rv = 'The VU has rejected the request with SID %s and returned an'\
             ' error instead: ' % hex(self.requestSID)
        if self.code in errors:
            rv += errors[self.code]
        else:
            rv += 'Unknown error code %s' % hex(self.code)
        return rv


def trep_to_string(trep):
    """Request number to human-readable string"""
    trep_strings = {
        0x1: 'Overview',
        0x2: 'Activities recorded on ',
        0x3: 'Events and faults',
        0x4: 'Detailed speed',
        0x5: 'Technical data',
        0x6: 'Card download'
    }
    if trep in trep_strings.keys():
        return trep_strings[trep]
    else:
        return 'Unknown, TREP ' + str(trep)


class VuSerial:
    """Class for Serial connections to the vehicle unit"""
    def __init__(self, serial_port, max_symbol_rate=115200, verbose=1):
        self.open = False
        self.verbose = verbose
        self.conn = serial.Serial(serial_port,
                                  9600,
                                  parity=serial.PARITY_EVEN,
                                  timeout=20)
        self.send_raw_expecting_response('\x81\xEE\xF0\x81\xE0',
                                         '\xC1\xEA\x8F',
                                         'Start Communication Request')
        self.send_expecting_response('\x10\x81',
                                     '\x50\x81',
                                     'Diagnostic Session')

        symbol_rates = {9600: 1, 19200: 2, 38400: 3, 57600: 4, 115200: 5}
        for symbol_rate in sorted(symbol_rates, reverse=True):
            if max_symbol_rate < symbol_rate:
                continue
            try:
                self.send_expecting_response('\x87\x01\x01'
                                             + chr(symbol_rates[symbol_rate]),
                                             '\xC7\x01',
                                             'Request Symbol Rate %i'
                                             % symbol_rate)
            except CommunicationError as err:
                print(err)
            else:
                self.send_composed_data('\x87\x02\x03')
                sleep(0.1)  # wait until data is sent
                self.conn.setBaudrate(symbol_rate)
                sleep(0.1)  # wait until transition is handled
                break
        self.send_expecting_response('\x35\0\0\0\0\0\xFF\xFF\xFF\xFF',
                                     '\x75\0\xFF',
                                     name='Request Upload')
        self.open = True

    def __del__(self):
        self.close()

    def close(self):
        """Close connection if it's still open, send required commands"""
        if self.open:
            self.send_expecting_response(chr(0x37),
                                         chr(0x77),
                                         name='Transfer Exit')
            self.send_expecting_response(chr(0x82),
                                         chr(0xC2),
                                         name='Stop Communication')
            self.conn.close()
            self.open = False

    def _get_checksum(self, data):
        """simple modulo-256-checksum."""
        datasum = 0
        for byte in data:
            datasum += ord(byte)
        return datasum % 256

    def get_block(self, trep, parameter=''):
        """Download a vehicle unit data block with trep and parameter
        (date for activity blocks)
        """
        if self.verbose >= 1:
            print('Requesting to download data for TREP %i (%s)'
                  % (trep, trep_to_string(trep)))
        self.send_composed_data(chr(0x36) + chr(trep) + parameter)
        payload = ''
        counter = 1
        failcounter = 0
        while True:
            try:
                response = self.single_response()
                sid = response[0]
                if self.verbose >= 2:
                    print('SID ' + sid + ', TREP ' + response[1]
                          + ', submessagecounter ',
                          unpack('>H', response[2:4])[0])
            except (WrongChecksum, MalformedHeader):
                failcounter += 1
                if failcounter == 3:
                    raise CommunicationError('Failed to get the same '
                                             '255-byte-chunk three times,'
                                             ' aborting')
                self.send_composed_data(chr(0x83) + sid + pack('>H', counter))
            else:
                failcounter = 0
                if len(response) < 255 and counter == 1:
                    payload += response[2:]
                else:
                    payload += response[4:]
                if len(response) < 255:
                    if self.verbose >= 1:
                        print('end of block, got %i bytes' % len(payload))
                    break
                counter += 1
                self.send_composed_data(chr(0x83) + sid + pack('>H', counter))
        return response[0:2] + payload

    def send_raw_expecting_response(self, rawsenddata, expectdata, name=''):
        """Send raw data to the VU, expect a response
        The first message to the VU requires message type 0x81, all other
        messages should be sent using send_expecting_response. expectdata
        should be without header and checksum
        """
        if name != '' and self.verbose >= 1:
            print("Sending 'Request %s'" % name)
        self.send_data(rawsenddata)
        response = self.single_response()
        if response == expectdata:
            if self.verbose >= 1:
                print('Got the expected response')
        else:
            raise CommunicationError('Expected ' + hexchunk(expectdata)
                                     + ' as response to ' + name
                                     + ', got ' + hexchunk(response))

    def send_expecting_response(self, senddata, expectdata, name=''):
        """send data to the VU after adding header, read back data"""
        return self.send_raw_expecting_response(self.compose_message(senddata),
                                                expectdata,
                                                name)

    def single_response(self):
        """Read response from the vehicle unit"""
        header = self.conn.read(4)
        if self.verbose >= 3:
            print('got header: ' + hexchunk(header))
        if header[0:3] != '\x80\xF0\xEE':
            raise MalformedHeader(header)
        length = ord(header[3])
        data_with_checksum = self.conn.read(length + 1)
        if self.verbose >= 3:
            print('got data: ' + hexchunk(data_with_checksum))
        payload = data_with_checksum[:-1]
        if self._get_checksum(header + payload) != ord(data_with_checksum[-1]):
            raise WrongChecksum
        if length == 3 and ord(payload[0]) == 0x7F:
            if ord(payload[2]) == 0x78:
                if self.verbose >= 1:
                    print('Got Response pending message, waiting')
                return self.single_response()
            raise VuErrorMessage(ord(payload[2]), ord(payload[1]))
        return payload

    def send_data(self, data):
        """Send data to the vehicle unit"""
        if self.verbose >= 3:
            print('sending ' + hexchunk(data))
        self.conn.write(data)

    def send_composed_data(self, data):
        """Send data to the vehicle unit after adding header and checksum"""
        self.send_data(self.compose_message(data))

    def compose_message(self, data):
        """add header(0x80 for message type, 0xEE for target VU, 0xF0 for
        source IDE, length of payload) and checksum to the data
        """
        fullmsg = '\x80\xEE\xF0' + chr(len(data)) + data
        checksum = self._get_checksum(fullmsg)
        return fullmsg + chr(checksum)
