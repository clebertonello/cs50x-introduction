import sys
import csv


def main():

    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    with open(sys.argv[1], "r") as file_data:
        reader_data = csv.DictReader(file_data)

        database = []
        for row in reader_data:
            database.append(row)

        for i in range(len(database)):
            for j in database[i]:
                if j != 'name':
                    database[i][j] = int(database[i][j])

        patterns = {}
        for i in database[0]:
            if i != 'name':
                patterns[i] = 0

    with open(sys.argv[2], "r") as file_sequence:
        dna = file_sequence.read()

    match(dna, patterns)

    for i in range(len(database)):
        ver = 0
        for j in patterns:
            if database[i][j] == patterns[j]:
                ver += 1
        if ver == len(patterns):
            break

    if ver == len(patterns):
        print(database[i]['name'])
    else:
        print("No match")

    return


def match(dna, patterns):
    for j in patterns:
        j_len = len(j)
        k = 0
        while k <= len(dna) - j_len:
            if dna[k:k + j_len] == j:
                count = 1
                for l in range(k + j_len, len(dna), j_len):
                    if dna[l: l + j_len] == j:
                        count += 1
                    else:
                        k += j_len * count
                        break
                if count > patterns[j]:
                    patterns[j] = count
            k += 1

    return patterns


if __name__ == "__main__":
    main()