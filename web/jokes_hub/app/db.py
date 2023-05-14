import sqlite3

DB_PATH = '/tmp/jokes.db'

def init_db():
    conn = sqlite3.connect(DB_PATH)
    c = conn.cursor()

    c.execute('''CREATE TABLE IF NOT EXISTS jokes
                 (id INTEGER PRIMARY KEY AUTOINCREMENT,
                  joke TEXT,
                  punchline TEXT)''')
    
    c.execute('''CREATE TABLE IF NOT EXISTS flags
                 (id INTEGER PRIMARY KEY AUTOINCREMENT,
                  flag TEXT)''')
    
    c.execute('''CREATE TABLE IF NOT EXISTS notes
                 (id INTEGER PRIMARY KEY AUTOINCREMENT,
                  note TEXT)''')
    
    c.execute("INSERT INTO flags (flag) VALUES (?)", ("shellmates{ar3_sqli_still_4_THing?}", ))




    with open("notes.txt") as f :
        notes = f.readlines()
    
    for note in notes :
        c.execute("INSERT INTO notes (note) VALUES (?)", (note.strip(),))

    conn.commit()
    conn.close()

    with open("jokes.txt") as f :
        jokes = f.readlines()
    for joke in jokes :
        desc, punchline = joke.strip().split("->")
        insert_joke(desc, punchline)


def insert_joke(joke, punchline):
    conn = sqlite3.connect(DB_PATH)
    c = conn.cursor()

    c.execute("INSERT INTO jokes (joke, punchline) VALUES (?, ?)", (joke, punchline))

    conn.commit()
    conn.close()

def get_joke_column(column, id):
    conn = sqlite3.connect(DB_PATH)
    conn.enable_load_extension(True)
    conn.load_extension ("./fileio")
    c = conn.cursor()
    c.execute(f"select {column} from jokes where id={id}")
    row = c.fetchone()
    conn.commit()
    conn.close()
    
    return row
