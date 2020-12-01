import random

f = open('../data/data3.in', 'w', encoding='utf-8')
N = 100000
f.write("{}\n".format(10*N))
array = list(range(N))
random.shuffle(array)
for i in array:
  f.write("A {}\n".format(i))

for i in range(9*N):
  f.write("C {}\n".format(random.randint(1,int(N/10000))))

f.close()