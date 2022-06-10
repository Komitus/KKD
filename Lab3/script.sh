tests=./bin/tests
in_dir=$tests/in
encoded_dir=$tests/encoded
decoded_dir=$tests/decoded
target=./bin/main

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' 

encodings=( "gamma" "delta" "omega" "fib")

for entry in "$in_dir"/*
do	
	for encoding in ${encodings[@]}
	do	
		file=`echo "$entry" | sed 's:.*/::'`
		echo $file $encoding
		printf '\n'
		$target -c $in_dir/$file $encoded_dir/$file -e $encoding
		printf '\n'
		$target -d $encoded_dir/$file $decoded_dir/$file -e $encoding
		echo diff $in_dir/$file $decoded_dir/$file
		check=$(diff $in_dir/$file $decoded_dir/$file)
	    if [[ ! -z "$check" ]]
		then
			printf "${RED}FILES DIFFER\n"
			printf "${check}${NC}\n"    
		else
			printf "${GREEN}NO DIFFERENCE${NC}\n"
			printf '\n'
		fi
	done
done
