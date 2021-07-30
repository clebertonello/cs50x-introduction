import re
from cs50 import get_string

card = get_string("Number: ")

l = len(card)
sum = 0

for i in range(l - 1, -1, -2):
    if i - 1 >= 0:
        x = int(card[i - 1]) * 2
    else:
        x = 0
    y = int(card[i])
    if x >= 10:
        sum += (x % 10) + ((x - (x % 10)) / 10)
    else:
        sum += x
    sum += y

patterns = {
    'VISA': '^4(............$|...............)',
    'AMEX': '^(34|37).............',
    'MASTERCARD': '^(51|52|53|54|55)..............'}

for j in patterns:
    result = re.match(patterns[j], card)
    if result != None:
        break

if (sum % 10) == 0 and result != None:
    print(j)
else:
    print("INVALID")

