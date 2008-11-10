output_dir=tmp/`date '+%y_%m_%d__%H_%M_%S'`
mkdir $output_dir
for i in input/*.pbm ; do
    echo "==========================================="
    echo "--------- $i"
    echo "==========================================="

    ref="$i.txt"
    echo "______________________________Reference"
    cat $ref
    sed -e 's/\(.\)/\1\n/g' $ref > tmp/ref
    total=`cat tmp/ref | wc -l `

    echo "______________________________without preprocessing"
    ./ocr_without_preprocess $i > tmp/without
    cat tmp/without
    cat tmp/without | sed -e 's/\(.\)/\1\n/g' > tmp/without

    echo "______________________________with preprocessing"
    ./ocr_with_preprocess $i $output_dir/`basename $i` > tmp/with
    cat tmp/with
    cat tmp/with | sed -e 's/\(.\)/\1\n/g' > tmp/with


    d_without=`diff  ./tmp/without tmp/ref | diffstat | grep insert | sed -r 's/.*, ([0-9]+) insertion.*/\1/g'`
    echo "$(($d_without * 100 / $total))% missmatch without preprocessing"

    d_with=`diff  ./tmp/with tmp/ref | diffstat | grep insert | sed -r 's/.*, ([0-9]+) insertion.*/\1/g'`
    echo "$(($d_with * 100 / $total))% missmatch with preprocessing"
    echo ""
done
