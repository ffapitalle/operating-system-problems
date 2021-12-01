import logging
import threading
import time

def thread_bear(name):

    while True:        
        s_full_jar.acquire()

        m_jar.acquire()
        logging.info("bear eating")

        for index in range(JAR_SIZE):
            s_places_jar.release()
        
        logging.info("bear finished eating")
        m_jar.release()

        logging.info("bear sleeping")
            


def thread_bee(id):

    while True:

        m_jar.acquire()
        s_places_jar.acquire()

        logging.info("Bee %s: add honey", id)

        if s_places_jar.acquire(blocking=False):
            s_places_jar.release()            
        else:
            s_full_jar.release()

        m_jar.release()
        time.sleep(2)


BEES = 5
JAR_SIZE = 15

s_places_jar = threading.Semaphore(JAR_SIZE)
s_full_jar = threading.Semaphore(0)
m_jar = threading.Lock()

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")

    bear = threading.Thread(target=thread_bear, args=("bear",))
    bear.start()

    threads = list()
    for index in range(BEES):
        s = threading.Thread(target=thread_bee, args=(index, ))
        threads.append(s)
        s.start()

    for index, thread in enumerate(threads):
        thread.join()
    
    bear.join()