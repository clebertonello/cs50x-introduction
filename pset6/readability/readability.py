from cs50 import get_string


def main():
    text = get_string("Text: ")

    l = count_letters(text)
    w = len(count_words(text))
    s = count_sentences(text)

    CLI = round((0.0588 * (l / (w * 0.01)) - 0.296 * (s / (w * 0.01)) - 15.8), 0)

    print(len(text))

    if (CLI < 1):
        print("Before Grade 1")
    elif (CLI > 16):
        print("Grade 16+")
    else:
        print(f"Grade {int(CLI)}")


def count_letters(text):
    letters = 0
    for j in text:
        if j.isalpha():
            letters += 1

    return letters


def count_words(text):
    words = text.split()
    return words


def count_sentences(text):
    sentences = 0
    for k in text:
        if k == "." or k == "!" or k == "?":
            sentences += 1

    return sentences


if __name__ == "__main__":
    main()