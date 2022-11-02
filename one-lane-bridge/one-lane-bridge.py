import threading
import random
import time
import logging


def cross_bridge(id, direction, ncars):
    logging.info("[%d] Crossing bridge %s (%d cars on it)", id, direction, ncars)
    time.sleep(random.randint(0,3))

def car_south_north(id):

    logging.info("Created car %d S -> N", id)
    while (True):
        s_allowed_south_north.acquire()
        if s_cars_south_north.acquire(blocking=False):
            s_cars_south_north.release()
        else:
            m_bridge.acquire()
        s_cars_south_north.release()
        s_allowed_south_north.release()

        cross_bridge(id, "S -> N", s_cars_south_north._value)

        s_allowed_south_north.acquire()
        s_cars_south_north.acquire()
        logging.info("[%d] Finished crossing S -> N (%d cars on it)", id, s_cars_south_north._value)
        if s_cars_south_north.acquire(blocking=False):
            s_cars_south_north.release()
        else:
            m_bridge.release()
        s_allowed_south_north.release()

        time.sleep(random.randint(0,10))


def car_north_south(id):

    logging.info("Created car %d N -> S", id)
    while (True):
        s_allowed_north_south.acquire()
        if s_cars_north_south.acquire(blocking=False):
            s_cars_north_south.release()
        else:
            m_bridge.acquire()
        s_cars_north_south.release()
        s_allowed_north_south.release()

        cross_bridge(id, "N -> S", s_cars_north_south._value)

        s_allowed_north_south.acquire()
        s_cars_north_south.acquire()
        logging.info("[%d] Finished crossing N -> S (%d cars on it)", id, s_cars_north_south._value)
        if s_cars_north_south.acquire(blocking=False):
            s_cars_north_south.release()
        else:
            m_bridge.release()
        s_allowed_north_south.release()
        time.sleep(random.randint(0,10))


NUM_CARS = 10

s_allowed_north_south = threading.Semaphore(1)
print(s_allowed_north_south._value)


s_allowed_south_north = threading.Semaphore(1)
s_cars_north_south = threading.Semaphore(0)
s_cars_south_north = threading.Semaphore(0)
m_bridge = threading.Lock()

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")

    threads = list()
    for index in range(NUM_CARS):
        if random.randint(0,1) % 2:
            c = threading.Thread(target=car_north_south,args=(index,))
        else:
            c = threading.Thread(target=car_south_north,args=(index,))

        threads.append(c)
        c.start()

    for index, thread in enumerate(threads):
        thread.join()
    
