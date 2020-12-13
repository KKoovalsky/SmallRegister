import re
import vim
import os


def get_file_name_from_path(path):
    return os.path.split(path)[-1]


def find_corresponding_cpp_file_buffer(header_file):
    header_file_name = get_file_name_from_path(header_file)
    header_file_name_no_extension = os.path.splitext(header_file_name)[0]
    corresponding_cpp_file_name = '{}.cpp'.format(
        header_file_name_no_extension)
    it = filter(lambda b: b.name.endswith(corresponding_cpp_file_name),
                vim.buffers)
    return list(it)[0]


def buffer_content_to_string(vim_buffer):
    return '\n'.join(vim_buffer)


def make_source_file_header_string(
        filename, brief_description, author_details):
    return ('/**\n'
            ' * @file        {0}\n'
            ' * @brief       Implements {1}\n'
            ' * @author      {2}\n'
            ' */\n\n'
            '#include "{0}"\n').format(
        filename, brief_description, author_details)


def prepend_multiline_string_to_vim_buffer(string, vim_buffer):
    vim_buffer[:0] = string.splitlines()


def append_multiline_string_to_vim_buffer(string, vim_buffer):
    for line in string.splitlines():
        vim_buffer.append(line)


def has_constructor_declared(header_file_content, class_name):
    regex = r'public:.*{}\(.*private:'.format(class_name)
    return bool(re.search(regex, header_file_content, re.DOTALL))


def make_construction_definition_string(header_file_content, class_name):
    regex = r'public:.*{}\((.*?)\)'.format(class_name)
    match = re.search(regex, header_file_content, re.DOTALL)
    constructor_params = match.group(1)
    return '{0}::{0}({1})\n{{\n}}\n'.format(class_name, constructor_params)


def fill_corresponding_cpp_class_source_file():
    header_file_buffer = vim.current.window.buffer
    header_file = header_file_buffer.name
    cpp_file_buffer = find_corresponding_cpp_file_buffer(header_file)
    regex = 'brief\\s+\\w+\\s+(.*?)\\n.*@author\\s+(.*?)\\n.*class (\\w+)'
    header_file_content = buffer_content_to_string(header_file_buffer)
    scrapped_elements = re.search(regex, header_file_content, re.DOTALL)
    brief_description = scrapped_elements.group(1)
    author_details = scrapped_elements.group(2)
    class_name = scrapped_elements.group(3)
    header_file_name = get_file_name_from_path(header_file)
    source_file_header = make_source_file_header_string(
        header_file_name, brief_description, author_details)
    prepend_multiline_string_to_vim_buffer(source_file_header, cpp_file_buffer)
    if has_constructor_declared(header_file_content, class_name):
        constructor_definition = make_construction_definition_string(
            header_file_content, class_name)
        append_multiline_string_to_vim_buffer(
            constructor_definition, cpp_file_buffer)


def call_hello():
    print('hello')
