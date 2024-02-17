
#!/bin/bash

cd "../data/downloaders"

if [ -f "tickers_for_stocks_downloader.txt" ] &&
  [ -s "tickers_for_stocks_downloader.txt" ]; then

  if python3 -W ignore::FutureWarning stocks_downloader.py; then
    echo "Downloaded stocks dataset."
  elif python -W ignore::FutureWarning stocks_downloader.py; then
    echo "Downloaded stocks dataset."
  else
    echo "Download of stocks dataset failed."
  fi

else
  echo "Bad / nonexistent tickers file (../data/downloaders/tickers_for_stocks_downloader.txt)."
fi
