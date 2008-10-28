for i in input/*.pbm ; do
    echo "==========================================="
    echo "--------- $i"
    echo "==========================================="

    ref="$i.txt"
    cat $ref
    sed -e 's/\(.\)/\1\n/g' $ref > tmp/ref
    total=`cat tmp/ref | wc -l `

    ./ocr_without_preprocess $i tmp/`basename $i` > tmp/without
    echo "_______________without preprocessing"
    cat tmp/without
    cat tmp/without | sed -e 's/\(.\)/\1\n/g' > tmp/without

    ./ocr_with_preprocess $i tmp/`basename $i` > tmp/with
    echo "_______________with preprocessing"
    cat tmp/with
    cat tmp/with | sed -e 's/\(.\)/\1\n/g' > tmp/with


    d_without=`diff  ./tmp/without tmp/ref | diffstat | grep insert | sed -r 's/.*, ([0-9]+) insertion.*/\1/g'`
    echo "$(($d_without * 100 / $total))% missmatch without preprocessing"

    ./ocr_with_preprocess $i tmp/`basename $i` | sed -e 's/\(.\)/\1\n/g' > tmp/with
    d_with=`diff  ./tmp/with tmp/ref | diffstat | grep insert | sed -r 's/.*, ([0-9]+) insertion.*/\1/g'`
    echo "$(($d_with * 100 / $total))% missmatch with preprocessing"
    echo ""
done
