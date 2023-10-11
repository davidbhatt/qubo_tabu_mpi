fo=test_res.txt
rm -f $fo
touch $fo
for i in 1 2 4 8 16
do
    echo $i |tee -a $fo
    mpirun -np $i ./d2ts -i q1.txt -t 1024 | tee -a $fo
done 
