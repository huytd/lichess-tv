#phase 1 : format
clang-format -i lib/* src/*

#phase 2 : build
cd build
if make; then
	if [[ -z $1 ]]; then
		./litv
	fi
fi
