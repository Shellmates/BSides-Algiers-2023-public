
def jail(inp):
    return eval(inp)

def guess(inp, guessed):
    import os, random

    os.setregid(65534, 65534), os.setreuid(65534, 65534)
    try:
        r = jail(inp)
        if random.random() == r:
            print('That\'s right, but you ain\'t get any flags')
            guessed.value = False
    except Exception as e:
        print("Don't break my jail")
