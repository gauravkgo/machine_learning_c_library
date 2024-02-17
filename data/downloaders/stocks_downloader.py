
import pandas as pd
import yfinance as yf



TICKERS_FILE = "tickers_for_stocks_downloader.txt"
STOCKS_CSV_FILE = "../datasets/stocks.csv"



with open(TICKERS_FILE, 'r') as read_file:
  ticker_names = [ticker_name for ticker_name in read_file.read().split('\n') if ticker_name]



all_tickers_history = pd.DataFrame()

for ticker_name in ticker_names:

  start_message = f"Downloading {ticker_name.upper()} data ..."
  print(start_message, end = '\r')

  ticker_data = yf.Ticker(ticker_name)
  ticker_history = ticker_data.history(period = "max", interval = "1d")
  ticker_history = ticker_history.reset_index()
  ticker_history.insert(0, "Ticker", [ticker_name.upper()] * len(ticker_history))
  all_tickers_history = pd.concat([all_tickers_history, ticker_history])

  end_message = f"Downloaded {ticker_name.upper()}"
  print((end_message + " " * max(0, len(start_message) - len(end_message))))

all_tickers_history = all_tickers_history.reset_index(drop = True)

all_tickers_history.to_csv(STOCKS_CSV_FILE)


