
#!/bin/bash

echo ""

cd ../make

make clean

echo ""

if [ $? -eq 0 ]; then
  echo "Cleaned successfully."
else
  echo "Make failed."
fi

echo ""
