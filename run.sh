#!/bin/bash
# Đường dẫn tới trình biên dịch C++
CXX=g++

# Cờ biên dịch
CXXFLAGS="-std=c++11"

# Tên file nguồn
SRC="main.cpp"

# Tên file thực thi đầu ra
OUT="program"

# Biên dịch chương trình
$CXX $CXXFLAGS $SRC -o $OUT

# Kiểm tra xem biên dịch có thành công không
if [ $? -eq 0 ]; then
    echo "Compile successfully! Running the program..."
    
    # Chạy chương trình thực thi
    ./$OUT
    
    # Thông báo chương trình đã chạy xong
    echo "Run successfully!"
else
    echo "Compile failed!"
fi
