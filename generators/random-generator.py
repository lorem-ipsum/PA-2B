import random

f = open('../data/data2.in', 'w', encoding='utf-8')
N = 100000
array = list(range(N))
random.shuffle(array)
f.write("{}\n".format(10*N))
for i in array:
  f.write("A {}\n".format(i))
  f.write("C {}\n".format(int(N/2)))

for _ in range(6):
  random.shuffle(array)
  for i in array:
    f.write("C {}\n".format(i))

random.shuffle(array)
for i in array:
  f.write("B {}\n".format(i))
  f.write("C {}\n".format(int(N/2)))

f.close()