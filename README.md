# Flower Exchange Project


## Description
• The flower exchange is a system which supports basic trading. There is a `server` so that it can accept buy/sell orders in the form of csv file. Then the exchange application will process the incoming order against existing orders in the Order Book and do a full or partial execution. Every order is replied with an `Execution Report` by the Exchange Application indicating 
the status of the order.

• Orders sometimes could be rejected due to quantity limitations, invalid flower type, etc. and such entries are displayed in `Rejected Execution Report`.
• The system has the ability to reject incoming orders with errorneous formatting including some invalid data types, missing field in csv gracefully.But system generated OrderID will not be generated for such errorneously-formatted orders.



## Build & Run

For WSL/Ubuntu:

```bash
# clone from github
git clone https://github.com/Dinurang/FlowerExchangeSystem.git
cd FlowerExchangeSystem

# Update package list
sudo apt update

# Install dependencies
sudo apt install -y g++ make qtbase5-dev qt5-qmake

# Build
make clean && make

# Run
./bin/server

```

# Usage


• After start Running using `./bin/server` command, CSV file selector `gui` will be appeared. Select the csv file required to be used as test-input by navigating through that gui. Some test examples given in slides are listed under `reports/` directory.

• After the execution of the exchange program, the `execution_rep` (Execution Report) and `rejected_execution_rep`(Rejected Execution Report)  will be generated under `reports/` directory as well as displayed in `gui` as a popup window.