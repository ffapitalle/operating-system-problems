import logging
import threading
import time
import random

def thread_agent(name):
    logging.info("Agent starting")
    while True:
        s_table.acquire()
        gen = random.randint(0,2)
        if gen == 0:
            s_tobacco.release()
            s_matches.release()
            logging.info("Puts tobacco and matches")
        elif gen == 1:
            s_paper.release()
            s_matches.release()
            logging.info("Puts paper and matches")
        else:
            s_paper.release()
            s_tobacco.release()
            logging.info("Puts tobacco and paper")
            
    logging.info("Agent finishing")

def obtain_ingredients(name, s1, s2):
    s1.acquire()
    if s2.acquire(blocking=False):
        logging.info("%s smoking", name)
        time.sleep(2)
        s_table.release()
    else:
        s1.release()

def thread_smoker(id, name):
    logging.info("Thread %s (%s): starting", name, id)
    while True:
        if id == 0:
            obtain_ingredients(name, s_tobacco, s_matches)
        elif id == 1:
            obtain_ingredients(name, s_paper, s_matches)
        else:
            obtain_ingredients(name, s_tobacco, s_paper)


ingredients = [ "PAPER", "TOBACCO", "MATCHES" ]

s_table = threading.Semaphore(1)
s_paper = threading.Semaphore(0)
s_tobacco = threading.Semaphore(0)
s_matches = threading.Semaphore(0)

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")

    agent = threading.Thread(target=thread_agent, args=("agent",))
    agent.start()

    threads = list()
    for index in range(3):
        s = threading.Thread(target=thread_smoker, args=(index, ingredients[index],))
        threads.append(s)
        s.start()

    for index, thread in enumerate(threads):
        thread.join()
    
    agent.join()