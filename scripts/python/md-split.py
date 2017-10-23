#! /usr/bin/env python

# A script that splits a Markdown file into plain text (for spell checking) and c++ files.


from __future__ import absolute_import, print_function, unicode_literals

import os
import shutil
import io
import argparse

import re, cgi
TAG_REGEX = re.compile(r'(<!--.*?-->|<[^>]*>)')
NAMED_A_TAG_REGEX = re.compile(r'.*name ?= ?"([^"]*)"')

def main():
    """
    This script ended up ugly, so in case somebody wants to reimplement, here is the spec that grew by time.

    What it should do it take a markdown file, and split it into more files. A targetfile should have the same
    number of lines as the original, with source code snippets and markdown non-words removed, for spell-checking.

    Each code snipped should go into a separate file in codedir.

    Each code snipped should get additional C++ code around it to help compile the line in context, with
    some heuristic guessing of what is needed around. The wrapping code should have a token in each line allowing
    other tools to filter out these lines

    The name for each file chosen consists os the section id in the markdown document, a counter for the snippet inside the section.

    Snippets without code (only comments) or containing lines starting with ??? should not yeld files,
    but the counter for naming snippets should still increment.
    """
    parser = argparse.ArgumentParser(description='Split md file into plain text and code blocks')
    parser.add_argument('sourcefile',
                        help='which file to read')
    parser.add_argument('targetfile',
                        help='where to put plain text')
    parser.add_argument('codedir',
                        help='where to put codeblocks')
    args = parser.parse_args()

    # ensure folder exists
    if not os.path.exists(args.codedir):
        os.makedirs(args.codedir)


    if os.path.exists(args.targetfile):
        os.remove(args.targetfile)

    code_block_index = 0
    last_header = ''
    linenum = 0
    with io.open(args.sourcefile, 'r') as read_filehandle:
        with io.open(args.targetfile, 'w') as text_filehandle:
            for line in read_filehandle:
                linenum += 1
                indent_depth = is_code(line)
                if indent_depth:
                    (line, linenum) = process_code(read_filehandle,
                                                    text_filehandle,
                                                    line, linenum,
                                                    args.sourcefile, args.codedir,
                                                    last_header, code_block_index,
                                                    indent_depth)
                    code_block_index += 1
                # reach here either line was not code, or was code
                # and we dealt with n code lines
                if indent_depth < 4 or not is_code(line, indent_depth):
                    # store header id for codeblock
                    section_id = get_marker(line)
                    if section_id is not None:
                        code_block_index = 0
                        last_header = section_id
                    sline = stripped(line)
                    text_filehandle.write(sline)

    assert line_length(args.sourcefile) == line_length(args.targetfile)


def process_code(read_filehandle, text_filehandle, line, linenum, sourcefile, codedir, name, index, indent_depth):
    fenced = (line.strip() == '```')
    if fenced:
        try:
            line = read_filehandle.next()
            linenum += 1
            text_filehandle.write('\n')
        except StopIteration:
            return ('', linenum)
    start_linenum = linenum
    has_actual_code = False
    has_question_marks = False
    linebuffer = []
    while ((fenced and line.strip() != '```') or (not fenced and is_inside_code(line, indent_depth))):
        # copy comments to plain text for spell check
        comment_idx = line.find('//')
        no_comment_line = line
        if comment_idx >= 0:
            no_comment_line = line[:comment_idx].strip()
            text_filehandle.write(line[comment_idx + 2:])
        else:
            # write empty line so line numbers stay stable
            text_filehandle.write('\n')

        if (not has_actual_code
            and not line.strip().startswith('//')
            and not line.strip().startswith('???')
            and not line.strip() == ''):
            has_actual_code = True

        if (not line.strip() == '```'):
            if ('???' == no_comment_line or '...' == no_comment_line):
                has_question_marks = True
            linebuffer.append(dedent(line, indent_depth) if not fenced else line)
        try:
            line = read_filehandle.next()
            linenum += 1
        except StopIteration:
            line = ''
            break
    codefile = os.path.join(codedir, '%s%s.cpp' % (name, index))
    if fenced:
        text_filehandle.write('\n')

    if (has_actual_code and not has_question_marks):
        linebuffer = clean_trailing_newlines(linebuffer)
        write_with_harness(codefile, sourcefile, start_linenum, linebuffer)
    return (line, linenum)


def clean_trailing_newlines(linebuffer):
    result = []
    code_started = False
    linebuffer.reverse()
    for line in linebuffer:
        if not code_started and line == '\n':
            continue
        code_started = True
        result.append(line)
    result.reverse()
    return result


def write_with_harness(codefile, sourcefile, start_linenum, linebuffer):
    '''write output with additional lines to make code likely compilable'''
    # add commonly used headers, so that lines can likely compile.
    # This is work in progress, the main issue remains handling class
    # declarations in in-function code differently
    with io.open(codefile, 'w') as code_filehandle:
        code_filehandle.write('''\
#include<stdio.h>      // by md-split
#include<stdlib.h>     // by md-split
#include<tuple>        // by md-split
#include<utility>      // by md-split
#include<limits>       // by md-split
#include<functional>   // by md-split
#include<string>       // by md-split
#include<map>          // by md-split
#include<iostream>     // by md-split
#include<vector>       // by md-split
#include<algorithm>    // by md-split
#include<memory>       // by md-split
using namespace std;   // by md-split
// %s : %s
''' % (sourcefile, start_linenum))
        # TODO: if not toplevel code, wrap inside class
        for codeline in linebuffer:
            code_filehandle.write(codeline)


def is_code(line, indent_depth = 4):
    '''returns the indent depth, 0 means not code in markup'''
    if line.startswith(' ' * indent_depth):
        return len(line) - len(line.lstrip(' '))
    return 0

def is_inside_code(line, indent_depth):
    return is_code(line, indent_depth) > 0 or line.strip() == ''

def stripped(line):
    # Remove well-formed html tags, fixing mistakes by legitimate users
    sline = TAG_REGEX.sub('', line)
    sline = re.sub('[()\[\]#*]', ' ', line)
    return sline

def dedent(line, indent_depth):
    if line.startswith(' ' * indent_depth):
        return line[indent_depth:]
    if line.startswith('\t'):
        return line[1:]
    return line

def get_marker(line):
    matchlist = TAG_REGEX.findall(line)
    if matchlist:
        namematch = NAMED_A_TAG_REGEX.match(line)
        if namematch:
            return namematch.group(1) # group 0 is full match

    return None

def line_length(filename):
    return sum(1 for line in open(filename))

if __name__ == '__main__':
    main()
