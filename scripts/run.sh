python3 ../generators/sequence-generator.py
python3 ../generators/random-generator.py
python3 ../generators/local-generator.py
python3 ../generators/cycle-generator.py
python3 ../generators/query-only-generator.py
g++ ../src/rbt.cpp -o ../src/rbt
g++ ../src/splay.cpp -o ../src/splay

printf "============ sequence ============\n"
printf "++++++ rbt ++++++"
time ../src/rbt < ../data/data1.in > ../data/data1.out
printf "++++++ splay ++++++"
time ../src/splay < ../data/data1.in > ../data/data1.out
printf "============ random ============\n"
printf "++++++ rbt ++++++"
time ../src/rbt < ../data/data2.in > ../data/data2.out
printf "++++++ splay ++++++"
time ../src/splay < ../data/data2.in > ../data/data2.out
printf "============ local ============\n"
printf "++++++ rbt ++++++"
time ../src/rbt < ../data/data3.in > ../data/data3.out
printf "++++++ splay ++++++"
time ../src/splay < ../data/data3.in > ../data/data3.out
printf "============ cycle ============\n"
printf "++++++ rbt ++++++"
time ../src/rbt < ../data/data4.in > ../data/data4.out
printf "++++++ splay ++++++"
time ../src/splay < ../data/data4.in > ../data/data4.out
printf "============ query-only ============\n"
printf "++++++ rbt ++++++"
time ../src/rbt < ../data/data5.in > ../data/data5.out
printf "++++++ splay ++++++"
time ../src/splay < ../data/data5.in > ../data/data5.out
