
#!/bin/bash

echo ""

cd ../make

make lib

echo ""

if [ $? -eq 0 ]; then
  echo "Built library successfully."
else
  echo "Make failed."
fi

echo ""
