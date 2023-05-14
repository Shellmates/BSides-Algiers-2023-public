import multiprocessing as mp
from jail import guess

# This is for challenge setup only
if __name__ == '__main__':
    print('Welcome to the game! I give you the possibility to execute whatever you want but I dare you to guess it right...')
    inp = input('>> ')
    guessed = mp.Value('b', False)

    
    child_proc = mp.Process(target=guess, args=(inp,guessed,))
    child_proc.start()

    child_proc.join(timeout=3600)

    if guessed.value == True:
        with open('flag.txt') as f:
            print(f.read())
    else:
        print("Told you, no flags will be given for you")

    if child_proc.exitcode is None:
        child_proc.terminate()

