import sys
import crypt

MAX_LEN = 5

alphabet = "EeTtAaOoIiNnSsRrHhLlDdCcUuMmFfPpGgWwYyBbVvKkXxJjQqZz"

# ensure proper usage
if not len(sys.argv) == 2:
    print("Usage python crack.py hash")
    sys.exit(1)

salt = sys.argv[1][0] + sys.argv[1][1]
hash_in = sys.argv[1]

password = "NOT FOUND"

# try encrypting passwords till password found
# or all possiblities of passwords size MAX_LEN
# exhausted
for i in range(MAX_LEN):
    for j in range(52 ** (i + 1)):
        pass_num = j
        try_pass = ""
        while True:
            try_pass += alphabet[pass_num % 52]
            if pass_num < 52:
                break
            else:
                pass_num //= 52
        if crypt.crypt(try_pass, salt) == hash_in:
            password = try_pass
            i = MAX_LEN
            break

print(password)