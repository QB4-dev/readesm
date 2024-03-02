#!/usr/bin/env python3
# -*- coding: utf-8 -*-

## Generate the classes needed to read tachograph data
## Copyright(C) Andreas Gölzer

# remove auto-gen'd files with
# now for python 3
# find . \( ! -regex '.*/\..*' ! -name "generate.py" \) | xargs grep -snH "AUTO-GENERATED" --color | cut -f1 -d':' | xargs rm


from xml.etree.ElementTree import ElementTree
import os
import sys

XML_FILENAME = 'DataDefinitions.xml'
tree = ElementTree()
try:
    tree.parse(XML_FILENAME)
except IOError:
    __dir__ = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(__dir__, XML_FILENAME)
    tree.parse(filepath)

# os.mkdir('CardBlocks')
# os.mkdir('DataTypes')
# os.mkdir('VuBlocks')


def write_if_different(filename, new_content):
    try:
        file = open(filename, 'r')
        content = file.read()
        file.close()
    except IOError:
        content = ''
    if content != new_content:
        file = open(filename, 'w')
        file.write(new_content)
        file.close()
        print(filename)


def lcfirst(string):
    return string[0].lower() + string[1:]


def sorted_includes(iterable):
    return sorted(iterable,
                  key=lambda s: ('\0' + s)if s.startswith('<') else s)


def write_code_file(name, directory, header_content, code_content,
                    header_dependencies, code_dependencies):
    auto_gen_warning = '\n/** \\file \n\\brief auto-generated file for '\
        + name + '\n\nThis file was AUTO-GENERATED.\nMake changes in the'\
        ' generator script generate.py,\nthe data definitions in'\
        ' DataDefinitions.xml\nor derive a class */\n\n'
    header_file = '#ifndef ' + name.upper() + '_H\n'\
        + '#define ' + name.upper() + '_H\n'\
        + '\n' + auto_gen_warning\
        + '\n\n' + '\n'.join(
            ['#include ' + f for f in sorted_includes(header_dependencies)])\
        + '\n\n'\
        + header_content\
        + '\n#endif\n'
    code_file = '#include "' + name + '.h"\n'\
        + '\n' + auto_gen_warning\
        + '\n\n' + '\n'.join(
            ['#include ' + f for f in sorted_includes(code_dependencies)])\
        + '\n\n' + code_content

    filename = directory + '/' + name + '.h'
    write_if_different(filename, header_file)

    filename = directory + '/' + name + '.cpp'
    write_if_different(filename, code_file)


sizes = {
    'TimeReal': 4,
    'Timespan': 8,
    'ActivityChangeInfo': 2
    }

for j in range(3):
    have_sizes = True
    for block in tree.findall('DataType'):
        name = block.get('name')
        if name in sizes:
            continue
        try:
            print(name)
            size = 0
            for elem in block.find('content'):
                length = elem.get('length')
                if length:
                    size += int(length)
                elif elem.tag in sizes:
                    size += sizes[elem.tag]
                else:
                    raise StopIteration()
            sizes[block.get('name')] = size
        except StopIteration:
            have_sizes = False
            continue
print(sizes)
if not have_sizes:
    print("Could not determine block sizes for "
          + ', '.join(block.get('name')
                      if block.get('name') not in sizes
                      else "" for block in tree.findall('DataType'))
          + ", abandoning")
    sys.exit()

hasToString = set(['TimeReal', 'Timespan', 'RawData', 'LargeNumber'])
# for block in tree.findall('DataType'):
#     if block.find('toString') is not None:
#         hasToString.add(block.get('name'))
# print hasToString


for block in tree.findall('CardBlock')\
        + tree.findall('DataType')\
        + tree.findall('VuBlock'):
    initList = block.tag + '(filewalker)'
    output = ''
    elements = ''
    header_dependencies = set(['"' + block.tag + '.h"'])
    code_dependencies = set()
    name = block.get('name')
    print(name)
    if block.get('hasrefined'):
        name = 'Raw' + name

    offsets = {'CardBlock': 5, 'VuBlock': 2, 'DataType': 0}
    offset = offsets[block.tag]
    offsetextra = ''
    for elem in block.find('content'):
        ename = elem.get('name')
        tag = elem.tag
        is_last = False
        thisoutput = None
        full_offset = 'start + %(offset)i%(offsetextra)s' % vars()

        length = elem.get('length')
        if length is not None:
            length = int(length)
        elif tag in sizes:
            length = sizes[tag]
        elif block.tag == 'CardBlock':
            length = 'dataBlockSize() - (%(offset)i%(offsetextra)s)' % vars()
            is_last = True

        if tag == 'string':
            tag = 'QString'
            builder = 'fixedString(%s, %i)' % (full_offset, length)
        elif tag == 'cpstring':
            tag = 'QString'
            builder = 'codepageStringCombination(%s, %i)'\
                % (full_offset, length)
        elif tag == 'int':
            encoding = elem.get('encoding')
            if encoding == 'BCD':
                builder = 'readBCD%(length)i(%(full_offset)s)' % vars()
            else:
                builder = 'readBigEndianInt%(length)i(%(full_offset)s)'\
                    % vars()
            unit = elem.get('unit')
            if unit:
                thisoutput = '\nreport.tagValuePair(tr("%(ename)s"), QString("%%1 %(unit)s").arg(%(ename)s));'\
                    % vars()
            else:
                thisoutput = '\nreport.tagValuePair(tr("%(ename)s"), %(ename)s);'\
                    % vars()
        elif tag in ('RawData', 'LargeNumber'):
            builder = '%s, %s' % (full_offset, str(length))
            header_dependencies.add('"../DataTypes/' + tag + '.h"')
        elif tag == 'Subblocks':
            subtype = elem.get('type')
            counterlength = elem.get('counterlength')
            if not ename:
                ename = lcfirst(subtype + 's')
            if counterlength:
                length = int(counterlength)
                subcount = 'readBigEndianInt%(length)i(%(full_offset)s)'\
                    % vars()
                builder = 'Subblocks<%(subtype)s>::fromTypeAndCount(%(full_offset)s + %(counterlength)s, %(subcount)s)'\
                    % vars()
            elif block.tag == 'CardBlock':
                builder = 'Subblocks<%(subtype)s>::fromTypeAndLength(%(full_offset)s, %(length)s)'\
                    % vars()
                is_last = True
            else:
                raise
            tag = 'Subblocks<' + subtype + '>'
            offsetextra += ' + ' + ename + '.size()'
            header_dependencies.add('"../DataTypes/Subblocks.h"')
            header_dependencies.add('"../DataTypes/%(subtype)s.h"' % vars())
            thisoutput = '\nreport.writeArray(%(ename)s, tr("%(ename)s"));'\
                % vars()
        else:
            builder = full_offset
            header_dependencies.add('"../DataTypes/' + tag + '.h"')

        table = elem.get('table')
        if table is not None:
            code_dependencies.add('"../formatStrings.h"')
            output += '\nreport.tagValuePair(tr("' + ename\
                + '"), formatStrings::' + table + '(' + ename + '));'
        elif tag in hasToString:
            output += '\nreport.tagValuePair(tr("' + ename + '"), '\
                + ename + '.toString());'
        elif thisoutput is not None:
            output += thisoutput
        elif tag == 'QString':
            output += '\nreport.tagValuePair(tr("%(ename)s"), %(ename)s);'\
                % vars()
        else:
            print(ename, tag)
            output += '\nreport.writeBlock(%(ename)s, tr("%(ename)s"));'\
                % vars()
        elements += '\n' + tag + ' ' + ename + ';'
        initList += ',\n\t' + ename + '(' + builder + ')'
        if not is_last:
            offset += length

    header_content = 'class ' + name + ' : public ' + block.tag + ' {\n'\
        + '\t' + 'Q_DECLARE_TR_FUNCTIONS(' + block.get('name')\
        + ')\npublic:\n' + '\t' + elements.replace('\n', '\n\t') + '\n\n'\
        + '\t' + 'explicit ' + name + '(const DataPointer& filewalker);\n'
    code_content = name + '::' + name + '(const DataPointer& filewalker) : '\
        + initList + '\n{'
    if block.tag == 'VuBlock' and block.get("requiresSignature") == "no":
        code_content += '\thasSignature = false;\n'
    code_content += '}\n'

    header_content += '\t' + '///returns "' + name\
        + '"\n\tQString className() const;\n'
    code_content += '\nQString ' + name + '::className() const {\n\treturn "'\
        + name + '";\n}\n'

    title = block.find('title')
    if title is not None:
        header_content += '\t' + 'QString title() const;\n'
        code_content += '\nQString ' + name + '::title() const {\n\treturn '
        if title.get('dynamic') == 'yes':
            code_content += title.text
        else:
            code_content += 'tr("%s")' % title.text
        code_content += ';\n}\n'

    if block.tag != 'DataType':
        header_content += '\t' + 'static const int Type = '\
            + block.get('type') + ';\n'
    if block.tag != 'CardBlock':
        header_content += '\tint size() const;\n'
        if block.tag == 'DataType':
            header_content += '\tstatic const int staticSize = %(offset)i;\n'\
                % vars()
        if block.tag == 'VuBlock' and block.get('requiresSignature') != "no":
            offset += 128
        code_content += '\nint %(name)s::size() const {\n\treturn %(offset)i%(offsetextra)s;\n}\n'\
            % vars()
    toString = block.find('toString')
    if toString is not None:
        header_content += '\tQString toString() const;\n'
        toString = toString.text
        code_content += '\nQString %(name)s::toString() const {\n\treturn %(toString)s;\n}\n'\
            % vars()

    header_content += '\t' + 'virtual void printOn(Reporter& report) const;\n'\
        + '};\n\n'
    code_content += '\nvoid ' + name + '::printOn(Reporter& report) const {'\
        + output.replace('\n', '\n\t')
    if block.tag == 'CardBlock':
        code_content += "\n\tif (size() != %(offset)i%(offsetextra)s) {\n\t\treport.tagValuePair(\"should have\", %(offset)i%(offsetextra)s);\n\t\treport.tagValuePair(\"has\", size());\n\t}"\
            % vars()
    code_content += '\n}\n'

    comment = block.find('comment')
    if comment is not None:
        header_content = '/** ' + comment.text + '*/\n' + header_content
    write_code_file(name,
                    block.tag + 's',
                    header_content,
                    code_content,
                    header_dependencies,
                    code_dependencies)

class_name = 'CardBlock'
blocklist = tree.findall(class_name)

header_dependencies = set(('"CardBlock.h"', '<QSharedPointer>'))
code_dependencies = set('"%s.h"' % block.get('name') for block in blocklist)
code_dependencies.add('"../readTypes.h"')

header_content = 'QSharedPointer<CardBlock> cardBlockFactory(const DataPointer& start);'
code_content = header_content[:-1] + ' {\n'\
    + '\tif (start.bytesLeft() < 5) {\n\t\treturn QSharedPointer<'\
    + class_name + '>();\n\t}\n'\
    + '\tswitch (readBigEndianInt2(start)) {\n\t\t'\
    + '\n\t\t'.join('case ' + block.get('name')
                    + '::Type: return QSharedPointer<' + block.get('name')
                    + '>(new ' + block.get('name') + '(start));'
                    for block in blocklist)\
    + '\n'\
    + '\t\tdefault: return QSharedPointer<CardBlock>(new CardBlock(start));'\
    '\n\t}\n}\n'
write_code_file('cardBlockFactory',
                'CardBlocks',
                header_content,
                code_content,
                header_dependencies,
                code_dependencies)


class_name = 'VuBlock'
blocklist = tree.findall(class_name)

header_dependencies = set(('"VuBlock.h"', '<QSharedPointer>'))
code_dependencies = set('"%s.h"' % block.get('name') for block in blocklist)
code_dependencies.add('"VuUnknownBlock.h"')
code_dependencies.add('"../readTypes.h"')

header_content = 'QSharedPointer<VuBlock> vuBlockFactory(const DataPointer& start);'
code_content = header_content[:-1] + ' {\n'\
    + '\tif (start.bytesLeft() < 2 || readBigEndianInt1(start) != 0x76) {\n\t'\
    '\treturn QSharedPointer<' + class_name + '>();\n\t}\n'\
    + '\tswitch (readBigEndianInt1(start + 1)) {\n\t\t'\
    + '\n\t\t'.join('case ' + block.get('name')
                    + '::Type: return QSharedPointer<'
                    + block.get('name') + '>(new '
                    + block.get('name') + '(start));'
                    for block in blocklist)\
    + '\n\t\tdefault:'\
    ' return QSharedPointer<VuBlock>(new VuUnknownBlock(start));\n\t}\n}\n'
write_code_file('vuBlockFactory',
                'VuBlocks',
                header_content,
                code_content,
                header_dependencies,
                code_dependencies)
