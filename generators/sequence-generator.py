f = open('../data/data1.in', 'w', encoding='utf-8')
N = 100000
f.write("{}\n".format(10*N))
for i in range(N):
  f.write("A {}\n".format(i))
  f.write("C {}\n".format(int(N/2)))

for _ in range(6):
  for i in range(N):
    f.write("C {}\n".format(i))

for i in range(N):
  f.write("B {}\n".format(i))
  f.write("C {}\n".format(int(N/2)))

f.close()