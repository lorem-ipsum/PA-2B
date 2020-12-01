import random

f = open('../data/data4.in', 'w', encoding='utf-8')
N = 100000
array = list(range(N))
f.write("{}\n".format(10*N))
for i in range(N):
  f.write("A {}\n".format(i))
  f.write("C {}\n".format(int(N/2)))

random.shuffle(array)
for _ in range(6):
  for i in array:
    f.write("C {}\n".format(i))

for i in range(N):
  f.write("B {}\n".format(i))
  f.write("C {}\n".format(int(N/2)))

f.close()