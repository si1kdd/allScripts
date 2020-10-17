import time
import random


beg = time.mktime((1900,1,1,0,0,0,0,0,0))
end = time.mktime((2020,12,31,0,0,0,0,0,0))

for i in range(5):
    t = random.randint(beg, end)
    date = time.strftime('%Y-%m-%d %H:%m:%S', time.localtime(t))
    print(date)

