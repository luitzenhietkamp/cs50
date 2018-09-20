from nltk.tokenize import sent_tokenize


def merge_unique_string_copies(a, b):
    """Merges two lists of strings and keeps only one unique copy of each string occuring in both lists"""
    output = []

    # for each line in a
    for line_in_a in a:
        should_add = False
        # compare the line to each line in b
        for line_in_b in b:
            if line_in_a == line_in_b:
                should_add = True
                break
        # then in output
        for out_line in output:
            if line_in_a == out_line:
                should_add = False
                break
        # add line to output when needed
        if should_add == True:
            output.append(line_in_a)

    return output


def split_in_substrings(s, n):
    """Splits a list of strings in a list of substrings of length n"""

    output = []

    # for each line in s
    for line_in_s in s:
        # split the line in substrings of size n
        for i in range(len(line_in_s) - n + 1):
            output.append(line_in_s[i:i + n])

    return output


def lines(a, b):
    """Return lines in both a and b"""

    a = a.splitlines()
    b = b.splitlines()

    return merge_unique_string_copies(a, b)


def sentences(a, b):
    """Return sentences in both a and b"""

    a = sent_tokenize(a)
    b = sent_tokenize(b)

    return merge_unique_string_copies(a, b)


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    a = a.splitlines()
    b = b.splitlines()

    a = split_in_substrings(a, n)
    b = split_in_substrings(b, n)

    return merge_unique_string_copies(a, b)
