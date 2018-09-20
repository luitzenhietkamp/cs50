from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # a[i] and b[j]
    # set up an empty matrix output[i][j] with extra row on top and column to the left
    output = [[(0, None) for x in range(len(b) + 1)] for y in range(len(a) + 1)]

    # fill in the top row
    for i in range(len(a)):
        output[i + 1][0] = (i + 1, Operation.DELETED)

    # fill in the first column
    for j in range(len(b)):
        output[0][j + 1] = (j + 1, Operation.INSERTED)

    # navigate through the output matrix
    for i in range(len(a)):
        for j in range(len(b)):
            # calculate costs for deletion, insertion and substitution
            deletion = (output[i][j + 1][0] + 1, Operation.DELETED)
            insertion = (output[i + 1][j][0] + 1, Operation.INSERTED)
            substitution = (output[i][j][0] + (0 if a[i] == b[j] else 1), Operation.SUBSTITUTED)

            # choose based on lowest cost
            if insertion[0] <= deletion[0] and insertion[0] <= substitution[0]:
                output[i + 1][j + 1] = insertion
            elif deletion[0] <= substitution[0]:
                output[i + 1][j + 1] = deletion
            else:
                output[i + 1][j + 1] = substitution

    return output