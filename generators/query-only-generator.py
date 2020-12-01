import random

f = open('../data/data5.in', 'w', encoding='utf-8')
N = 100000
array = list(range(N))
f.write("{}\n".format(10*N))
for i in range(N):
  f.write("A {}\n".format(i))

for _ in range(9):
  random.shuffle(array)
  for i in range(N):
    f.write("C {}\n".format(i))

f.close()