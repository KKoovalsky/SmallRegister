import re


def is_header_file(path):
    return path.endswith('.h') or path.endswith('.hpp') or path.endswith('hxx')


def to_include_guard(filename):
    return re.sub(r'[^A-Za-z0-9]+', '_', filename).upper()


def get_class_name_from_few_previous_lines(vim_buffer, current_line_num):
    for i in range(0, 10):
        line = vim_buffer[current_line_num - i]
        match = re.match('(class|struct) (\\w+)', line)
        if match:
            return match.group(2)
    return ''
