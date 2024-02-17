
#!/bin/bash

cd "../data/downloaders"

if python3 iris_downloader.py; then
  echo "Downloaded iris dataset."
elif python iris_downloader.py; then
  echo "Downloaded iris dataset."
else
  echo "Download of iris dataset failed."
fi
